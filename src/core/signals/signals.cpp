#include "signals.h"

#include "core/huds/huds.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/gui.h"
#include "core/photon.h"

SIGNAL( int, __rescall, game_frame, bool, simulating );
SIGNAL( int, __rescall, frame );
SIGNAL( int, __rescall, set_signon_state, int, state, int, count, void*, unk );
SIGNAL( int, __rescall, paint, paint_mode_t, mode );
SIGNAL( int, __rescall, lock_cursor );
SIGNAL( int, __rescall, in_key_event, int, eventcode, button_code_t, keyname, const char*, current_binding );
SIGNAL( int, __rescall, update_button_state, const int*, event );
SIGNAL( int, __rescall, on_screen_size_changed, int, old_width, int, old_height );

SIGNAL_CALLBACK( void, __rescall, game_frame, bool, simulating ) {
	photon->common->post_event( &plugin, "pre_tick" );

	original( ecx, simulating );

	photon->common->post_event( &plugin, "post_tick" );
}

SIGNAL_CALLBACK( void, __rescall, frame ) {
	photon->common->post_event( &plugin, "pre_frame" );

	original( ecx );

	// XXX: im not sure why we need this condition here, it used to be fine
	// possibly i changed something about unloading and this doesnt get unhooked early enough?
	// if there is more stuff being done here later, this might break, this should be looked into !!!
	if ( photon )
		photon->common->post_event( &plugin, "post_frame" );
}

SIGNAL_CALLBACK( void, __rescall, set_signon_state, int, state, int, count, void*, unk ) {
	original( ecx, state, count, unk );

	// this is probably not the best way, i saw SAR do something similar but this needs further thought
	if ( state == signonstate_full )
		photon->common->post_event( &plugin, "session_start" );
	else
		photon->common->post_event( &plugin, "session_end" );
}

SIGNAL_CALLBACK( void, __rescall, on_screen_size_changed, int, old_width, int, old_height ) {
	original( ecx, old_width, old_height );

	photon->common->post_event( &plugin, "on_screen_size_changed" );

	// recreate fonts
	gui::create_fonts( );
}

bool signals::initialize( ) {
	photon->signal->create( "game_frame" )->in_module( MODULE( "server" ) )->in_interface( "ServerGameDLL005" )->from_vtable( 4 )->enable( &game_frame_handler );
	photon->signal->create( "frame" )->at_address( interfaces::engine )->from_vtable( OS( 5, 6 ) )->enable( &frame_handler );
	photon->signal->create( "set_signon_state" )->at_address( interfaces::client_state )->from_vtable( OS( 15, 36 ) )->enable( &set_signon_state_handler );
	photon->signal->create( "paint" )->in_module( MODULE( "engine" ) )->in_interface( "VEngineVGui001" )->from_vtable( OS( 14, 15 ) )->enable( &paint_handler );
	photon->signal->create( "lock_cursor" )->at_address( interfaces::surface )->from_vtable( 65 )->enable( &lock_cursor_handler );
	photon->signal->create( "in_key_event" )->in_module( MODULE( "client" ) )->in_interface( "VClient016" )->from_vtable( 20 )->enable( &in_key_event_handler );
	photon->signal->create( "update_button_state" )->in_module( MODULE( "vgui2" ) )->in_interface( "VGUI_Input005" )->from_vtable( OS( 87, 88 ) )->enable( &update_button_state_handler );
	photon->signal->create( "on_screen_size_changed" )->at_address( interfaces::surface )->from_vtable( 114 )->enable( &on_screen_size_changed_handler );

	photon->signal->get( "game_frame" )->add_callback( &game_frame_cbk );
	photon->signal->get( "frame" )->add_callback( &frame_cbk );
	photon->signal->get( "set_signon_state" )->add_callback( &set_signon_state_cbk );
	photon->signal->get( "on_screen_size_changed" )->add_callback( &on_screen_size_changed_cbk );

	return true;
}

void signals::uninitialize( ) {
	photon->signal->remove_all( );
}
