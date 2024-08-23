#include "framework.h"

#include "core/interfaces/interfaces.h"
#include "core/mods/mods.h"
#include "util/util.h"

#include <algorithm>

void gui::framework::set_theme( bool dark ) {
	if ( !dark ) {
		colors::bg       = color_t( 255, 255, 255, 255 );
		colors::fg       = color_t( 200, 200, 200, 255 );
		colors::text     = color_t( 0, 0, 0, 200 );
		colors::disabled = color_t( 0, 0, 0, 64 );
	} else {
		colors::bg       = color_t( 21, 21, 21, 255 );
		colors::fg       = color_t( 59, 59, 59, 255 );
		colors::text     = color_t( 255, 255, 255, 64 );
		colors::disabled = color_t( 255, 255, 255, 2 );
	}
}

void gui::framework::begin( vec2_t pos, vec2_t size ) {
	cur_menu = menu_t( );

	cur_menu.pos  = pos;
	cur_menu.size = size;

	cur_menu.cursor = vec2_t( 8, 8 );

	// block input to other controls when a dropdown or colorpicker is open
	cur_menu.block_input = !cur_dropdown.id.empty( ) || !cur_colorpicker.id.empty( );

	photon->render->draw_rounded_rect( cur_menu.pos.x, cur_menu.pos.y, cur_menu.size.x, cur_menu.size.y, colors::bg, 8 );

	interfaces::surface->enable_clipping = true;
	interfaces::surface->set_clip_rect( cur_menu.pos.x, cur_menu.pos.y, cur_menu.pos.x + cur_menu.size.x, cur_menu.pos.y + cur_menu.size.y );
}

