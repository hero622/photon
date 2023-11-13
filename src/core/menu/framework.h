#pragma once

#include "wormhole-sdk/wormhole.h"

#include <string>

namespace gui {
	namespace framework {
		namespace colors {
			inline sdk::color_t bg = sdk::color_t( 0, 0, 0, 200 );
			inline sdk::color_t white = sdk::color_t( 255, 255, 255, 255 );
			inline sdk::color_t dark = sdk::color_t( 64, 64, 64, 255 );
			inline sdk::color_t darker = sdk::color_t( 32, 32, 32, 255 );
		}  // namespace colors

		namespace fonts {
			inline sdk::h_font normal;
			inline sdk::h_font title;
			inline sdk::h_font icon;
		}  // namespace fonts

		struct menu_t {
			sdk::vec2_t pos;
			sdk::vec2_t size;

			sdk::vec2_t cursor;

			int tab_count;
			int mod_count;
		};
		inline menu_t cur_menu;

		void begin( sdk::vec2_t pos, sdk::vec2_t size );
		void end( );
		bool tab( int &selected, sdk::vec2_t pos, sdk::vec2_t size, std::string title );
		bool mod( std::string title, std::string subtitle );
		void modlist( std::vector<std::string> items );

		bool button( sdk::vec2_t size, std::string label );
		void checkbox( bool &val, std::string label );
	}  // namespace framework
}  // namespace gui
