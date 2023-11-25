#include "convars.h"

#include <wormhole.h>

concmd_cbk( example_command ) {
	wh->portal2->console->msg( "hello." );
}

bool convars::initialize( ) {
	wh->con->create_concmd( "example_command", example_command_cbk, "example command.\n" );

	wh->con->create_convar( "example_convar", "0", fcvar_none, "example convar.\n", true, 0, true, 1, 0 );

	return true;
}

void convars::uninitialize( ) {
	wh->con->destruct_convar( "example_convar" );

	wh->con->destruct_concmd( "example_command" );
}
