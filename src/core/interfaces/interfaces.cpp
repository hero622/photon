#include "interfaces.h"

#include "sdk/photon.h"
#include "util/util.h"

#define LOG_PTR( ptr )                              \
	if ( ptr )                                         \
		util::console::log( "[+] %s: %p.\n", #ptr, ptr ); \
	else                                               \
		util::console::log( "[!] %s is nullptr.\n", #ptr );

bool interfaces::initialize( ) {
	console = new c_console( util::get_module_handle( OS( "tier0.dll", "libtier0.so" ) ) );

	cvar                  = ( i_cvar* ) photon->common->get_interface( OS( "vstdlib.dll", "libvstdlib.so" ), "VEngineCvar007" );
	engine_api            = photon->common->get_interface( MODULE( "engine" ), "VENGINE_LAUNCHER_API_VERSION004" );
	engine_client         = ( i_engine_client* ) photon->common->get_interface( MODULE( "engine" ), "VEngineClient015" );
	server_plugin_helpers = photon->common->get_interface( MODULE( "engine" ), "ISERVERPLUGINHELPERS001" );
	surface               = ( i_surface* ) photon->common->get_interface( MODULE( "vguimatsurface" ), "VGUI_Surface031" );
	input_system          = ( i_input_system* ) photon->common->get_interface( MODULE( "inputsystem" ), "InputSystemVersion001" );
	scheme_manager        = ( i_scheme_manager* ) photon->common->get_interface( MODULE( "vgui2" ), "VGUI_Scheme010" );
	input_stack_system    = ( i_input_stack_system* ) photon->common->get_interface( MODULE( "inputsystem" ), "InputStackSystemVersion001" );

	const auto tier0 = util::get_module_handle( OS( "tier0.dll", "libtier0.so" ) );
	mem_alloc        = *util::get_sym_addr< i_mem_alloc** >( tier0, "g_pMemAlloc" );
	command_line     = util::get_sym_addr< i_command_line* ( * ) ( ) >( tier0, "CommandLine" )( );

	engine       = **reinterpret_cast< void*** >( util::get_virtual< 12 >( interfaces::engine_api ) + OS( 0x2, 0x4 ) );
	client_state = util::read< void* ( * ) ( ) >( util::get_virtual< 7 >( interfaces::engine_client ) + OS( 0x4, 0x9 ) )( );
	font_manager = util::read< c_font_manager* ( * ) ( ) >( util::get_virtual< 132 >( interfaces::surface ) + OS( 0x8, 0x9 ) )( );
	scheme       = interfaces::scheme_manager->get_i_scheme( 1 );

	LOG_PTR( mem_alloc );
	LOG_PTR( command_line );
	LOG_PTR( engine );
	LOG_PTR( client_state );
	LOG_PTR( font_manager );
	LOG_PTR( scheme );

	return true;
}

void interfaces::uninitialize( ) {
	SAFE_DELETE( console );
}