void gui::framework::end( ) {
	// draw scrollbar only if the content cant fit in to the page
	const auto max_y = cur_menu.cursor.y - cur_menu.size.y;
	if ( max_y > 0 ) {
		const auto cur_pos = vec2_t( cur_menu.pos.x + cur_menu.size.x - 14, cur_menu.pos.y + 8 );

		const auto height = max( cur_menu.size.y - max_y, 16 );
		const auto size   = vec2_t( 8, cur_menu.size.y - height - 16 );

		bool hover    = !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + cur_menu.size.y - 16 );
		bool clicking = photon->input->get_key_held( mouse_left );

#define SCROLLBAR_ID "photon scrollbar"

		static int grab_pos{ };

		float value = ( float ) scroll_offset / max_y;

		if ( hover && clicking && cur_slider.empty( ) ) {
			cur_slider = SCROLLBAR_ID;
			grab_pos   = photon->input->get_cursor_position( ).y - cur_pos.y - value * size.y;
		} else if ( !clicking && !cur_slider.empty( ) )
			cur_slider.clear( );

		if ( cur_slider == SCROLLBAR_ID ) {
			value         = ( photon->input->get_cursor_position( ).y - cur_pos.y - grab_pos ) / size.y;
			scroll_offset = std::clamp( value * max_y, 0.f, max_y );
			value         = ( float ) scroll_offset / max_y;
		}

		photon->render->draw_rounded_rect( cur_pos.x, cur_pos.y + value * size.y, 8, height, cur_slider == SCROLLBAR_ID ? colors::accent : colors::fg, 4 );

#undef SCROLLBAR_ID
	} else
		scroll_offset = 0;  // reset scroll offset if content can fit in page

	interfaces::surface->set_clip_rect( 0, 0, photon->render->get_screen_size( ).x, photon->render->get_screen_size( ).y );
	interfaces::surface->enable_clipping = false;

	// draw dropdowns
	if ( !cur_dropdown.id.empty( ) ) {
		auto cur_pos = cur_dropdown.pos;

		const auto size = vec2_t( 140, 24 );

		cur_pos.y += size.y;

		photon->render->draw_rounded_rect( cur_pos.x, cur_pos.y, size.x, cur_dropdown.items.size( ) * size.y, colors::accent, 8 );
		photon->render->draw_rounded_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, cur_dropdown.items.size( ) * size.y - 2, colors::bg, 8 );

		for ( std::size_t i = 0; i < cur_dropdown.items.size( ); ++i ) {
			const auto& item = cur_dropdown.items[ i ];

			bool hover    = photon->input->is_cursor_in_area( cur_pos.x + 1, cur_pos.y + 1, cur_pos.x + size.x - 1, cur_pos.y + size.y - 1 );
			bool clicking = hover && photon->input->get_key_press( mouse_left );

			if ( !cur_dropdown.multiselect ) {
				if ( clicking ) {
					cur_dropdown.value = i;
					cur_dropdown.done  = true;
				}

				photon->render->draw_text( cur_pos.x + 8, cur_pos.y + 4, fonts::smaller, hover ? colors::accent : colors::text, false, util::to_upper( item ).c_str( ) );
			} else {
				if ( clicking )
					cur_dropdown.value ^= ( 1 << i );

				photon->render->draw_text( cur_pos.x + 8, cur_pos.y + 4, fonts::smaller, cur_dropdown.value & ( 1 << i ) ? colors::accent : colors::text, false, util::to_upper( item ).c_str( ) );
			}

			cur_pos.y += size.y;
		}
	}

	// draw colorpickers
	if ( !cur_colorpicker.id.empty( ) ) {
		auto cur_pos = cur_colorpicker.pos;

		auto size = vec2_t( 220, 200 );

		cur_pos.x += 20;

		photon->render->draw_rounded_rect( cur_pos.x, cur_pos.y, size.x, size.y, colors::fg, 8 );
		photon->render->draw_rounded_rect( cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg, 8 );

		cur_pos += vec2_t( 8, 8 );
		size -= vec2_t( 16, 16 );

		const auto width  = size.x - 24;
		const auto height = size.y - 20;

		/* logic */
		bool hover_sv = photon->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + width - 2, cur_pos.y + height - 2 );
		bool hover_h  = photon->input->is_cursor_in_area( cur_pos.x + size.x - 18, cur_pos.y, cur_pos.x + size.x - 18 + 20, cur_pos.y + height - 2 );
		bool hover_a  = photon->input->is_cursor_in_area( cur_pos.x, cur_pos.y + size.y - 14, cur_pos.x + width - 2, cur_pos.y + size.y - 14 + 14 );
		bool holding  = photon->input->get_key_held( mouse_left );
		bool clicking = photon->input->get_key_press( mouse_left );

		float& h = cur_colorpicker.h;
		float& s = cur_colorpicker.s;
		float& v = cur_colorpicker.v;
		float& a = cur_colorpicker.a;

		// clicking out
		// HACK: check if not first frame by checking if alpha is initialized
		if ( a != -1 && clicking && !hover_sv && !hover_h && !hover_a ) {
			cur_colorpicker = colorpicker_t( );
			return;
		}

		a = cur_colorpicker.value.a / 255.f;

#define SV_ID "photon colorpicker sv"
#define H_ID  "photon colorpicker h"
#define A_ID  "photon colorpicker a"

		if ( holding && cur_slider.empty( ) ) {
			if ( hover_sv )
				cur_slider = SV_ID;
			else if ( hover_h )
				cur_slider = H_ID;
			else if ( hover_a )
				cur_slider = A_ID;
		} else if ( !holding && !cur_slider.empty( ) )
			cur_slider.clear( );

		if ( cur_slider == SV_ID ) {
			s = std::clamp( ( photon->input->get_cursor_position( ).x - cur_pos.x ) / ( width - 2 ), 0.f, 1.f );
			v = 1.f - std::clamp( ( photon->input->get_cursor_position( ).y - cur_pos.y ) / ( height - 2 ), 0.f, 1.f );
		} else if ( cur_slider == H_ID )
			h = std::clamp( ( photon->input->get_cursor_position( ).y - cur_pos.y ) / ( height - 2 ), 0.f, 1.f );
		else if ( cur_slider == A_ID )
			a = std::clamp( ( photon->input->get_cursor_position( ).x - cur_pos.x ) / ( width - 2 ), 0.f, 1.f );

