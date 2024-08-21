#include "gui.h"

#include "core/convars/convars.h"
#include "core/huds/huds.h"
#include "core/interfaces/interfaces.h"
#include "core/mods/mods.h"
#include "core/photon.h"
#include "dx9.h"
#include "framework.h"

// unlock the cursor from the game when menu is open
SIGNAL_CALLBACK( void, __rescall, lock_cursor ) {
	static void* input_ctx = interfaces::engine_client->get_input_context( 0 );

	if ( gui::open ) {
		interfaces::surface->unlock_cursor( );

		interfaces::input_stack_system->set_cursor_visible( input_ctx, true );
	} else {
		interfaces::input_stack_system->set_cursor_visible( input_ctx, false );

		original( ecx );
	}
}

SIGNAL_CALLBACK( void, __rescall, paint, paint_mode_t, mode ) {
	original( ecx, mode );

	interfaces::surface->start_drawing( );

	if ( mode == paint_uipanels ) {
		photon->input->poll_input( );  // not sure if this is the best place to call this

		huds::paint( );

		photon->common->post_event( &plugin, "paint" );

		if ( photon->input->get_key_press( key_insert ) )
			gui::open = !gui::open;

		if ( gui::open ) {
			gui::paint( );
			huds::paint_ui( );
		}
	}

	interfaces::surface->finish_drawing( );
}

// block input to the game when photon's menu is open, only works in game, not in the menu
SIGNAL_CALLBACK( int, __rescall, in_key_event, int, eventcode, button_code_t, keynum, const char*, current_binding ) {
	if ( gui::open )
		return 0;

	return original( ecx, eventcode, keynum, current_binding );
}

// block input to the menu, vgui has its own input system for some reason, so we have to hook another function
SIGNAL_CALLBACK( void, __rescall, update_button_state, const int*, event ) {
	if ( gui::open ) {
		/*
		 * so we cant actually just return here because theres other
		 * functions calling SetKeyCodeState and SetMouseCodeState,
		 * i didnt want to hook those functions so we just reset
		 * the struct that those functions update here
		 */
		uint8_t* context_addr = ( uint8_t* ) ecx + 0xce8;  // m_hContext
		int      context      = *( int* ) context_addr;

		return util::call_virtual< 88, void >( ecx, context );  // ResetInputContext
	}

	original( ecx, event );
}

bool gui::initialize( ) {
	photon->signal->get( "lock_cursor" )->add_callback( &lock_cursor_cbk );
	photon->signal->get( "paint" )->add_callback( &paint_cbk );
	photon->signal->get( "in_key_event" )->add_callback( &in_key_event_cbk );
	photon->signal->get( "update_button_state" )->add_callback( &update_button_state_cbk );

	photon->render->create_font( framework::fonts::normal, "Segoe UI Light", 22, false, fontflag_antialias );
	photon->render->create_font( framework::fonts::title, "Segoe UI Light", 30, false, fontflag_antialias );

	dx9::initialize( );

	return true;
}

void gui::uninitialize( ) {
	dx9::uninitialize( );

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
		static bool fast_loads = true;
		if ( framework::checkbox( fast_loads, "fast loads" ) )
			convars::set_fast_loads( fast_loads );

		framework::slider( huds::safezone_x, 0, 32, "hud safezone x" );
		framework::slider( huds::safezone_y, 0, 32, "hud safezone y" );
	}

	framework::end( );
}
