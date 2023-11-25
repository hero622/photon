#include "convars.h"

#include <wormhole.h>

concmd_cbk( example_command ) {
	wh->portal2->console->msg( "hello." );
}

bool convars::initialize( ) {
	wh->cvars->create_concmd( "example_command", example_command_cbk, "example command.\n" );

	wh->cvars->create_convar( "example_convar", "0", fcvar_none, "example convar.\n", true, 0, true, 1, 0 );

	return true;
}

void convars::uninitialize( ) {
	wh->cvars->destruct_convar( "example_convar" );

	wh->cvars->destruct_concmd( "example_command" );
}
