#include "signals.h"

#include "core/huds/huds.h"
#include "core/interfaces/interfaces.h"
#include "core/menu/gui.h"
#include "core/mods/mods.h"
#include "core/photon.h"

SIGNAL( int, game_frame, bool, simulating );
SIGNAL( int, frame );
SIGNAL( int, set_signon_state, int, state, int, count, void*, unk );
SIGNAL( int, paint, paint_mode_t, mode );
SIGNAL( int, lock_cursor );
SIGNAL( int, in_key_event, int, eventcode, button_code_t, keyname, const char*, current_binding );
SIGNAL( int, update_button_state, const int*, event );
SIGNAL( int, on_screen_size_changed, int, old_width, int, old_height );

SIGNAL_CALLBACK( void, game_frame, bool, simulating ) {
	photon->common->post_event( &plugin, "pre_tick" );

	original( ecx, simulating );

	photon->common->post_event( &plugin, "post_tick" );
}

SIGNAL_CALLBACK( void, frame ) {
	photon->common->post_event( &plugin, "pre_frame" );

	original( ecx );

	// XXX: im not sure why we need this condition here, it used to be fine
	// possibly i changed something about unloading and this doesnt get unhooked early enough?
	// if there is more stuff being done here later, this might break, this should be looked into !!!
	if ( photon )
		photon->common->post_event( &plugin, "post_frame" );
}

SIGNAL_CALLBACK( void, set_signon_state, int, state, int, count, void*, unk ) {
	original( ecx, state, count, unk );

	// this is probably not the best way, i saw SAR do something similar but this needs further thought
	if ( state == signonstate_full )
		photon->common->post_event( &plugin, "session_start" );
	else
		photon->common->post_event( &plugin, "session_end" );
}

SIGNAL_CALLBACK( void, on_screen_size_changed, int, old_width, int, old_height ) {
	original( ecx, old_width, old_height );

	photon->common->post_event( &plugin, "on_screen_size_changed" );

	// recreate fonts
	gui::initialize( );
}

bool signals::initialize( ) {
	photon->signal->create( "game_frame" )->in_module( MODULE( "server" ) )->in_interface( "ServerGameDLL005" )->from_vtable( 4 )->enable( &game_frame_handler );
	photon->signal->create( "frame" )->at_address( interfaces::engine )->from_vtable( OS( 5, 6 ) )->enable( &frame_handler );
	photon->signal->create( "set_signon_state" )->at_address( interfaces::client_state )->from_vtable( OS( 15, 36 ) )->enable( &set_signon_state_handler );
	photon->signal->create( "paint" )->in_module( MODULE( "engine" ) )->in_interface( "VEngineVGui001" )->from_vtable( OS( 14, 15 ) )->enable( &paint_handler );
	photon->signal->create( "lock_cursor" )->at_address( interfaces::surface )->from_vtable( 65 )->enable( &lock_cursor_handler );
	photon->signal->create( "in_key_event" )->in_module( MODULE( "client" ) )->in_interface( "VClient016" )->from_vtable( 20 )->enable( &in_key_event_handler );
	photon->signal->create( "update_button_state" )->in_module( MODULE( "vgui2" ) )->in_interface( "VGUI_Input005" )->from_vtable( 87 )->enable( &update_button_state_handler );
	photon->signal->create( "on_screen_size_changed" )->at_address( interfaces::surface )->from_vtable( 114 )->enable( &on_screen_size_changed_handler );

	photon->signal->get( "game_frame" )->add_callback( &game_frame_cbk );
	photon->signal->get( "frame" )->add_callback( &frame_cbk );
	photon->signal->get( "set_signon_state" )->add_callback( &set_signon_state_cbk );
	photon->signal->get( "on_screen_size_changed" )->add_callback( &on_screen_size_changed_cbk );

	// HK_CMD( plugin_load );
	// HK_CMD( plugin_unload );

	return true;
}

void signals::uninitialize( ) {
	// UNHK_CMD( plugin_unload );
	// UNHK_CMD( plugin_load );

	photon->signal->get( "on_screen_size_changed" )->remove_callback( &on_screen_size_changed_cbk );
	photon->signal->get( "set_signon_state" )->remove_callback( &set_signon_state_cbk );
	photon->signal->get( "frame" )->remove_callback( &frame_cbk );
	photon->signal->get( "game_frame" )->remove_callback( &game_frame_cbk );

	photon->signal->remove( "on_screen_size_changed" );
	photon->signal->remove( "update_button_state" );
	photon->signal->remove( "in_key_event" );
	photon->signal->remove( "lock_cursor" );
	photon->signal->remove( "paint" );
	photon->signal->remove( "set_signon_state" );
	photon->signal->remove( "frame" );
	photon->signal->remove( "game_frame" );
}

// prevent from loading the plugin twice (why doesnt source do this ???)
// HK_CMD_FN( hooks::plugin_load ) {
// 	if ( args.arg_c( ) >= 2 && strstr( args[ 1 ], "photon" ) )
// 		photon->common->log_warn( "Photon is already loaded.\n" );
// 	else
// 		plugin_load( args );
// }

// // we need to unhook cengine::frame before the plugin gets unloaded
// HK_CMD_FN( hooks::plugin_unload ) {
// 	if ( args.arg_c( ) >= 2 && plugin.get_info( ) && ( !strcmp( args[ 1 ], "photon" ) || std::atoi( args[ 1 ] ) == plugin.info->index ) )
// 		plugin.unload( );
// 	else
// 		plugin_unload( args );
// }
