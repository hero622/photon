#include "hooks.h"

#include "core/huds/huds.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/gui.h"
#include "core/mods/mods.h"
#include "core/photon.h"
#include "util/util.h"

bool hooks::initialize( ) {
	HK_VIRTUAL( interfaces::server_game_dll, game_frame, 4 );
	HK_VIRTUAL( interfaces::engine, frame, OS( 5, 6 ) );
	HK_VIRTUAL( interfaces::client_state, set_signon_state, OS( 15, 36 ) );
	HK_VIRTUAL( interfaces::engine_vgui_internal, paint, OS( 14, 15 ) );
	HK_VIRTUAL( interfaces::surface, lock_cursor, 65 );
	HK_VIRTUAL( interfaces::base_client_dll, in_key_event, 20 );
	HK_VIRTUAL( interfaces::vgui_input, update_button_state, 87 );
	HK_VIRTUAL( interfaces::surface, on_screen_size_changed, 114 );

	HK_CMD( plugin_load );
	HK_CMD( plugin_unload );

	return true;
}

void hooks::uninitialize( ) {
	UNHK_CMD( plugin_unload );
	UNHK_CMD( plugin_load );

	UNHK( on_screen_size_changed );
	UNHK( update_button_state );
	UNHK( in_key_event );
	UNHK( lock_cursor );
	UNHK( paint );
	UNHK( set_signon_state );
	UNHK( frame );
	UNHK( game_frame );
}

HK_FN( void, hooks::game_frame, bool simulating ) {
	photon->event->post( &plugin, "pre_tick" );

	game_frame( ecx, simulating );

	photon->event->post( &plugin, "post_tick" );
}

HK_FN( void, hooks::frame ) {
	photon->event->post( &plugin, "pre_frame" );

	frame( ecx );

	// XXX: im not sure why we need this condition here, it used to be fine
	// possibly i changed something about unloading and this doesnt get unhooked early enough?
	// if there is more stuff being done here later, this might break, this should be looked into !!!
	if ( photon )
		photon->event->post( &plugin, "post_frame" );
}

HK_FN( void, hooks::set_signon_state, int state, int count, void* unk ) {
	set_signon_state( ecx, state, count, unk );

	// this is probably not the best way, i saw SAR do something similar but this needs further thought
	if ( state == signonstate_full )
		photon->event->post( &plugin, "session_start" );
	else
		photon->event->post( &plugin, "session_end" );
}

HK_FN( void, hooks::paint, paint_mode_t mode ) {
	paint( ecx, mode );

	interfaces::surface->start_drawing( );

	if ( mode == paint_uipanels ) {
		photon->input->poll_input( );  // not sure if this is the best place to call this

		huds::paint( );

		photon->event->post( &plugin, "paint" );

		if ( photon->input->get_key_press( key_insert ) )
			gui::open = !gui::open;

		if ( gui::open ) {
			gui::paint( );
			huds::paint_ui( );
		}
	}

	interfaces::surface->finish_drawing( );
}

// unlock the cursor from the game when menu is open
HK_FN( void, hooks::lock_cursor ) {
	static void* input_ctx = interfaces::engine_client->get_input_context( 0 );

	if ( gui::open ) {
		interfaces::surface->unlock_cursor( );

		interfaces::input_stack_system->set_cursor_visible( input_ctx, true );
	} else {
		interfaces::input_stack_system->set_cursor_visible( input_ctx, false );

		lock_cursor( ecx );
	}
}

// block input to the game when photon's menu is open, only works in game, not in the menu
HK_FN( int, hooks::in_key_event, int eventcode, button_code_t keynum, const char* current_binding ) {
	if ( gui::open )
		return 0;

	return in_key_event( ecx, eventcode, keynum, current_binding );
}

// block input to the menu, vgui has its own input system for some reason, so we have to hook another function
HK_FN( void, hooks::update_button_state, const int* event ) {
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

	update_button_state( ecx, event );
}

// recreate fonts because they get cleared everytime you change screen resolution
HK_FN( void, hooks::on_screen_size_changed, int old_width, int old_height ) {
	on_screen_size_changed( ecx, old_width, old_height );

	photon->event->post( &plugin, "on_screen_size_changed" );

	// recreate fonts
	gui::initialize( );
}

// prevent from loading the plugin twice (why doesnt source do this ???)
HK_CMD_FN( hooks::plugin_load ) {
	if ( args.arg_c( ) >= 2 && strstr( args[ 1 ], "photon" ) )
		interfaces::console->warning( "Photon is already loaded.\n" );
	else
		plugin_load( args );
}

// we need to unhook cengine::frame before the plugin gets unloaded
HK_CMD_FN( hooks::plugin_unload ) {
	if ( args.arg_c( ) >= 2 && plugin.get_info( ) && ( !strcmp( args[ 1 ], "photon" ) || std::atoi( args[ 1 ] ) == plugin.info->index ) )
		plugin.unload( );
	else
		plugin_unload( args );
}
