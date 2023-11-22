#pragma once

#include "wormhole-sdk/hud.h"

#include <vector>

namespace huds {
	inline std::vector<wh_api::i_hud *> huds;
	inline std::vector<wh_api::i_thud *> thuds;

	inline int safezone_x = 8;
	inline int safezone_y = 8;

	void paint( );
	void paint_ui( );
}  // namespace huds
