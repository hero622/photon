#include "convars.h"

#include <wormhole.h>

concmd_cbk( example_command ) {
	wh->portal2->console->msg( "hello." );
}

bool convars::initialize( ) {
	wh->cvars->create_concmd( "example_command", example_command_cbk, "example command.\n" );

	return true;
}

void convars::uninitialize( ) {
}
