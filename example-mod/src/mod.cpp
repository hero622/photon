#include "mod.h"

#include "convars/convars.h"
#include "huds/hud.h"
#include "signals/signals.h"

c_photon_mod mod;

EXPOSE_PHOTON_MOD( c_photon_mod, mod );

photon_api::c_shared* photon;

h_font font;

bool c_photon_mod::load( photon_api::c_shared* photon ) {
	::photon = photon;

	if ( signals::initialize( ) ) {
		convars::initialize( );

		huds::initialize( );

		photon->common->log( "example mod loaded.\n" );

		// post example event
		// this will look like: "example mod:load" for other mods
		photon->common->post_event( &mod, "load" );

		photon->render->create_font( font, "Tahoma", 12, false, fontflag_dropshadow );
	}

	return true;
}

void c_photon_mod::unload( ) {
	photon->render->destruct_font( font );

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
	static bool example_toggle_val;
	photon->menu->toggle( example_toggle_val, "example toggle" );

	static int example_slider_val;
	photon->menu->slider( example_slider_val, 0, 100, "example slider" );

	static float example_sliderf_val;
	photon->menu->sliderf( example_sliderf_val, 0.f, 10.f, "example sliderf" );

	static color_t example_colorpicker_val;
	photon->menu->colorpicker( example_colorpicker_val, "example colorpicker" );

	photon->menu->separator( "example separator" );

	static std::size_t example_combo_val;
	const char*        example_combo_items[] = { "value 1", "value 2", "value 3" };
	photon->menu->combo( example_combo_val, example_combo_items, ARRAY_LEN( example_combo_items ), "example combo" );

	static std::size_t example_multicombo_val;
	const char*        example_multicombo_items[] = { "value 1", "value 2", "value 3" };
	photon->menu->multicombo( example_multicombo_val, example_multicombo_items, ARRAY_LEN( example_multicombo_items ), "example multicombo" );
}
