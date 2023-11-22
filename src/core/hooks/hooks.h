#pragma once

#include "wormhole-sdk/hook.h"

namespace hooks {
	decl_hk( void, game_frame, bool simulating );
	decl_hk( void, frame );
	decl_hk( void, set_signon_state, int state, int count, void *unk );
	decl_hk( void, paint, sdk::paint_mode_t mode );
	decl_hk( void, lock_cursor );
	decl_hk( int, in_key_event, int eventcode, sdk::button_code_t keynum, const char *current_binding );
	decl_hk( void, update_button_state, const int *event );
	decl_hk( void, on_screen_size_changed, int old_width, int old_height );

	decl_hk_cmd( plugin_unload );

	bool initialize( );
	void uninitialize( );
}  // namespace hooks
