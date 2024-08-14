#pragma once

#include "sdk/hook.h"
#include "util/util.h"

namespace hooks {
	DECL_HK( void, game_frame, bool simulating );
	DECL_HK( void, frame );
	DECL_HK( void, set_signon_state, int state, int count, void* unk );
	DECL_HK( void, paint, paint_mode_t mode );
	DECL_HK( void, lock_cursor );
	DECL_HK( int, in_key_event, int eventcode, button_code_t keynum, const char* current_binding );
	DECL_HK( void, update_button_state, const int* event );
	DECL_HK( void, on_screen_size_changed, int old_width, int old_height );

	DECL_HK_CMD( plugin_load );
	DECL_HK_CMD( plugin_unload );

	bool initialize( );
	void uninitialize( );
}  // namespace hooks
