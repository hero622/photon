#include "hook.h"
#include "memory.h"

/*
 * NOTE: NONE OF THIS IS TESTED ON LINUX, MIGHT NOT EVEN WORK AT ALL
 * ALSO HAVENT EVEN MADE MAKEFILE YET
 */

#pragma optimize( "", off )

UTEST_STATE( );

int main( int argc, const char* const argv[] ) {
	// setup photon environment
	photon         = new photon_api::c_shared( );
	photon->common = new c_common( );
	photon->con    = new c_con( );
	photon->signal = new c_signal( );
	photon->hud    = new c_hud( );
	photon->render = new c_render( );
	photon->input  = new c_input( );
	photon->menu   = new c_menu( );

	return utest_main( argc, argv );
}

#pragma optimize( "", on )
