#include "mods.h"

#include "core/wormhole.h"

#ifdef _WIN32
#	include <windows.h>
#	include <memoryapi.h>
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#endif

bool mods::load( const char *name ) {
	if ( mod_list.count( name ) ) return false;

#ifdef _WIN32
	void *lib = LoadLibraryA( utils::string::ssprintf( "wormhole/%s.dll", name ).c_str( ) );
#else
	void *lib = dlopen( utils::string::ssprintf( "wormhole/%s.so", name ).c_str( ), RTLD_NOW );
#endif

	if ( lib ) {
		using create_mod_fn = wh_api::i_wormhole_mod *( * ) ( );
		const auto fn = utils::memory::get_sym_addr<create_mod_fn>( lib, "create_mod" );

		if ( fn ) {
			auto mod = fn( );

			if ( !mod ) return false;

			auto result = mod->load( wh );

			if ( !result )
				return false;

			mod_info_t info;
			info.handle = lib;
			info.ptr = mod;

			mod_list.insert( std::make_pair( name, info ) );

			return true;
		}
	}

#ifdef _WIN32
	wh->portal2->console->msg( "Failed to load library (%lu).\n", GetLastError( ) );
#else
	wh->portal2->console->msg( "Failed to load library (%s).\n", dlerror( ) );
#endif

	return false;
}

void mods::unload( const char *name ) {
	if ( !mod_list.count( name ) ) return;

	auto mod = mod_list[ name ];

	mod.ptr->unload( );

#ifdef _WIN32
	FreeLibrary( ( HMODULE ) mod.handle );
#else
	dlclose( mod.handle );
#endif

	mod_list.erase( name );
}

void mods::unloadall( ) {
	for ( const auto &mod : mod_list ) {
		mod.second.ptr->unload( );
	}
}

void mods::print( ) {
	wh->portal2->console->msg( "Loaded wormhole mods (%d):\n", mod_list.size( ) );
	for ( const auto &mod : mod_list ) {
		wh->portal2->console->msg( "%s\n", mod.first.c_str( ) );
	}
}

void mods::post_event( void *sender, const char *msg ) {
	auto msg_s = std::string( msg );

	if ( sender != &wormhole ) {
		msg_s = std::string( ( ( wh_api::i_wormhole_mod * ) sender )->get_info( )->name ) + std::string( ":" ) + msg_s;
	}

	for ( const auto &mod : mod_list ) {
		mod.second.ptr->on_event( msg_s.c_str( ) );
	}
}

create_con_command( wh_load, "wh_load <mod name> - load a wormhole mod.\n" ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::load( args[ 1 ] );
	} else {
		wh->portal2->console->msg( wh_load_cmd.ptr->help_string );
	}
}

create_con_command( wh_unload, "wh_unload <mod name> - unload a wormhole mod.\n" ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::unload( args[ 1 ] );
	} else {
		wh->portal2->console->msg( wh_unload_cmd.ptr->help_string );
	}
}

create_con_command( wh_print, "prints a list of all loaded wormhole mods.\n" ) {
	mods::print( );
}
