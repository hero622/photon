#include "interfaces.h"

#include "sdk/photon.h"
#include "util/util.h"

#define log_ptr( ptr )                              \
	if ( ptr )                                         \
		util::console::log( "[+] %s: %p.\n", #ptr, ptr ); \
	else                                               \
		util::console::log( "[!] %s is nullptr.\n", #ptr );

bool interfaces::initialize( ) {
	console = new c_console( util::get_module_handle( OS( "tier0.dll", "libtier0.so" ) ) );

	cvar                  = ( i_cvar* ) photon->portal2->get_interface( OS( "vstdlib.dll", "libvstdlib.so" ), "VEngineCvar007" );
	server_game_dll       = photon->portal2->get_interface( MODULE( "server" ), "ServerGameDLL005" );
	engine_api            = photon->portal2->get_interface( MODULE( "engine" ), "VENGINE_LAUNCHER_API_VERSION004" );
	engine_client         = ( i_engine_client* ) photon->portal2->get_interface( MODULE( "engine" ), "VEngineClient015" );
	server_plugin_helpers = photon->portal2->get_interface( MODULE( "engine" ), "ISERVERPLUGINHELPERS001" );
	surface               = ( i_surface* ) photon->portal2->get_interface( MODULE( "vguimatsurface" ), "VGUI_Surface031" );
	engine_vgui_internal  = photon->portal2->get_interface( MODULE( "engine" ), "VEngineVGui001" );
	input_system          = ( i_input_system* ) photon->portal2->get_interface( MODULE( "inputsystem" ), "InputSystemVersion001" );
	base_client_dll       = photon->portal2->get_interface( MODULE( "client" ), "VClient016" );
	scheme_manager        = ( i_scheme_manager* ) photon->portal2->get_interface( MODULE( "vgui2" ), "VGUI_Scheme010" );
	input_stack_system    = ( i_input_stack_system* ) photon->portal2->get_interface( MODULE( "inputsystem" ), "InputStackSystemVersion001" );
	vgui_input            = photon->portal2->get_interface( MODULE( "vgui2" ), "VGUI_Input005" );

	const auto tier0 = util::get_module_handle( OS( "tier0.dll", "libtier0.so" ) );
	mem_alloc        = *util::get_sym_addr< i_mem_alloc** >( tier0, "g_pMemAlloc" );
	command_line     = util::get_sym_addr< i_command_line* ( * ) ( ) >( tier0, "CommandLine" )( );

	engine       = **reinterpret_cast< void*** >( util::get_virtual< 12 >( interfaces::engine_api ) + OS( 0x2, 0x4 ) );
	client_state = util::read< void* ( * ) ( ) >( util::get_virtual< 7 >( interfaces::engine_client ) + OS( 0x4, 0x9 ) )( );
	font_manager = util::read< c_font_manager* ( * ) ( ) >( util::get_virtual< 132 >( interfaces::surface ) + OS( 0x8, 0x9 ) )( );
	scheme       = interfaces::scheme_manager->get_i_scheme( 1 );

	log_ptr( mem_alloc );
	log_ptr( command_line );
	log_ptr( engine );
	log_ptr( client_state );
	log_ptr( font_manager );
	log_ptr( scheme );

	return true;
}

void interfaces::uninitialize( ) {
	delete_ptr( console );
}
