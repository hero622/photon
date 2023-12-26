#include "interfaces.h"

#include "photon-sdk/photon.h"

bool interfaces::initialize( ) {
	photon->portal2->console = new c_console( utils::memory::get_module_handle( os( "tier0.dll", "libtier0.so" ) ) );

	photon->portal2->cvar = ( i_cvar * ) photon->portal2->get_interface( os( "vstdlib.dll", "libvstdlib.so" ), "VEngineCvar007" );
	photon->portal2->server_game_dll = photon->portal2->get_interface( module( "server" ), "ServerGameDLL005" );
	photon->portal2->engine_api = photon->portal2->get_interface( module( "engine" ), "VENGINE_LAUNCHER_API_VERSION004" );
	photon->portal2->engine_client = ( i_engine_client * ) photon->portal2->get_interface( module( "engine" ), "VEngineClient015" );
	photon->portal2->server_plugin_helpers = photon->portal2->get_interface( module( "engine" ), "ISERVERPLUGINHELPERS001" );
	photon->portal2->surface = ( i_surface * ) photon->portal2->get_interface( module( "vguimatsurface" ), "VGUI_Surface031" );
	photon->portal2->engine_vgui_internal = photon->portal2->get_interface( module( "engine" ), "VEngineVGui001" );
	photon->portal2->input_system = ( i_input_system * ) photon->portal2->get_interface( module( "inputsystem" ), "InputSystemVersion001" );
	photon->portal2->base_client_dll = photon->portal2->get_interface( module( "client" ), "VClient016" );
	photon->portal2->scheme_manager = ( i_scheme_manager * ) photon->portal2->get_interface( module( "vgui2" ), "VGUI_Scheme010" );
	photon->portal2->input_stack_system = ( i_input_stack_system * ) photon->portal2->get_interface( module( "inputsystem" ), "InputStackSystemVersion001" );
	photon->portal2->vgui_input = photon->portal2->get_interface( module( "vgui2" ), "VGUI_Input005" );

	photon->portal2->engine = **reinterpret_cast<void ***>( utils::memory::get_virtual<12>( photon->portal2->engine_api ) + os( 0x2, 0x4 ) );
	photon->portal2->client_state = utils::memory::read<void *( * ) ( )>( utils::memory::get_virtual<7>( photon->portal2->engine_client ) + os( 0x4, 0x9 ) )( );
	photon->portal2->mem_alloc = *utils::memory::get_sym_addr<i_mem_alloc **>( utils::memory::get_module_handle( os( "tier0.dll", "libtier0.so" ) ), "g_pMemAlloc" );
	photon->portal2->scheme = photon->portal2->scheme_manager->get_i_scheme( 1 );
	photon->portal2->font_manager = utils::memory::read<c_font_manager *( * ) ( )>( utils::memory::get_virtual<132>( photon->portal2->surface ) + os( 0x8, 0x9 ) )( );

	return true;
}

void interfaces::uninitialize( ) {
	delete_ptr( photon->portal2->console );
}
