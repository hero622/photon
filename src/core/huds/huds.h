#pragma once

#include "sdk/hud.h"

#include <vector>

namespace huds {
	inline std::vector< photon_api::i_hud* > huds;

	inline int safezone_x = 8;
	inline int safezone_y = 8;

	inline bool edit = false;

	void draw( );
	void draw_ui( );
}  // namespace huds
