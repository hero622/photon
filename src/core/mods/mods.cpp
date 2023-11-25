#include "mods.h"

#include "core/wormhole.h"

#include <filesystem>
#include <iostream>

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

			mod_info_t info;
			info.handle = lib;
			info.ptr = mod;
			info.is_loaded = false;

			mod_list.insert( std::make_pair( name, info ) );

			return true;
		}
	}

#ifdef _WIN32
	wh->portal2->console->warning( "Failed to load library (%lu).\n", GetLastError( ) );
#else
	wh->portal2->console->warning( "Failed to load library (%s).\n", dlerror( ) );
#endif

	return false;
}

void mods::unload( const char *name ) {
	if ( !mod_list.count( name ) ) return;

	auto &mod = mod_list[ name ];

	if ( mod.is_loaded )
		mod.ptr->unload( );

#ifdef _WIN32
	FreeLibrary( ( HMODULE ) mod.handle );
#else
	dlclose( mod.handle );
#endif

	mod_list.erase( name );
}

bool mods::loadall( ) {
	bool had_fail = false;
	for ( const auto &entry : std::filesystem::directory_iterator( "wormhole" ) ) {
		bool result = load( entry.path( ).stem( ).string( ).c_str( ) );

		if ( !result ) {
			had_fail = true;
		}
	}

	return !had_fail;
}

void mods::unloadall( ) {
	for ( const auto &mod : mod_list ) {
		if ( mod.second.is_loaded )
			mod.second.ptr->unload( );

#ifdef _WIN32
		FreeLibrary( ( HMODULE ) mod.second.handle );
#else
		dlclose( mod.second.handle );
#endif
	}

	mod_list.clear( );
}

bool mods::enable( mod_info_t *mod ) {
	bool result = false;
	if ( !mod->is_loaded ) {
		result = mod->ptr->load( wh );
		mod->is_loaded = true;
	}

	return result;
}

void mods::disable( mod_info_t *mod ) {
	if ( !mod->is_loaded )
		return;

	mod->is_loaded = false;
	mod->ptr->unload( );
}

void mods::print( ) {
	wh->portal2->console->msg( "Loaded wormhole mods (%d):\n", mod_list.size( ) );
	for ( const auto &mod : mod_list ) {
		const char *status = mod.second.is_loaded ? "enabled" : "disabled";
		const auto info = mod.second.ptr->get_info( );

		wh->portal2->console->msg( "%s: %s@%s (%s)\n", mod.first.c_str( ), info.name, info.version, status );
	}
}

void mods::post_event( void *sender, const char *msg ) {
	auto msg_s = std::string( msg );

	if ( sender != &wormhole ) {
		const auto mod = reinterpret_cast<wh_api::i_wormhole_mod *>( sender );
		msg_s = std::string( mod->get_info( ).name ) + std::string( ":" ) + msg_s;
	}

	for ( const auto &mod : mod_list ) {
		if ( mod.second.is_loaded )
			mod.second.ptr->on_event( msg_s.c_str( ) );
	}
}
