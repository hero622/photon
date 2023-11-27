#include "convars.h"

#include "core/mods/mods.h"
#include "wormhole-sdk/wormhole.h"

concmd_cbk( wh_load ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::load( args[ 1 ] );
	} else {
		wh->portal2->console->msg( "wh_load <mod name> - load a wormhole mod.\n" );
	}
}

concmd_cbk( wh_unload ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::unload( args[ 1 ] );
	} else {
		wh->portal2->console->msg( "wh_unload <mod name> - unload a wormhole mod.\n" );
	}
}

concmd_cbk( wh_enable ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::enable( &mods::mod_list[ args[ 1 ] ] );
	} else {
		wh->portal2->console->msg( "wh_enable <mod name> - enable a wormhole mod.\n" );
	}
}

concmd_cbk( wh_disable ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::disable( &mods::mod_list[ args[ 1 ] ] );
	} else {
		wh->portal2->console->msg( "wh_disable <mod name> - disable a wormhole mod.\n" );
	}
}

concmd_cbk( wh_print ) {
	mods::print( );
}

bool convars::initialize( ) {
	wh->con->create_concmd( "wh_load", wh_load_cbk, "wh_load <mod name> - load a wormhole mod.\n" );
	wh->con->create_concmd( "wh_unload", wh_unload_cbk, "wh_unload <mod name> - unload a wormhole mod.\n" );
	wh->con->create_concmd( "wh_enable", wh_enable_cbk, "wh_enable <mod name> - enable a wormhole mod.\n" );
	wh->con->create_concmd( "wh_disable", wh_disable_cbk, "wh_disable <mod name> - disable a wormhole mod.\n" );
	wh->con->create_concmd( "wh_print", wh_print_cbk, "prints a list of all loaded wormhole mods.\n" );

	return true;
}

void convars::uninitialize( ) {
	wh->con->destruct_concmd( "wh_print" );
	wh->con->destruct_concmd( "wh_disable" );
	wh->con->destruct_concmd( "wh_enable" );
	wh->con->destruct_concmd( "wh_unload" );
	wh->con->destruct_concmd( "wh_load" );
}
