#include "hook.h"
#include "memory.h"
#include "photon-sdk/photon.h"
#include "utest.h"

/*
 * NOTE: NONE OF THIS IS TESTED ON LINUX, MIGHT NOT EVEN WORK AT ALL
 * ALSO HAVENT EVEN MADE MAKEFILE YET
 */

#pragma optimize( "", off )

UTEST_STATE( );

int main( int argc, const char *const argv[] ) {
	// setup photon environment
	photon = new photon_api::c_shared( );
	photon->portal2 = new c_portal2( );
	photon->con = new c_con( );
	photon->hook = new c_hook( );
	photon->event = new c_event( );
	photon->hud = new c_hud( );
	photon->render = new c_render( );
	photon->input = new c_input( );
	photon->menu = new c_menu( );

	return utest_main( argc, argv );
}

#pragma optimize( "", on )
