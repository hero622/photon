#include "hooks.h"

#include "core/huds/huds.h"
#include "core/menu/gui.h"
#include "core/mods/mods.h"
#include "core/wormhole.h"

bool hooks::initialize( ) {
	hk_virtual( wh->portal2->server_game_dll, game_frame, 4 );
	hk_virtual( wh->portal2->engine, frame, os( 5, 6 ) );
	hk_virtual( wh->portal2->client_state, set_signon_state, os( 15, 36 ) );
	hk_virtual( wh->portal2->engine_vgui_internal, paint, os( 14, 15 ) );
	hk_virtual( wh->portal2->surface, lock_cursor, 65 );
	hk_virtual( wh->portal2->base_client_dll, in_key_event, 20 );
	hk_virtual( wh->portal2->vgui_input, update_button_state, 87 );
	hk_virtual( wh->portal2->surface, on_screen_size_changed, 114 );

	hk_cmd( plugin_unload );

	return true;
}

void hooks::uninitialize( ) {
	unhk_cmd( plugin_unload );

	unhk( on_screen_size_changed );
	unhk( update_button_state );
	unhk( in_key_event );
	unhk( lock_cursor );
	unhk( paint );
	unhk( set_signon_state );
	unhk( frame );
	unhk( game_frame );
}

hk_fn( void, hooks::game_frame, bool simulating ) {
	wh->events->post( &wormhole, "pre_tick" );

	game_frame( ecx, simulating );

	wh->events->post( &wormhole, "post_tick" );
}

hk_fn( void, hooks::frame ) {
	wh->events->post( &wormhole, "pre_frame" );

	frame( ecx );

	// todo: look into why this broke
	if ( wh )
		wh->events->post( &wormhole, "post_frame" );
}

hk_fn( void, hooks::set_signon_state, int state, int count, void *unk ) {
	set_signon_state( ecx, state, count, unk );

	// this is probably not the best way, i saw SAR do something similar but this needs further thought
	if ( state == sdk::signonstate_full )
		wh->events->post( &wormhole, "session_start" );
	else
		wh->events->post( &wormhole, "session_end" );
}

hk_fn( void, hooks::paint, sdk::paint_mode_t mode ) {
	paint( ecx, mode );

	wh->portal2->surface->start_drawing( );

	if ( mode == sdk::paint_uipanels ) {
		wh->input->poll_input( );  // not sure if this is the best place to call this

		huds::paint( );

		wh->events->post( &wormhole, "paint" );

		if ( wh->input->get_key_press( sdk::key_insert ) )
			gui::open = !gui::open;

		if ( gui::open ) {
			gui::paint( );
			huds::paint_ui( );
		}
	}

	wh->portal2->surface->finish_drawing( );
}

// unlock the cursor from the game when menu is open
hk_fn( void, hooks::lock_cursor ) {
	static void *input_ctx = wh->portal2->engine_client->get_input_context( 0 );

	if ( gui::open ) {
		wh->portal2->surface->unlock_cursor( );

		wh->portal2->input_stack_system->set_cursor_visible( input_ctx, true );
	} else {
		wh->portal2->input_stack_system->set_cursor_visible( input_ctx, false );

		lock_cursor( ecx );
	}
}

// block input to the game when wormhole's menu is open, only works in game, not in the menu
hk_fn( int, hooks::in_key_event, int eventcode, sdk::button_code_t keynum, const char *current_binding ) {
	if ( gui::open )
		return 0;

	return in_key_event( ecx, eventcode, keynum, current_binding );
}

// block input to the menu, vgui has its own input system for some reason, so we have to hook another function
hk_fn( void, hooks::update_button_state, const int *event ) {
	if ( gui::open ) {
		// so we cant actually just return here because theres other functions calling SetKeyCodeState and SetMouseCodeState
		// i didnt want to hook those functions so we just reset the struct that those functions update here
		uint8_t *context_addr = ( uint8_t * ) ecx + 0xce8;  // m_hContext
		int context = *( int * ) context_addr;

		return utils::memory::call_virtual<88, void>( ecx, context );  // ResetInputContext
	}

	update_button_state( ecx, event );
}

// recreate fonts because they get cleared everytime you change screen resolution
hk_fn( void, hooks::on_screen_size_changed, int old_width, int old_height ) {
	on_screen_size_changed( ecx, old_width, old_height );

	wh->events->post( &wormhole, "on_screen_size_changed" );

	// recreate fonts
	gui::initialize( );
}

// we need to unhook cengine::frame before the plugin gets unloaded
hk_cmd_fn( hooks::plugin_unload ) {
	if ( args.arg_c( ) >= 2 && wormhole.get_plugin( ) && ( !strcmp( args[ 1 ], "wormhole" ) || std::atoi( args[ 1 ] ) == wormhole.plugin->index ) )
		wormhole.unload( );
	else
		plugin_unload( args );
}
