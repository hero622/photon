#pragma once

#include "wormhole-sdk/hud.h"

#include <vector>

namespace huds {
	inline std::vector<wh_api::i_hud *> huds;
	inline std::vector<wh_api::i_thud *> thuds;

	void paint();

	void paint_ui();
}  // namespace huds