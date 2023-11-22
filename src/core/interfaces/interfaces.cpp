#include "interfaces.h"

#include "wormhole-sdk/portal2.h"
#include "wormhole-sdk/wormhole.h"

bool interfaces::initialize( ) {
	wh->portal2->console = new c_console( utils::memory::get_module_handle( modules::tier0 ) );

	wh->portal2->cvar = get_interface<i_cvar>( modules::tier1, "VEngineCvar007" );
	wh->portal2->server_game_dll = get_interface<void *>( module( "server" ), "ServerGameDLL005" );
	wh->portal2->engine_api = get_interface<void *>( module( "engine" ), "VENGINE_LAUNCHER_API_VERSION004" );
	wh->portal2->engine_client = get_interface<i_engine_client>( module( "engine" ), "VEngineClient015" );
	wh->portal2->server_plugin_helpers = get_interface<void *>( module( "engine" ), "ISERVERPLUGINHELPERS001" );
	wh->portal2->surface = get_interface<i_surface>( module( "vguimatsurface" ), "VGUI_Surface031" );
	wh->portal2->engine_vgui_internal = get_interface<void *>( module( "engine" ), "VEngineVGui001" );
	wh->portal2->input_system = get_interface<i_input_system>( module( "inputsystem" ), "InputSystemVersion001" );
	wh->portal2->base_client_dll = get_interface<void *>( module( "client" ), "VClient016" );
	wh->portal2->scheme_manager = get_interface<i_scheme_manager>( module( "vgui2" ), "VGUI_Scheme010" );
	wh->portal2->input_stack_system = get_interface<i_input_stack_system>( module( "inputsystem" ), "InputStackSystemVersion001" );
	wh->portal2->vgui_input = get_interface<void *>( module( "vgui2" ), "VGUI_Input005" );

	wh->portal2->engine = **reinterpret_cast<void ***>( utils::memory::get_virtual( wh->portal2->engine_api, offsets::is_running_simulation ) + offsets::eng );
	wh->portal2->client_state = utils::memory::read<void *( * ) ( )>( utils::memory::get_virtual( wh->portal2->engine_client, offsets::client_cmd ) + offsets::get_client_state )( );
	wh->portal2->mem_alloc = *utils::memory::get_sym_addr<i_mem_alloc **>( utils::memory::get_module_handle( modules::tier0 ), "g_pMemAlloc" );
	wh->portal2->scheme = wh->portal2->scheme_manager->get_i_scheme( 1 );

	return true;
}

void interfaces::uninitialize( ) {
	delete_ptr( wh->portal2->console );
}