#undef SV_ID
#undef H_ID
#undef A_ID

		color_t col = color_t::from_hsv( h, s, v );
		col.a       = a * 255;

		cur_colorpicker.value = col;

		/* drawing */
		// saturation, value
		photon->render->draw_outlined_rect( cur_pos.x - 1, cur_pos.y - 1, width + 2, height + 2, colors::fg );
		photon->render->draw_gradient( cur_pos.x, cur_pos.y, width, height, { 255, 255, 255 }, color_t::from_hsv( h, 1.f, 1.f ), true );
		photon->render->draw_gradient( cur_pos.x, cur_pos.y, width, height, { 0, 0, 0, 0 }, { 0, 0, 0 }, false );

		// hue
		photon->render->draw_outlined_rect( cur_pos.x + size.x - 18 - 1, cur_pos.y - 1, 20, height + 2, colors::fg );
		photon->render->draw_texture( cur_pos.x + size.x - 18, cur_pos.y, 18, height, "photon_hue_gradient" );

		// alpha
		photon->render->draw_outlined_rect( cur_pos.x - 1, cur_pos.y + size.y - 14 - 1, width + 2, 16, colors::fg );
		photon->render->draw_filled_rect( cur_pos.x, cur_pos.y + size.y - 14, width, 14, col );

		/* draw cursors */
		// saturation, value
		photon->render->draw_outlined_rect( cur_pos.x + s * ( width - 2 ) - 1, cur_pos.y + ( 1.f - v ) * ( height - 2 ) - 1, 4, 4, colors::gray );
		photon->render->draw_filled_rect( cur_pos.x + s * ( width - 2 ), cur_pos.y + ( 1.f - v ) * ( height - 2 ), 2, 2, { 255, 255, 255, 64 } );

		// hue
		photon->render->draw_outlined_rect( cur_pos.x + size.x - 18 - 1, cur_pos.y + h * ( height - 2 ) - 1, 20, 3, colors::gray );
		photon->render->draw_filled_rect( cur_pos.x + size.x - 18, cur_pos.y + h * ( height - 2 ), 18, 1, { 255, 255, 255, 64 } );

		// alpha
		photon->render->draw_outlined_rect( cur_pos.x + a * ( width - 2 ) - 1, cur_pos.y + size.y - 14 - 1, 3, 16, colors::gray );
		photon->render->draw_filled_rect( cur_pos.x + a * ( width - 2 ), cur_pos.y + size.y - 14, 1, 14, { 255, 255, 255, 64 } );
	}
}

bool gui::framework::tab( int& selected, vec2_t pos, vec2_t size, const std::string& label, bool texture ) {
	bool hover  = photon->input->is_cursor_in_area( pos.x, pos.y, pos.x + size.x, pos.y + size.y );
	bool active = ( hover && photon->input->get_key_press( mouse_left ) ) || selected == cur_menu.tab_count;
	if ( active ) {
		selected = cur_menu.tab_count;

		cur_menu.cursor = vec2_t( 8, 8 );
	}

	interfaces::surface->set_clip_rect( 0, 0, photon->render->get_screen_size( ).x, photon->render->get_screen_size( ).y );

	photon->render->draw_filled_rect( pos.x, pos.y, size.x, size.y, hover ? colors::accent : colors::bg );
	if ( hover )
		photon->render->draw_outlined_rect( pos.x - 1, pos.y - 1, size.x + 2, size.y + 2, colors::bg, 3 );

	if ( !texture ) {
		const auto text_size = photon->render->get_text_size( fonts::title, util::to_upper( label ).c_str( ) );
		photon->render->draw_text( pos.x + size.x / 2, pos.y + size.y / 2 - text_size.y / 2, fonts::title, colors::text, true, util::to_upper( label ).c_str( ) );
	} else {
		// yep these numbers are hardcoded idc
		photon->render->draw_texture( pos.x + 12, pos.y + 12, 32, 32, label.c_str( ), colors::text );
	}

	interfaces::surface->set_clip_rect( cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.pos.x + cur_menu.size.x - 1, cur_menu.pos.y + cur_menu.size.y - 1 );

	++cur_menu.tab_count;

	return active;
}

