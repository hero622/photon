#pragma once

#include "core/mods/mods.h"
#include "sdk/photon.h"

#include <string>

namespace gui {
	namespace framework {
		namespace colors {
			inline color_t bg     = color_t( 0, 0, 0, 200 );
			inline color_t black  = color_t( 0, 0, 0, 255 );
			inline color_t white  = color_t( 255, 255, 255, 255 );
			inline color_t dark   = color_t( 64, 64, 64, 255 );
			inline color_t darker = color_t( 32, 32, 32, 255 );
		}  // namespace colors

		namespace fonts {
			inline h_font normal;
			inline h_font title;
			inline h_font icon;
		}  // namespace fonts

		struct menu_t {
			vec2_t pos;
			vec2_t size;

			vec2_t cursor;

			std::size_t tab_count;
			std::size_t mod_count;

			bool block_input;
		};
		inline menu_t cur_menu;

		struct dropdown_t {
			std::string id;

			vec2_t pos;

			std::vector< std::string > items;
			std::size_t                value;  // this is a bitmask when the dropdown is a multiselect and just a regular index when it isnt

			bool multiselect;
			bool done;
		};
		inline dropdown_t cur_dropdown;

		void begin( vec2_t pos, vec2_t size );
		void end( );
		bool tab( int& selected, vec2_t pos, vec2_t size, const std::string& title );
		bool mod( mods::mod_info_t* info );

		bool button( vec2_t size, const std::string& label );
		bool checkbox( bool& val, const std::string& label );
		void slider( int& val, int min, int max, const std::string& label );
		void sliderf( float& val, float min, float max, const std::string& label );
		void combo( std::size_t& val, const std::vector< std::string >& items, const std::string& label );
		void multicombo( std::size_t& val, const std::vector< std::string >& items, const std::string& label );
	}  // namespace framework
}  // namespace gui
