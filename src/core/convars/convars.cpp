#include "convars.h"

#include "core/mods/mods.h"
#include "photon-sdk/photon.h"

static sdk::con_var *ui_loadingscreen_transition_time;
static sdk::con_var *ui_loadingscreen_fadein_time;
static sdk::con_var *ui_loadingscreen_mintransition_time;

void convars::set_fast_loads( bool enable ) {
	// TODO: do the stuff that SAR does to make loads even faster
	ui_loadingscreen_transition_time->set_value( !enable );
	ui_loadingscreen_fadein_time->set_value( !enable );
	ui_loadingscreen_mintransition_time->set_value( enable ? 0.0f : 0.5f );
}

concmd_cbk( photon_load ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::load( args[ 1 ] );
	} else {
		photon->portal2->console->msg( "photon_load <mod name> - load a photon mod.\n" );
	}
}

concmd_cbk( photon_unload ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::unload( args[ 1 ] );
	} else {
		photon->portal2->console->msg( "photon_unload <mod name> - unload a photon mod.\n" );
	}
}

concmd_cbk( photon_enable ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::enable( &mods::mod_list[ args[ 1 ] ] );
	} else {
		photon->portal2->console->msg( "photon_enable <mod name> - enable a photon mod.\n" );
	}
}

concmd_cbk( photon_disable ) {
	if ( args.arg_c( ) >= 2 ) {
		mods::disable( &mods::mod_list[ args[ 1 ] ] );
	} else {
		photon->portal2->console->msg( "photon_disable <mod name> - disable a photon mod.\n" );
	}
}

concmd_cbk( photon_print ) {
	mods::print( );
}

bool convars::initialize( ) {
	photon->con->create_concmd( "photon_load", photon_load_cbk, "photon_load <mod name> - load a photon mod.\n" );
	photon->con->create_concmd( "photon_unload", photon_unload_cbk, "photon_unload <mod name> - unload a photon mod.\n" );
	photon->con->create_concmd( "photon_enable", photon_enable_cbk, "photon_enable <mod name> - enable a photon mod.\n" );
	photon->con->create_concmd( "photon_disable", photon_disable_cbk, "photon_disable <mod name> - disable a photon mod.\n" );
	photon->con->create_concmd( "photon_print", photon_print_cbk, "prints a list of all loaded photon mods.\n" );

	// fast loads
	ui_loadingscreen_transition_time = photon->portal2->cvar->find_con_var( "ui_loadingscreen_transition_time" );
	ui_loadingscreen_fadein_time = photon->portal2->cvar->find_con_var( "ui_loadingscreen_fadein_time" );
	ui_loadingscreen_mintransition_time = photon->portal2->cvar->find_con_var( "ui_loadingscreen_mintransition_time" );

	ui_loadingscreen_transition_time->remove_flag( fcvar_developmentonly | fcvar_hidden );
	ui_loadingscreen_fadein_time->remove_flag( fcvar_developmentonly | fcvar_hidden );
	ui_loadingscreen_mintransition_time->remove_flag( fcvar_developmentonly | fcvar_hidden );

	return true;
}

void convars::uninitialize( ) {
	ui_loadingscreen_transition_time->add_flag( fcvar_developmentonly | fcvar_hidden );
	ui_loadingscreen_fadein_time->add_flag( fcvar_developmentonly | fcvar_hidden );
	ui_loadingscreen_mintransition_time->add_flag( fcvar_developmentonly | fcvar_hidden );

	photon->con->destruct_concmd( "photon_print" );
	photon->con->destruct_concmd( "photon_disable" );
	photon->con->destruct_concmd( "photon_enable" );
	photon->con->destruct_concmd( "photon_unload" );
	photon->con->destruct_concmd( "photon_load" );
}