bool gui::framework::mod( mods::mod_info_t& info ) {
	const auto size = vec2_t( 232, 158 );

	if ( cur_menu.mod_count % 3 == 0 ) {
		cur_menu.cursor.x = 8;
	} else {
		cur_menu.cursor.x += size.x;
		cur_menu.cursor.y -= 80 + 36 + 40 + 8;
	}

	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	photon->render->draw_filled_rect( cur_pos.x, cur_pos.y, size.x, size.y, info.is_loaded ? colors::green : colors::fg );

	const auto mod_info = info.ptr->get_info( );

	photon->render->draw_text( cur_pos.x + size.x / 2 + 2, cur_pos.y + 16 + 2, fonts::normal, colors::gray, true, mod_info.name );
	photon->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 16, fonts::normal, colors::white, true, mod_info.name );

	photon->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + 40, fonts::smaller, colors::text, true, util::ssprintf( "by %s", mod_info.author ).c_str( ) );

	cur_menu.cursor += vec2_t( 8, 80 );

	if ( !info.is_loaded ) {
		if ( button( { size.x - 16, 28 }, "ENABLE", true, fonts::smaller, colors::green ) )
			mods::enable( &info );
	} else {
		if ( button( { size.x - 16, 28 }, "DISABLE", true, fonts::smaller, colors::red ) )
			mods::disable( &info );
	}

	bool result = button( { size.x - 16, 32 }, "OPTIONS", info.is_loaded );

	++cur_menu.mod_count;

	cur_menu.cursor.y += 8;

	return result;
}

void gui::framework::split( int width ) {
	cur_menu.cursor.x += width + 8;
	cur_menu.cursor.y = 8;
}

bool gui::framework::icon_button( vec2_t size, const std::string& texture ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	bool hover    = !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && photon->input->get_key_press( mouse_left );

	photon->render->draw_filled_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::accent : colors::bg );
	if ( hover )
		photon->render->draw_outlined_rect( cur_pos.x - 1, cur_pos.y - 1, size.x + 2, size.y + 2, colors::bg, 3 );

	photon->render->draw_texture( cur_pos.x + 12, cur_pos.y + 12, 32, 32, texture.c_str( ), colors::text );

	cur_menu.cursor.y += size.y + 8;

	return clicking;
}

bool gui::framework::button( vec2_t size, const std::string& label, bool enabled, h_font font, color_t color ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	bool hover    = enabled && !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y );
	bool clicking = hover && photon->input->get_key_press( mouse_left );

	photon->render->draw_filled_rect( cur_pos.x, cur_pos.y, size.x, size.y, hover ? color : colors::bg );
	if ( hover )
		photon->render->draw_outlined_rect( cur_pos.x - 1, cur_pos.y - 1, size.x + 2, size.y + 2, colors::bg, 3 );

	const auto text_size = photon->render->get_text_size( font, util::to_upper( label ).c_str( ) );

	photon->render->draw_text( cur_pos.x + size.x / 2, cur_pos.y + size.y / 2 - text_size.y / 2, font, colors::text, true, util::to_upper( label ).c_str( ) );

	if ( !enabled )
		photon->render->draw_filled_rect( cur_pos.x, cur_pos.y, size.x, size.y, colors::disabled );

	cur_menu.cursor.y += size.y + 8;

	return clicking;
}

static inline vec2_t align_right( vec2_t pos, vec2_t size ) {
	auto pos_x = gui::framework::cur_menu.pos.x + gui::framework::cur_menu.size.x - size.x - 20;

	return vec2_t( pos_x, pos.y );
}

bool gui::framework::toggle( bool& val, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size        = vec2_t( 40, 20 );
	const int  radius      = size.y / 2 - 2;
	const int  radius_half = radius / 2;

	auto cur_pos2 = align_right( cur_pos, size );

	bool hover  = !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool result = false;

	if ( hover && photon->input->get_key_press( mouse_left ) ) {
		val    = !val;
		result = true;
	}

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y, size.x, size.y, val ? colors::accent : colors::fg, 10 );

	cur_pos2 += vec2_t( radius, radius );
	cur_pos2.x += val * size.x / 2;

	photon->render->draw_circle( cur_pos2.x + 2, cur_pos2.y + 2, radius, colors::white );

	cur_menu.cursor.y += size.y + 16;

	return result;
}

