#include "framework.h"

#include "core/mods/mods.h"

#include <algorithm>

void gui::framework::begin( sdk::vec2_t pos, sdk::vec2_t size ) {
	cur_menu = menu_t( );

	cur_menu.pos = pos;
	cur_menu.size = size;

	cur_menu.cursor = sdk::vec2_t( 12, 12 );

	// block input to other controls when a dropdown is open
	cur_menu.block_input = !cur_dropdown.id.empty( );

	wh->render->draw_outlined_rect( cur_menu.pos.x, cur_menu.pos.y, cur_menu.size.x, cur_menu.size.y, colors::dark );
	wh->render->draw_filled_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.size.x - 2, cur_menu.size.y - 2, colors::bg );

	wh->portal2->surface->enable_clipping = true;
	wh->portal2->surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );
}

void gui::framework::end( ) {
	// draw dropdowns
	{
		auto cur_pos = cur_menu.pos + cur_dropdown.pos;

		const auto size = sdk::vec2_t( 160, 26 );

		wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, cur_dropdown.items.size( ) * size.y, colors::dark );
		wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, cur_dropdown.items.size( ) * size.y - 2, colors::black );

		for ( std::size_t i = 0; i < cur_dropdown.items.size( ); ++i ) {
			const auto &item = cur_dropdown.items[ i ];

			bool hover = wh->input->is_cursor_in_area( cur_pos.x + 1, cur_pos.y + 1, cur_pos.x + size.x - 1, cur_pos.y + size.y - 1 );
			bool clicking = hover && wh->input->get_key_press( sdk::mouse_left );

			if ( !cur_dropdown.multiselect ) {
				if ( clicking ) {
					cur_dropdown.value = i;
					cur_dropdown.done = true;
				}

				wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, hover ? colors::white : colors::dark, false, item.c_str( ) );
			} else {
				if ( clicking )
					cur_dropdown.value ^= ( 1 << i );

				wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, cur_dropdown.value & ( 1 << i ) ? colors::white : colors::dark, false, item.c_str( ) );
			}

			cur_pos.y += 26;
		}
	}

	wh->portal2->surface->set_clip_rect( 0, 0, wh->render->get_screen_size( ).x, wh->render->get_screen_size( ).y );
	wh->portal2->surface->enable_clipping = false;
}

bool gui::framework::tab( int &selected, sdk::vec2_t pos, sdk::vec2_t size, std::string title ) {
	bool hover = wh->input->is_cursor_in_area( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
	bool active = ( hover && wh->input->get_key_press( sdk::mouse_left ) ) || selected == cur_menu.tab_count;
	if ( active )
		selected = cur_menu.tab_count;

	wh->portal2->surface->set_clip_rect( 0, 0, wh->render->get_screen_size( ).x, wh->render->get_screen_size( ).y );

	wh->render->draw_outlined_rect( pos.x, pos.y, size.x, size.y, active ? colors::white : hover ? colors::dark
	                                                                                             : colors::darker );
	wh->render->draw_filled_rect( pos.x + 1, pos.y + 1, size.x - 2, size.y - 2, colors::bg );

	const auto text_size = wh->render->get_text_size( fonts::title, title.c_str( ) );

	wh->render->draw_text( pos.x + size.x / 2, pos.y + size.y / 2 - text_size.y / 2, fonts::title, colors::white, true, title.c_str( ) );

	wh->portal2->surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );

	++cur_menu.tab_count;

	return active;
}

bool gui::framework::mod( mods::mod_info_t *info ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 220, 128 );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::dark : colors::darker );
	wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg );

	const auto mod_info = info->ptr->get_info( );

	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 8, fonts::title, colors::white, true, mod_info.name );
	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 36, fonts::normal, colors::dark, true, mod_info.version );

	cur_menu.cursor += sdk::vec2_t( 8, 64 );

	bool result = false;

	if ( !info->is_loaded ) {
		if ( button( sdk::vec2_t( size.x - 16, 26 ), "enable" ) )
			mods::enable( info );
	} else {
		if ( button( sdk::vec2_t( size.x - 16, 26 ), "disable" ) )
			mods::disable( info );

		result = button( sdk::vec2_t( size.x - 16, 26 ), "settings" );
	}

	++cur_menu.mod_count;

	if ( cur_menu.mod_count % 3 == 0 ) {
		cur_menu.cursor.x = 12;
		cur_menu.cursor.y += size.y + 8;
	} else
		cur_menu.cursor.x += size.x + 8;

	return result;
}


