#include "photon.h"

#include "core/convars/convars.h"
#include "core/hooks/hooks.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/gui.h"
#include "core/mods/mods.h"
#include "util/util.h"

#include <cstring>

c_photon plugin;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( c_photon, i_server_plugin_callbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, plugin );

photon_api::c_shared* photon;

bool c_photon::load( create_interface_fn interface_factory, create_interface_fn game_server_factory ) {
	info           = new plugin_info_t( );
	photon         = new photon_api::c_shared( );
	photon->common = new c_common( );
	photon->con    = new c_con( );
	photon->hook   = new c_hook( );
	photon->hud    = new c_hud( );
	photon->render = new c_render( );
	photon->input  = new c_input( );
	photon->menu   = new c_menu( );

#ifdef _DEBUG
	util::console::alloc( );
#endif

	if ( interfaces::initialize( ) ) {
		if ( hooks::initialize( ) ) {
			convars::initialize( );

			gui::initialize( );

			// only works when done early enough
			interfaces::command_line->append_parm( "-background", "5" );

			photon->common->log( { 0, 255, 0, 255 }, "Photon loaded.\n" );

			if ( !mods::loadall( ) )
				photon->common->log_warn( "Failed to load one or more mods.\n" );

			return true;
		} else
			photon->common->log_warn( "Failed to initialize one or more hooks.\n" );
	} else
		photon->common->log_warn( "Failed to initialize one or more interfaces.\n" );

	return false;
}

bool c_photon::get_info( ) {
	auto server_plugin_helpers = reinterpret_cast< uint8_t* >( interfaces::server_plugin_helpers );
	auto size                  = *reinterpret_cast< int* >( server_plugin_helpers + SERVER_PLUGIN_SIZE );
	if ( size > 0 ) {
		auto plugins = *reinterpret_cast< uint8_t** >( server_plugin_helpers + SERVER_PLUGIN_PLUGINS );
		for ( auto i = 0; i < size; ++i ) {
			auto ptr = *reinterpret_cast< c_plugin** >( plugins + sizeof( uint8_t* ) * i );
			if ( !std::strcmp( ptr->name, PLUGIN_SIGNATURE ) ) {
				info->ptr   = ptr;
				info->index = i;
				return true;
			}
		}
	}
	return false;
}

void c_photon::unload( ) {
	if ( unloading )
		return;
	unloading = true;

	mods::unloadall( );

	// this doesnt really do anything lol
	interfaces::command_line->remove_parm( "-background" );

	gui::uninitialize( );

	convars::uninitialize( );

	hooks::uninitialize( );

	if ( plugin.get_info( ) ) {
		auto unload_cmd = std::string( "plugin_unload " ) + std::to_string( plugin.info->index );
		interfaces::engine_client->cbuf_add( unload_cmd.c_str( ), SAFE_UNLOAD_DELAY );
	}

	photon->common->log( "Goodbye.\n" );

	interfaces::uninitialize( );

#ifdef _DEBUG
	util::console::free( );
#endif

	DELETE_PTR( photon->menu );
	DELETE_PTR( photon->input );
	DELETE_PTR( photon->render );
	DELETE_PTR( photon->hud );
	DELETE_PTR( photon->common );
	DELETE_PTR( photon->hook );
	DELETE_PTR( photon->con );
	DELETE_PTR( photon->common );
	DELETE_PTR( photon );
	DELETE_PTR( info );
}
