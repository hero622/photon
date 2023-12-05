#include "convars.h"

#include <photon.h>

concmd_cbk( example_command ) {
	// get multiple types of values of the cvar
	const auto cvar = photon->portal2->cvar->find_con_var( "example_convar" );

	const auto fl_value = cvar->get_float( );
	const auto n_value = cvar->get_int( );
	const auto col_value = cvar->get_color( );
	const auto b_value = cvar->get_bool( );
	const auto str_value = cvar->get_string( );

	photon->portal2->console->msg( "the value of example_convar is:\n" );
	photon->portal2->console->msg( "float: %f\n", fl_value );
	photon->portal2->console->msg( "int: %d\n", n_value );
	photon->portal2->console->msg( "color: %d %d %d %d\n", col_value.r, col_value.g, col_value.b, col_value.a );
	photon->portal2->console->msg( "bool: %d\n", ( int ) b_value );
	photon->portal2->console->msg( "string: %s\n", str_value );
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
