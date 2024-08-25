#include "mod.h"

#include "config/config.h"
#include "convars/convars.h"
#include "huds/hud.h"
#include "signals/signals.h"

c_photon_mod mod;

EXPOSE_PHOTON_MOD( c_photon_mod, mod );

photon_api::c_shared* photon;

h_font font;

bool c_photon_mod::load( photon_api::c_shared* photon ) {
	::photon = photon;  // expose photon interface globally.

	if ( !signals::initialize( ) )
		return false;

	if ( !convars::initialize( ) )
		return false;

	if ( !huds::initialize( ) )
		return false;

	if ( !config::initialize( ) )
		return false;

	photon->common->log( "example mod loaded.\n" );

	// post example event
	// this will look like: "example mod:load" for other mods
	photon->common->post_event( &mod, "load" );

	photon->render->create_font( font, "Tahoma", 12, false, fontflag_dropshadow );

	return true;
}

void c_photon_mod::unload( ) {
	photon->render->destruct_font( font );

	config::uninitialize( );
	huds::uninitialize( );
	convars::uninitialize( );
	signals::uninitialize( );

	photon->common->log( "example mod unloaded.\n" );
}

void c_photon_mod::on_event( const char* msg ) {
	if ( !strcmp( msg, "paint" ) ) {
		photon->render->draw_text( 2, 2, font, { 255, 255, 255, 255 }, false, "example mod loaded." );
	}
	if ( !strcmp( msg, "on_screen_size_changed" ) ) {
		photon->render->create_font( font, "Tahoma", 12, false, fontflag_dropshadow );
	}
}

photon_api::mod_info_t c_photon_mod::get_info( ) {
	photon_api::mod_info_t info;
	info.name    = "Example Mod";
	info.author  = "hero";
	info.version = "0.0.1";
	return info;
}

void c_photon_mod::paint_menu( ) {
	photon->menu->toggle( config::example_boolean, "example toggle" );
	photon->menu->slider( config::example_integer, 0, 100, "example slider" );
	photon->menu->sliderf( config::example_float, 0.f, 10.f, "example sliderf" );
	photon->menu->colorpicker( config::example_color, "example colorpicker" );

	photon->menu->separator( "example separator" );

	static const char* values[] = { "value 1", "value 2", "value 3" };
	photon->menu->combo( config::example_combo_value, values, 3, "example combo" );
	photon->menu->multicombo( config::example_mcombo_value, values, 3, "example multicombo" );
}
