#pragma once

#include "core/mods/mods.h"
#include "wormhole-sdk/wormhole.h"

#include <string>

namespace gui {
	namespace framework {
		namespace colors {
			inline sdk::color_t bg = sdk::color_t( 0, 0, 0, 200 );
			inline sdk::color_t black = sdk::color_t( 0, 0, 0, 255 );
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

			std::size_t tab_count;
			std::size_t mod_count;

			bool block_input;
		};
		inline menu_t cur_menu;

		struct dropdown_t {
			std::string id;

			sdk::vec2_t pos;

			std::vector<std::string> items;
			std::size_t value;  // this is a bitmask when the dropdown is a multiselect and just a regular index when it isnt

			bool multiselect;
			bool done;
		};
		inline dropdown_t cur_dropdown;

		void begin( sdk::vec2_t pos, sdk::vec2_t size );
		void end( );
		bool tab( int &selected, sdk::vec2_t pos, sdk::vec2_t size, std::string title );
		bool mod( mods::mod_info_t *info );

		bool button( sdk::vec2_t size, std::string label );
		void checkbox( bool &val, std::string label );
		void slider( int &val, int min, int max, std::string label );
		void sliderf( float &val, float min, float max, std::string label );
		void combo( std::size_t &val, std::vector<std::string> items, std::string label );
		void multicombo( std::size_t &val, std::vector<std::string> items, std::string label );
	}  // namespace framework
}  // namespace gui
