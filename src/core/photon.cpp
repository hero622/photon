#include "photon.h"

#include "core/convars/convars.h"
#include "core/hooks/hooks.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/gui.h"
#include "core/mods/mods.h"

#include <cstring>

c_photon plugin;

expose_single_interface_globalvar( c_photon, i_server_plugin_callbacks, interfaceversion_iserverplugincallbacks, plugin );

photon_api::c_shared *photon;

c_photon::c_photon( ) {
	info = new plugin_info_t( );
	photon = new photon_api::c_shared( );
	photon->portal2 = new c_portal2( );
	photon->con = new c_con( );
	photon->hook = new c_hook( );
	photon->event = new c_event( );
	photon->hud = new c_hud( );
	photon->render = new c_render( );
	photon->input = new c_input( );
	photon->menu = new c_menu( );
}

bool c_photon::load( sdk::create_interface_fn interface_factory, sdk::create_interface_fn game_server_factory ) {
#ifdef _DEBUG
	utils::console::alloc( );
#endif

	if ( interfaces::initialize( ) ) {
		if ( hooks::initialize( ) ) {
			convars::initialize( );

			gui::initialize( );

			photon->portal2->console->color_msg( { 0, 255, 0, 255 }, "Photon loaded.\n" );

			if ( !mods::loadall( ) )
				photon->portal2->console->warning( "Failed to load one or more mods.\n" );

			return true;
		}
	}

	return false;
}

bool c_photon::get_info( ) {
	auto server_plugin_helpers = reinterpret_cast<uint8_t *>( photon->portal2->server_plugin_helpers );
	auto size = *reinterpret_cast<int *>( server_plugin_helpers + c_server_plugin_size );
	if ( size > 0 ) {
		auto plugins = *reinterpret_cast<uint8_t **>( server_plugin_helpers + c_server_plugin_plugins );
		for ( auto i = 0; i < size; ++i ) {
			auto ptr = *reinterpret_cast<sdk::c_plugin **>( plugins + sizeof( uint8_t * ) * i );
			if ( !std::strcmp( ptr->name, photon_plugin_sig ) ) {
				info->ptr = ptr;
				info->index = i;
				return true;
			}
		}
	}
	return false;
}

void c_photon::unload( ) {
	if ( unloading ) return;
	unloading = true;

	mods::unloadall( );

	gui::uninitialize( );

	convars::uninitialize( );

	hooks::uninitialize( );

	if ( plugin.get_info( ) ) {
		auto unload_cmd = std::string( "plugin_unload " ) + std::to_string( plugin.info->index );
		photon->portal2->engine_client->cbuf_add( unload_cmd.c_str( ), safe_unload_delay );
	}

	photon->portal2->console->msg( "Goodbye.\n" );

	interfaces::uninitialize( );

#ifdef _DEBUG
	utils::console::free( );
#endif

	delete_ptr( photon->menu );
	delete_ptr( photon->input );
	delete_ptr( photon->render );
	delete_ptr( photon->hud );
	delete_ptr( photon->event );
	delete_ptr( photon->hook );
	delete_ptr( photon->con );
	delete_ptr( photon->portal2 );
	delete_ptr( photon );
	delete_ptr( info );
}

void c_photon::pause( ) {}
void c_photon::un_pause( ) {}
const char *c_photon::get_plugin_description( ) {
	return photon_plugin_sig;
}
void c_photon::level_init( char const *map_name ) {}
void c_photon::server_activate( void *edict_list, int edict_count, int client_max ) {}
void c_photon::game_frame( bool simulating ) {}
void c_photon::level_shutdown( ) {}
void c_photon::client_fully_connect( void *edict ) {}
void c_photon::client_active( void *entity ) {}
void c_photon::client_disconnect( void *entity ) {}
void c_photon::client_put_in_server( void *entity, char const *playername ) {}
void c_photon::set_command_client( int index ) {}
void c_photon::client_settings_changed( void *edict ) {}
int c_photon::client_connect( bool *allow_connect, void *entity, const char *name, const char *address, char *reject, int maxrejectlen ) {
	return 0;
}
int c_photon::client_command( void *entity, const void *&args ) {
	return 0;
}
int c_photon::network_id_validated( const char *user_name, const char *network_id ) {
	return 0;
}
void c_photon::on_query_cvar_value_finished( int cookie, void *player_entity, int status, const char *cvar_name, const char *cvar_value ) {}
void c_photon::on_edict_allocated( void *edict ) {}
void c_photon::on_edict_freed( const void *edict ) {}
