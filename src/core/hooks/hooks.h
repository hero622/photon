#pragma once

#include "wormhole-sdk/hook.h"

namespace hooks {
	decl_hk(void, game_frame, bool simulating);
	decl_hk(void, frame);
	decl_hk(void, set_signon_state, int state, int count, void *unk);
	decl_hk(void, paint, sdk::paint_mode_t mode);

	decl_hk_cmd(plugin_unload);

	bool initialize();
	void uninitialize();
}  // namespace hooks