bool gui::framework::button( sdk::vec2_t size, std::string label ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	bool hover = !cur_menu.block_input && wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? clicking ? colors::white : colors::dark : colors::darker );
	wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg );

	const auto text_size = wh->render->get_text_size( fonts::normal, label.c_str( ) );

	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + size.y / 2 - text_size.y / 2, fonts::normal, colors::white, true, label.c_str( ) );

	cur_menu.cursor.y += size.y + 4;

	if ( hover && wh->input->get_key_press( sdk::mouse_left ) ) return true;

	return false;
}

void gui::framework::checkbox( bool &val, std::string label ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 20, 20 );

	bool hover = !cur_menu.block_input && wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	if ( hover && wh->input->get_key_press( sdk::mouse_left ) )
		val = !val;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, val ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, size.x - 6, size.y - 6, val ? colors::white : sdk::color_t( 0, 0, 0, 0 ) );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 2, fonts::normal, colors::white, false, label.c_str( ) );

	cur_menu.cursor.y += size.y + 4;
}

void gui::framework::slider( int &val, int min, int max, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 160, 18 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label.c_str( ) );

	bool hover = !cur_menu.block_input && wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	float value = ( float ) val / ( max - min );

	if ( clicking ) {
		value = ( wh->input->get_cursor_position( ).x - ( cur_pos.x + 3 ) ) / ( size.x - 6 );
		val = value * ( max - min );
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label.c_str( ) );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, clicking ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, value * ( size.x - 6 ), size.y - 6, colors::white );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 3, fonts::normal, colors::white, false, utils::string::ssprintf( "%d", val ).c_str( ) );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}

void gui::framework::sliderf( float &val, float min, float max, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 160, 18 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label.c_str( ) );

	bool hover = !cur_menu.block_input && wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	float value = val / ( max - min );

	if ( clicking ) {
		value = ( wh->input->get_cursor_position( ).x - ( cur_pos.x + 3 ) ) / ( size.x - 6 );
		val = value * ( max - min );
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label.c_str( ) );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, clicking ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, value * ( size.x - 6 ), size.y - 6, colors::white );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 3, fonts::normal, colors::white, false, utils::string::ssprintf( "%.1f", val ).c_str( ) );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}

void gui::framework::combo( std::size_t &val, std::vector<std::string> items, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 160, 26 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label.c_str( ) );

	bool open = cur_dropdown.id == label;

	bool hover = ( !cur_menu.block_input || open ) && wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_press( sdk::mouse_left );

	if ( clicking ) {
		if ( open )
			cur_dropdown = dropdown_t( );
		else {
			cur_dropdown.id = label;
			cur_dropdown.pos = sdk::vec2_t( cur_menu.cursor.x, cur_menu.cursor.y + size.y + text_size.y );
			cur_dropdown.items = items;
			cur_dropdown.value = val;
			cur_dropdown.multiselect = false;
		}

		open = !open;
	}

	if ( cur_dropdown.done ) {
		val = cur_dropdown.value;

		cur_dropdown = dropdown_t( );
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label.c_str( ) );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, open ? colors::white : colors::dark );

	wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, colors::white, false, items[ val ].c_str( ) );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}

void gui::framework::multicombo( std::size_t &val, std::vector<std::string> items, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 160, 26 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label.c_str( ) );

	bool open = cur_dropdown.id == label;

	bool hover = ( !cur_menu.block_input || open ) && wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_press( sdk::mouse_left );

	if ( clicking ) {
		if ( open ) {
			val = cur_dropdown.value;

			cur_dropdown = dropdown_t( );
		} else {
			cur_dropdown.id = label;
			cur_dropdown.pos = sdk::vec2_t( cur_menu.cursor.x, cur_menu.cursor.y + size.y + text_size.y );
			cur_dropdown.items = items;
			cur_dropdown.value = val;
			cur_dropdown.multiselect = true;
		}

		open = !open;
	}

	if ( open )
		val = cur_dropdown.value;

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label.c_str( ) );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, open ? colors::white : colors::dark );

	std::string display_text;
	bool comma = false;
	for ( std::size_t i = 0; i < items.size( ); ++i ) {
		if ( val & ( 1 << i ) ) {
			display_text += ( comma ? ", " : "" ) + items[ i ];

			comma = true;
		}
	}

	wh->portal2->surface->set_clip_rect( cur_pos.x + 8, cur_pos.y, cur_pos.x + size.x - 8, cur_pos.y + size.y );
	wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, colors::white, false, display_text.c_str( ) );
	wh->portal2->surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}
