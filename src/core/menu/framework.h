#pragma once

#include "wormhole-sdk/wormhole.h"

#include <string>

namespace menu {
	namespace framework {
		struct menu_t {
		};
		inline menu_t cur_menu;

		void begin(int w, int h, std::string title);
		void end();
		void tab(std::string title);
		bool button(std::string title);
	}  // namespace framework
}  // namespace menu