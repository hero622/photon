#include "mod.h"

#include "convars/convars.h"
#include "hooks/hooks.h"
#include "huds/hud.h"

c_photon_mod mod;

expose_photon_mod( c_photon_mod, mod );

photon_api::c_shared *photon;

sdk::h_font font;

bool c_photon_mod::load( photon_api::c_shared *photon ) {
	::photon = photon;

	if ( example_mod::hooks::initialize( ) ) {
		convars::initialize( );

		huds::initialize( );

		photon->portal2->console->msg( "example mod loaded.\n" );

		// post example event
		// this will look like: "example mod:load" for other mods
		photon->event->post( &mod, "load" );

		photon->render->create_font( font, "Tahoma", 12, false, sdk::fontflag_dropshadow );
	}

	return true;
}

void c_photon_mod::unload( ) {
	photon->render->destruct_font( font );

	huds::uninitialize( );

	convars::uninitialize( );

	example_mod::hooks::uninitialize( );

	photon->portal2->console->msg( "example mod unloaded.\n" );
}

void c_photon_mod::on_event( const char *msg ) {
	if ( !strcmp( msg, "paint" ) ) {
		photon->render->draw_text( 2, 2, font, { 255, 255, 255, 255 }, false, "example mod loaded." );
	}
	if ( !strcmp( msg, "on_screen_size_changed" ) ) {
		photon->render->create_font( font, "Tahoma", 12, false, sdk::fontflag_dropshadow );
	}
}

photon_api::mod_info_t c_photon_mod::get_info( ) {
	photon_api::mod_info_t info;
	info.name = "example mod";
	info.version = "0.0.1";
	return info;
}

void c_photon_mod::paint_menu( ) {
	static bool example_checkbox_val;
	photon->menu->checkbox( example_checkbox_val, "example checkbox" );

	static int example_slider_val;
	photon->menu->slider( example_slider_val, 0, 100, "example slider" );

	static float example_sliderf_val;
	photon->menu->sliderf( example_sliderf_val, 0.f, 10.f, "example sliderf" );

	static std::size_t example_combo_val;
	const char *example_combo_items[] = { "value 1", "value 2", "value 3" };
	photon->menu->combo( example_combo_val, example_combo_items, array_len( example_combo_items ), "example combo" );

	static std::size_t example_multicombo_val;
	const char *example_multicombo_items[] = { "value 1", "value 2", "value 3" };
	photon->menu->multicombo( example_multicombo_val, example_multicombo_items, array_len( example_multicombo_items ), "example multicombo" );
}
