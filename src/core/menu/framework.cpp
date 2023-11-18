#include "framework.h"

#include "core/mods/mods.h"

#include <algorithm>

void gui::framework::begin( sdk::vec2_t pos, sdk::vec2_t size ) {
	cur_menu = menu_t( );

	cur_menu.pos = pos;
	cur_menu.size = size;

	cur_menu.cursor = sdk::vec2_t( 12, 12 );

	wh->render->draw_outlined_rect( cur_menu.pos.x, cur_menu.pos.y, cur_menu.size.x, cur_menu.size.y, colors::dark );
	wh->render->draw_filled_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.size.x - 2, cur_menu.size.y - 2, colors::bg );

	wh->portal2->surface->enable_clipping = true;
	wh->portal2->surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );
}

void gui::framework::end( ) {
	if ( cur_menu.cur_combo_items.size( ) ) {
		auto cur_pos = cur_menu.pos + cur_menu.cur_combo_pos;

		const auto size = sdk::vec2_t( 140, 26 );

		for ( const auto &item : cur_menu.cur_combo_items ) {
			wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, colors::white, false, item );

			wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, colors::dark );

			cur_pos.y += 25;
		}

		// bool hover = wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
		// bool clicking = hover && wh->input->get_key_press( sdk::mouse_left );

		// if ( clicking ) {
		// }

		// wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label );

		// cur_pos.y += text_size.y;

		// wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, colors::white, false, items[ val ] );

		// cur_menu.cursor.y += size.y + text_size.y + 4;
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

	const auto text_size = wh->render->get_text_size( fonts::title, title );

	wh->render->draw_text( pos.x + size.x / 2, pos.y + size.y / 2 - text_size.y / 2, fonts::title, colors::white, true, title );

	wh->portal2->surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );

	++cur_menu.tab_count;

	return active;
}

bool gui::framework::mod( std::string title, std::string subtitle ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 220, 100 );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::dark : colors::darker );
	wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg );

	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 8, fonts::title, colors::white, true, title );
	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 36, fonts::normal, colors::dark, true, subtitle );

	cur_menu.cursor += sdk::vec2_t( 8, 68 );
	bool result = button( sdk::vec2_t( size.x - 16, 26 ), "settings" );
	cur_menu.cursor -= sdk::vec2_t( 8, 68 );

	++cur_menu.mod_count;

	if ( cur_menu.mod_count % 3 == 0 ) {
		cur_menu.cursor.x = 12;
		cur_menu.cursor.y += size.y + 8;
	} else
		cur_menu.cursor.x += size.x + 8;

	return result;
}

void gui::framework::modlist( std::vector<std::string> items ) {
	cur_menu.cursor.x = 12;

	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( cur_menu.size.x - 24, 40 );

	for ( const auto &mod : items ) {
		bool hover = wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );

		wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::dark : colors::darker );
		wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg );

		wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::title, colors::white, false, mod );

		cur_menu.cursor.x = cur_menu.size.x - 24 - 80;
		cur_menu.cursor.y += 8;
		if ( !mods::mod_list.count( mod ) ) {
			if ( button( sdk::vec2_t( 80, 24 ), "load" ) )
				mods::load( mod.c_str( ) );
		} else {
			if ( button( sdk::vec2_t( 80, 24 ), "unload" ) )
				mods::unload( mod.c_str( ) );
		}
		cur_menu.cursor.x = 12;
		cur_menu.cursor.y -= 8;

		cur_pos.y += 68;
	}
}


bool gui::framework::button( sdk::vec2_t size, std::string label ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	bool hover = wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? clicking ? colors::white : colors::dark : colors::darker );
	wh->render->draw_filled_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg );

	const auto text_size = wh->render->get_text_size( fonts::normal, label );

	wh->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + size.y / 2 - text_size.y / 2, fonts::normal, colors::white, true, label );

	cur_menu.cursor.y += size.y + 12;

	if ( hover && wh->input->get_key_press( sdk::mouse_left ) ) return true;

	return false;
}

void gui::framework::checkbox( bool &val, std::string label ) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 20, 20 );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	if ( hover && wh->input->get_key_press( sdk::mouse_left ) )
		val = !val;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, val ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, size.x - 6, size.y - 6, val ? colors::white : sdk::color_t( 0, 0, 0, 0 ) );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 2, fonts::normal, colors::white, false, label );

	cur_menu.cursor.y += size.y + 4;
}

void gui::framework::slider( int &val, int min, int max, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 140, 20 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	float value = ( float ) val / ( max - min );

	if ( clicking ) {
		value = ( wh->input->get_cursor_position( ).x - ( cur_pos.x + 3 ) ) / ( size.x - 6 );
		val = value * ( max - min );
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, clicking ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, value * ( size.x - 6 ), size.y - 6, colors::white );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 2, fonts::normal, colors::white, false, utils::string::ssprintf( "%d", val ) );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}

void gui::framework::sliderf( float &val, float min, float max, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 140, 20 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_held( sdk::mouse_left );

	float value = val / ( max - min );

	if ( clicking ) {
		value = ( wh->input->get_cursor_position( ).x - ( cur_pos.x + 3 ) ) / ( size.x - 6 );
		val = value * ( max - min );
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, clicking ? colors::white : colors::dark );
	wh->render->draw_filled_rect( cur_pos.x + 3, cur_pos.y + 3, value * ( size.x - 6 ), size.y - 6, colors::white );

	wh->render->draw_text( cur_pos.x + size.x + 4, cur_pos.y - 2, fonts::normal, colors::white, false, utils::string::ssprintf( "%.1f", val ) );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}

void gui::framework::combo( int &val, bool &open, std::vector<std::string> items, std::string label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t( 140, 26 );
	const auto text_size = wh->render->get_text_size( fonts::normal, label );

	bool hover = wh->input->is_cursor_in_area( cur_pos.x + 3, cur_pos.y + text_size.y + 3, cur_pos.x + size.x - 3, cur_pos.y + text_size.y + size.y - 3 );
	bool clicking = hover && wh->input->get_key_press( sdk::mouse_left );

	if ( clicking )
		open = !open;

	if ( open ) {
		cur_menu.cur_combo_pos = sdk::vec2_t( cur_menu.cursor.x, cur_menu.cursor.y + size.y + text_size.y );
		cur_menu.cur_combo_items = items;
	}

	wh->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::white, false, label );

	cur_pos.y += text_size.y;

	wh->render->draw_outlined_rect( cur_pos.x, cur_pos.y, size.x, size.y, open ? colors::white : colors::dark );
	wh->render->draw_text( cur_pos.x + 8, cur_pos.y + 2, fonts::normal, colors::white, false, items[ val ] );

	cur_menu.cursor.y += size.y + text_size.y + 4;
}
