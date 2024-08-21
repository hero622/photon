#include "core/interfaces/interfaces.h"
#include "sdk/photon.h"

void c_render::draw_filled_rect( int x, int y, int w, int h, color_t color ) {
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_filled_rect( x, y, x + w, y + h );
}

void c_render::draw_outlined_rect( int x, int y, int w, int h, color_t color, int stroke_width ) {
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	for ( int i = 0; i < stroke_width; ++i ) {
		interfaces::surface->draw_outlined_rect( x - i, y - i, x + w + i, y + h + i );
	}
}

void c_render::draw_line( int x, int y, int w, int h, color_t color ) {
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_line( x, y, x + w, y + h );
}

bool c_render::create_font( h_font& font, const char* font_name, int size, bool bold, int flags ) {
	font = interfaces::surface->create_font( );
	return interfaces::surface->set_font_glyph_set( font, font_name, size, bold ? 800 : 0, 0, 0, flags );
}

void c_render::destruct_font( h_font font ) {
	/*
	 * seems like the game doesnt have a way of destructing a specific font,
	 * so we have to do it ourselves
	 */

	for ( const auto& font_range : interfaces::font_manager->font_amalgams[ font ].fonts ) {
		const int idx = interfaces::font_manager->win32_fonts.find( font_range.win32_font );

		interfaces::mem_alloc->free( interfaces::font_manager->win32_fonts[ idx ] );
		interfaces::font_manager->win32_fonts.remove( idx );
	}

	interfaces::font_manager->font_amalgams.remove( font );
}

void c_render::draw_text( int x, int y, h_font font, color_t color, bool center, const char* text ) {
	int text_x = x;
	int text_y = y;

	if ( center ) {
		auto text_size = get_text_size( font, text );
		text_x         = x - text_size.x / 2;
	}

	interfaces::surface->draw_colored_text( font, text_x, text_y, color.r, color.g, color.b, color.a, text );
}

vec2_t c_render::get_text_size( h_font font, const char* text ) {
	int text_width, text_height;

	std::string stxt = text;
	interfaces::surface->get_text_size( font, std::wstring( stxt.begin( ), stxt.end( ) ).c_str( ), text_width, text_height );

	return vec2_t( text_width, text_height );
}

void c_render::draw_texture( int x, int y, int w, int h, const char* texture, color_t color ) {
	// check if texture already exists
	int id = interfaces::surface->draw_get_texture_id( texture );
	if ( !id ) {
		id = interfaces::surface->create_new_texture_id( true );
		interfaces::surface->draw_set_texture_file( id, texture, false, true );
	}

	interfaces::surface->draw_set_texture( id );
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_textured_rect( x, y, x + w, y + h );
}

vec2_t c_render::get_screen_size( ) {
	int w, h;

	interfaces::surface->get_screen_size( w, h );

	return vec2_t( w, h );
}

h_font c_render::get_font( unsigned long id ) {
	auto default_font = interfaces::scheme->get_font( "DefaultFixedOutline" );

	return default_font + id;
}

vec2_t c_render::normalize( vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return vec / screen_size;
}

vec2_t c_render::to_screen( vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return vec * screen_size;
}