void gui::framework::slider( int& val, int min, int max, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size        = vec2_t( 200, 20 );
	const int  radius      = size.y / 2;
	const int  radius_half = radius / 2;

	auto cur_pos2 = align_right( cur_pos, size );

	bool hover    = !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool clicking = photon->input->get_key_held( mouse_left );

	if ( hover && clicking && cur_slider.empty( ) )
		cur_slider = label;
	else if ( !clicking && !cur_slider.empty( ) )
		cur_slider.clear( );

	float value = ( float ) val / ( max - min );

	if ( cur_slider == label ) {
		value = ( photon->input->get_cursor_position( ).x - cur_pos2.x ) / size.x;
		val   = std::clamp( value * ( max - min ), ( float ) min, ( float ) max );
		value = ( float ) val / ( max - min );
	}

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	cur_pos2.y += 6;

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y + 1, size.x, size.y - 13, colors::fg, 4 );
	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y + 1, value * size.x, size.y - 13, colors::accent, 4 );

	const auto text_size = photon->render->get_text_size( fonts::smaller, util::ssprintf( "%d", val ).c_str( ) );
	photon->render->draw_text( cur_pos2.x - text_size.x / 2 - 12, cur_pos2.y - 3, fonts::smaller, colors::text, true, util::ssprintf( "%d", val ).c_str( ) );

	cur_pos2 += vec2_t( radius_half, radius_half );
	cur_pos2.x += value * ( size.x - radius );

	photon->render->draw_circle( cur_pos2.x, cur_pos2.y, radius, colors::accent );
	photon->render->draw_circle( cur_pos2.x, cur_pos2.y, radius - 4, colors::white );

	cur_menu.cursor.y += size.y + 16;
}

void gui::framework::sliderf( float& val, float min, float max, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size        = vec2_t( 200, 20 );
	const int  radius      = size.y / 2;
	const int  radius_half = radius / 2;

	auto cur_pos2 = align_right( cur_pos, size );

	bool hover    = !cur_menu.block_input && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool clicking = photon->input->get_key_held( mouse_left );

	if ( hover && clicking && cur_slider.empty( ) )
		cur_slider = label;
	else if ( !clicking && !cur_slider.empty( ) )
		cur_slider.clear( );

	float value = val / ( max - min );

	if ( cur_slider == label ) {
		value = ( photon->input->get_cursor_position( ).x - cur_pos2.x ) / size.x;
		val   = std::clamp( value * ( max - min ), min, max );
		value = val / ( max - min );
	}

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	cur_pos2.y += 6;

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y + 1, size.x, size.y - 13, colors::fg, 4 );
	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y + 1, value * size.x, size.y - 13, colors::accent, 4 );

	const auto text_size = photon->render->get_text_size( fonts::smaller, util::ssprintf( "%.1f", val ).c_str( ) );
	photon->render->draw_text( cur_pos2.x - text_size.x / 2 - 12, cur_pos2.y - 3, fonts::smaller, colors::text, true, util::ssprintf( "%.1f", val ).c_str( ) );

	cur_pos2 += vec2_t( radius_half, radius_half );
	cur_pos2.x += value * ( size.x - radius );

	photon->render->draw_circle( cur_pos2.x, cur_pos2.y, radius, colors::accent );
	photon->render->draw_circle( cur_pos2.x, cur_pos2.y, radius - 4, colors::white );

	cur_menu.cursor.y += size.y + 16;
}

void gui::framework::colorpicker( color_t& val, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size = vec2_t( 20, 20 );

	auto cur_pos2 = align_right( cur_pos, size );

	bool open = cur_colorpicker.id == label;

	bool hover    = ( !cur_menu.block_input || open ) && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool clicking = hover && photon->input->get_key_press( mouse_left );

	if ( clicking ) {
		if ( open )
			cur_colorpicker = colorpicker_t( );
		else {
			cur_colorpicker.id    = label;
			cur_colorpicker.pos   = cur_pos2;
			cur_colorpicker.value = val;
			val.to_hsv( cur_colorpicker.h, cur_colorpicker.s, cur_colorpicker.v );
		}

		open = !open;
	}

	if ( open )
		val = cur_colorpicker.value;

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y, size.x, size.y, colors::gray, 8 );

	photon->render->draw_rounded_rect( cur_pos2.x + 1, cur_pos2.y + 1, size.x - 2, size.y - 2, colors::bg, 8 );
	photon->render->draw_rounded_rect( cur_pos2.x + 1, cur_pos2.y + 1, size.x - 2, size.y - 2, val, 8 );

	cur_menu.cursor.y += size.y + 16;
}

