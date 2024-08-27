// i wanted to call this "con" but windows wasnt letting me >:(

#include "sdk/ccon.h"

#include "core/interfaces/interfaces.h"
#include "sdk/photon.h"
#include "util/util.h"

static std::unordered_map< std::string, con_command* >          concmds;
static std::unordered_map< std::string, fn_command_callback_t > hooked_cbks;

con_command* c_con::create_concmd( const char* name, fn_command_callback_t cbk, const char* help_string, int flags ) {
	// use the game's memory allocator
	auto concmd = reinterpret_cast< con_command* >( interfaces::mem_alloc->alloc( sizeof( con_command ) ) );
	memset( concmd, 0, sizeof( con_command ) );

	// steal vtable from the game
	concmd->vtable = interfaces::cvar->find_command_base( "listdemo" )->vtable;

	concmd->name                = name;
	concmd->fn_command_callback = cbk;
	concmd->help_string         = help_string;
	concmd->flags               = flags;

	// TODO: implement command completion
	concmd->fn_completion_callback           = 0;
	concmd->has_completion_callback          = false;
	concmd->using_new_command_callback       = true;
	concmd->using_command_callback_interface = false;

	interfaces::cvar->register_con_command( concmd );

	concmds.insert( std::make_pair( name, concmd ) );

	return concmd;
}
void c_con::destruct_concmd( const char* name ) {
	if ( !concmds.count( name ) )
		return;

	auto concmd = concmds[ name ];

	interfaces::cvar->unregister_con_command( concmd );

	interfaces::mem_alloc->free( concmd );

	concmds.erase( name );
}

void c_con::hook_cmd( const char* name, fn_command_callback_t detour ) {
	auto concmd = reinterpret_cast< con_command* >( interfaces::cvar->find_command_base( name ) );

	if ( !concmd )
		return;

	hooked_cbks.insert( std::make_pair( name, concmd->fn_command_callback ) );
	concmd->fn_command_callback = detour;
}
void c_con::unhook_cmd( const char* name ) {
	auto concmd   = reinterpret_cast< con_command* >( interfaces::cvar->find_command_base( name ) );
	auto original = hooked_cbks[ name ];
	hooked_cbks.erase( name );

	if ( !concmd || !original )
		return;

	concmd->fn_command_callback = original;
}

con_var* c_con::find_con_var( const char* name ) {
	return reinterpret_cast< con_var* >( interfaces::cvar->find_command_base( name ) );
}

con_command* c_con::find_con_command( const char* name ) {
	return reinterpret_cast< con_command* >( interfaces::cvar->find_command_base( name ) );
}
