#include "wormhole-sdk/convar.h"

#include "wormhole-sdk/wormhole.h"

// this whole thing is still a bit scuffed, and i do not understand everything 100%, needs some work
// also doesnt work on linux currently i think

sdk::con_var *c_convar::create_convar( const char *name, const char *default_value, int flags, const char *help_string, bool has_min, float min, bool has_max, float max ) {
	void *cvar = wh->portal2->mem_alloc->alloc( 0 );  //	honestly i have no clue what size i should pass here but this works

	using ctor_fn_t = sdk::con_var *( __rescall * ) ( void *, const char *, const char *, int, const char *, bool, float, bool, float );
	const auto ctor_fn = reinterpret_cast<ctor_fn_t>( utils::memory::pattern_scan( modules::tier1, signatures::convar_ctor ) );
	const auto ptr = ctor_fn( cvar, name, default_value, flags, help_string, has_min, min, has_max, max );

	convars.insert( std::make_pair( name, ptr ) );

	return ptr;
}
void c_convar::destruct_convar( const char *name ) {
	if ( !convars.count( name ) )
		return;

	const auto cvar = convars[ name ];

#ifdef _WIN32
	const auto dtor_fn = reinterpret_cast<void *( __rescall * ) ( void *, char )>( utils::memory::pattern_scan( modules::tier1, signatures::convar_dtor ) );
	dtor_fn( cvar, 0 );
#else
	const auto dtor_fn = reinterpret_cast<void *( __rescall * ) ( void * )>( utils::memory::pattern_scan( modules::tier1, signatures::convar_dtor ) );
	dtor_fn( cvar );
#endif

	convars.erase( name );
}

sdk::con_command *c_convar::create_concmd( const char *name, sdk::fn_command_callback_t cbk, const char *help_string, int flags ) {
	auto concmd = new sdk::con_command( name, cbk, help_string, flags );

	*( void ** ) concmd = *( void ** ) wh->portal2->cvar->find_command_base( "listdemo" );

	wh->portal2->cvar->register_con_command( concmd );

	concmds.insert( std::make_pair( name, concmd ) );

	return concmd;
}
void c_convar::destruct_concmd( const char *name ) {
	if ( !concmds.count( name ) )
		return;

	auto concmd = concmds[ name ];

	wh->portal2->cvar->unregister_con_command( concmd );

	delete_ptr( concmd );

	concmds.erase( name );
}

void c_convar::hook_cmd( const char *name, sdk::fn_command_callback_t detour, sdk::fn_command_callback_t &original ) {
	auto concmd = reinterpret_cast<sdk::con_command *>( wh->portal2->cvar->find_command_base( name ) );

	if ( !concmd )
		return;

	original = concmd->fn_command_callback;
	concmd->fn_command_callback = detour;
}
void c_convar::unhook_cmd( const char *name, sdk::fn_command_callback_t original ) {
	auto concmd = reinterpret_cast<sdk::con_command *>( wh->portal2->cvar->find_command_base( name ) );

	if ( !concmd || !original )
		return;

	concmd->fn_command_callback = original;
}