void gui::framework::combo( std::size_t& val, const std::vector< std::string >& items, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size = vec2_t( 140, 24 );

	auto cur_pos2 = align_right( cur_pos, size );
	cur_pos2.y -= 2;

	bool open = cur_dropdown.id == label;

	bool hover    = ( !cur_menu.block_input || open ) && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool clicking = hover && photon->input->get_key_press( mouse_left );

	if ( clicking ) {
		if ( open )
			cur_dropdown = dropdown_t( );
		else {
			cur_dropdown.id          = label;
			cur_dropdown.pos         = cur_pos2;
			cur_dropdown.items       = items;
			cur_dropdown.value       = val;
			cur_dropdown.multiselect = false;
		}

		open = !open;
	}

	if ( cur_dropdown.done ) {
		val = cur_dropdown.value;

		cur_dropdown = dropdown_t( );
	}

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y, size.x, size.y, open ? colors::accent : colors::fg, 8 );
	photon->render->draw_rounded_rect( cur_pos2.x + 1, cur_pos2.y + 1, size.x - 2, size.y - 2, colors::bg, 8 );

	photon->render->draw_text( cur_pos2.x + 8, cur_pos2.y + 4, fonts::smaller, colors::text, false, util::to_upper( items[ val ] ).c_str( ) );

	cur_menu.cursor.y += size.y + 16;
}

void gui::framework::multicombo( std::size_t& val, const std::vector< std::string >& items, const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size = vec2_t( 140, 24 );

	auto cur_pos2 = align_right( cur_pos, size );
	cur_pos2.y -= 2;

	bool open = cur_dropdown.id == label;

	bool hover    = ( !cur_menu.block_input || open ) && photon->input->is_cursor_in_area( cur_pos2.x, cur_pos2.y, cur_pos2.x + size.x, cur_pos2.y + size.y );
	bool clicking = hover && photon->input->get_key_press( mouse_left );

	if ( clicking ) {
		if ( open ) {
			val = cur_dropdown.value;

			cur_dropdown = dropdown_t( );
		} else {
			cur_dropdown.id          = label;
			cur_dropdown.pos         = cur_pos2;
			cur_dropdown.items       = items;
			cur_dropdown.value       = val;
			cur_dropdown.multiselect = true;
		}

		open = !open;
	}

	if ( open )
		val = cur_dropdown.value;

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	photon->render->draw_rounded_rect( cur_pos2.x, cur_pos2.y, size.x, size.y, open ? colors::accent : colors::fg, 8 );
	photon->render->draw_rounded_rect( cur_pos2.x + 1, cur_pos2.y + 1, size.x - 2, size.y - 2, colors::bg, 8 );

	std::string display_text;
	bool        comma = false;
	for ( std::size_t i = 0; i < items.size( ); ++i ) {
		if ( val & ( 1 << i ) ) {
			display_text += ( comma ? ", " : "" ) + items[ i ];

			comma = true;
		}
	}

	interfaces::surface->set_clip_rect( cur_pos2.x + 8, cur_pos2.y, cur_pos2.x + size.x - 8, cur_pos2.y + size.y );
	photon->render->draw_text( cur_pos2.x + 8, cur_pos2.y + 4, fonts::smaller, colors::text, false, util::to_upper( display_text ).c_str( ) );
	interfaces::surface->set_clip_rect( cur_menu.pos.x, cur_menu.pos.y, cur_menu.pos.x + cur_menu.size.x, cur_menu.pos.y + cur_menu.size.y );

	cur_menu.cursor.y += size.y + 16;
}

void gui::framework::separator( const std::string& label ) {
	auto cur_pos = cur_menu.pos + cur_menu.cursor;
	cur_pos.y -= scroll_offset;

	const auto size = vec2_t( cur_menu.size.x - cur_menu.cursor.x - 20, 2 );

	const auto text_size = photon->render->get_text_size( fonts::normal, util::to_upper( label ).c_str( ) );

	photon->render->draw_text( cur_pos.x, cur_pos.y, fonts::normal, colors::text, false, util::to_upper( label ).c_str( ) );

	photon->render->draw_filled_rect( cur_pos.x, cur_pos.y + text_size.y, size.x, size.y, colors::text );

	cur_menu.cursor.y += size.y + text_size.y + 8;
}
