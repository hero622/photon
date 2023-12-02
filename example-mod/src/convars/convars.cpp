#include "convars.h"

#include <photon.h>

concmd_cbk( example_command ) {
	photon->portal2->console->msg( "hello." );
}

bool convars::initialize( ) {
	photon->con->create_concmd( "example_command", example_command_cbk, "example command.\n" );

	photon->con->create_convar( "example_convar", "0", fcvar_none, "example convar.\n", true, 0, true, 1, 0 );

	return true;
}

void convars::uninitialize( ) {
	photon->con->destruct_convar( "example_convar" );

	photon->con->destruct_concmd( "example_command" );
}
