#include "gui.h"

#include "core/convars/convars.h"
#include "core/huds/huds.h"
#include "core/mods/mods.h"
#include "framework.h"
#include "sdk/photon.h"

bool gui::initialize( ) {
	photon->render->create_font( framework::fonts::normal, "Segoe UI Light", 22, false, fontflag_antialias );
	photon->render->create_font( framework::fonts::title, "Segoe UI Light", 30, false, fontflag_antialias );

	return true;
}

void gui::uninitialize( ) {
	photon->render->destruct_font( framework::fonts::title );
	photon->render->destruct_font( framework::fonts::normal );
}

void gui::paint( ) {
	const auto screen_size = photon->render->get_screen_size( );

	const int  tab_height = 50;
	const auto menu_size  = vec2_t( 700, 0.32f * screen_size.y );
	const auto menu_pos   = vec2_t( screen_size.x / 2 - menu_size.x / 2, screen_size.y / 2 + tab_height / 2 + 8 );

	static int                       tab = 1;
	static photon_api::i_photon_mod* cur_mod;

	framework::begin( menu_pos, menu_size );

	if ( framework::tab( tab, vec2_t( screen_size.x / 2 - 90 - 130 - 6, screen_size.y / 2 - tab_height / 2 ), vec2_t( 130, tab_height ), "profiles" ) ) {
	}
	if ( framework::tab( tab, vec2_t( screen_size.x / 2 - 90, screen_size.y / 2 - tab_height / 2 ), vec2_t( 180, tab_height ), "modules" ) ) {
		if ( !cur_mod ) {
			for ( auto& mod : mods::mod_list ) {
				if ( framework::mod( &mod.second ) )
					cur_mod = mod.second.ptr;
			}
		} else {
			if ( framework::button( vec2_t( 80, 30 ), "< back" ) )
				cur_mod = nullptr;

			if ( cur_mod )
				cur_mod->paint_menu( );
		}
	}
	if ( framework::tab( tab, vec2_t( screen_size.x / 2 + 90 + 6, screen_size.y / 2 - tab_height / 2 ), vec2_t( 130, tab_height ), "settings" ) ) {
		static bool fast_loads;
		if ( framework::checkbox( fast_loads, "fast loads" ) )
			convars::set_fast_loads( fast_loads );

		framework::slider( huds::safezone_x, 0, 32, "hud safezone x" );
		framework::slider( huds::safezone_y, 0, 32, "hud safezone y" );
	}

	framework::end( );
}
