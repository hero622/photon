#pragma once

#include "wormhole-sdk/hud.h"

#include <vector>

namespace huds {
	inline std::vector<wh_api::i_hud *> huds;
	inline std::vector<wh_api::i_thud *> thuds;

	inline sdk::vec2_t safezone = sdk::vec2_t(8, 8);

	void paint();
	void paint_ui();
}  // namespace huds