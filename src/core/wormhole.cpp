#include "wormhole.h"

#include "core/hooks/hooks.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/menu.h"
#include "core/mods/mods.h"

#include <cstring>

c_wormhole wormhole;

expose_single_interface_globalvar( c_wormhole, i_server_plugin_callbacks, interfaceversion_iserverplugincallbacks, wormhole );

wh_api::c_shared *wh;

c_wormhole::c_wormhole( ) {
	this->plugin = new c_plugin( );
	wh = new wh_api::c_shared( );
	wh->portal2 = new c_portal2( );
	wh->hook = new c_hook( );
	wh->events = new c_events( );
	wh->huds = new c_huds( );
	wh->render = new c_render( );
	wh->input = new c_input( );
}

bool c_wormhole::load( sdk::create_interface_fn interface_factory, sdk::create_interface_fn game_server_factory ) {
#ifdef _DEBUG
	utils::console::alloc( );
#endif

	if ( interfaces::initialize( ) ) {
		if ( hooks::initialize( ) ) {
			c_command::regall( );

			c_variable::regall( );

			menu::initialize( );

			wh->portal2->console->color_msg( { 0, 255, 0, 255 }, "Wormhole loaded.\n" );

			return true;
		}
	}

	return false;
}

bool c_wormhole::get_plugin( ) {
	auto server_plugin_helpers = reinterpret_cast<uintptr_t>( wh->portal2->server_plugin_helpers );
	auto size = *reinterpret_cast<int *>( server_plugin_helpers + c_server_plugin_size );
	if ( size > 0 ) {
		auto plugins = *reinterpret_cast<uintptr_t *>( server_plugin_helpers + c_server_plugin_plugins );
		for ( auto i = 0; i < size; ++i ) {
			auto ptr = *reinterpret_cast<sdk::c_plugin **>( plugins + sizeof( uintptr_t ) * i );
			if ( !std::strcmp( ptr->name, wormhole_plugin_sig ) ) {
				this->plugin->ptr = ptr;
				this->plugin->index = i;
				return true;
			}
		}
	}
	return false;
}

void c_wormhole::unload( ) {
	if ( unloading ) return;
	unloading = true;

	mods::unloadall( );

	menu::uninitialize( );

	c_variable::unregall( );

	c_command::unregall( );

	hooks::uninitialize( );

	if ( wormhole.get_plugin( ) ) {
		auto unload_cmd = std::string( "plugin_unload " ) + std::to_string( wormhole.plugin->index );
		wh->portal2->engine_client->cbuf_add( unload_cmd.c_str( ), safe_unload_delay );
	}

	wh->portal2->console->msg( "Goodbye.\n" );

	interfaces::uninitialize( );

#ifdef _DEBUG
	utils::console::free( );
#endif

	delete_ptr( wh->input );
	delete_ptr( wh->render );
	delete_ptr( wh->huds );
	delete_ptr( wh->events );
	delete_ptr( wh->hook );
	delete_ptr( wh->portal2 );
	delete_ptr( wh );
	delete_ptr( this->plugin );
}

void c_wormhole::pause( ) {}
void c_wormhole::un_pause( ) {}
const char *c_wormhole::get_plugin_description( ) {
	return wormhole_plugin_sig;
}
void c_wormhole::level_init( char const *p_map_name ) {}
void c_wormhole::server_activate( void *p_edict_list, int edict_count, int client_max ) {}
void c_wormhole::game_frame( bool simulating ) {}
void c_wormhole::level_shutdown( ) {}
void c_wormhole::client_fully_connect( void *p_edict ) {}
void c_wormhole::client_active( void *p_entity ) {}
void c_wormhole::client_disconnect( void *p_entity ) {}
void c_wormhole::client_put_in_server( void *p_entity, char const *playername ) {}
void c_wormhole::set_command_client( int index ) {}
void c_wormhole::client_settings_changed( void *p_edict ) {}
int c_wormhole::client_connect( bool *b_allow_connect, void *p_entity, const char *psz_name, const char *psz_address, char *reject, int maxrejectlen ) {
	return 0;
}
int c_wormhole::client_command( void *p_entity, const void *&args ) {
	return 0;
}
int c_wormhole::network_id_validated( const char *psz_user_name, const char *psz_network_id ) {
	return 0;
}
void c_wormhole::on_query_cvar_value_finished( int i_cookie, void *p_player_entity, int e_status, const char *p_cvar_name, const char *p_cvar_value ) {}
void c_wormhole::on_edict_allocated( void *edict ) {}
void c_wormhole::on_edict_freed( const void *edict ) {}
