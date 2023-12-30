#include "photon-sdk/photon.h"

void c_render::draw_filled_rect( int x, int y, int w, int h, sdk::color_t color ) {
	photon->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	photon->portal2->surface->draw_filled_rect( x, y, x + w, y + h );
}

void c_render::draw_outlined_rect( int x, int y, int w, int h, sdk::color_t color ) {
	photon->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	photon->portal2->surface->draw_outlined_rect( x, y, x + w, y + h );
}

void c_render::draw_line( int x, int y, int w, int h, sdk::color_t color ) {
	photon->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	photon->portal2->surface->draw_line( x, y, x + w, y + h );
}

bool c_render::create_font( sdk::h_font &font, const char *font_name, int size, bool bold, int flags ) {
	font = photon->portal2->surface->create_font( );
	return photon->portal2->surface->set_font_glyph_set( font, font_name, size, bold ? 800 : 0, 0, 0, flags );
}

void c_render::destruct_font( sdk::h_font font ) {
	/*
	 * seems like the game doesnt have a way of destructing a specific font,
	 * so we have to do it ourselves
	 */

	for ( const auto &font_range : photon->portal2->font_manager->font_amalgams[ font ].fonts ) {
		const int idx = photon->portal2->font_manager->win32_fonts.find( font_range.win32_font );

		photon->portal2->mem_alloc->free( photon->portal2->font_manager->win32_fonts[ idx ] );
		photon->portal2->font_manager->win32_fonts.remove( idx );
	}

	photon->portal2->font_manager->font_amalgams.remove( font );
}

void c_render::draw_text( int x, int y, sdk::h_font font, sdk::color_t color, bool center, const char *text ) {
	int text_x = x;
	int text_y = y;

	if ( center ) {
		auto text_size = get_text_size( font, text );
		text_x = x - text_size.x / 2;
	}

	photon->portal2->surface->draw_colored_text( font, text_x, text_y, color.r, color.g, color.b, color.a, text );
}

sdk::vec2_t c_render::get_text_size( sdk::h_font font, const char *text ) {
	int text_width, text_height;

	std::string stxt = text;
	photon->portal2->surface->get_text_size( font, std::wstring( stxt.begin( ), stxt.end( ) ).c_str( ), text_width, text_height );

	return sdk::vec2_t( text_width, text_height );
}

void c_render::draw_texture( int x, int y, int w, int h, const char *texture, sdk::color_t color ) {
	// check if texture already exists
	int id = photon->portal2->surface->draw_get_texture_id( texture );
	if ( !id ) {
		id = photon->portal2->surface->create_new_texture_id( true );
		photon->portal2->surface->draw_set_texture_file( id, texture, false, true );
	}

	photon->portal2->surface->draw_set_texture( id );
	photon->portal2->surface->draw_set_color( color.r, color.g, color.b, color.a );
	photon->portal2->surface->draw_textured_rect( x, y, x + w, y + h );
}

sdk::vec2_t c_render::get_screen_size( ) {
	int w, h;

	photon->portal2->surface->get_screen_size( w, h );

	return sdk::vec2_t( w, h );
}

sdk::h_font c_render::get_font( unsigned long id ) {
	auto default_font = photon->portal2->scheme->get_font( "DefaultFixedOutline" );

	return default_font + id;
}

sdk::vec2_t c_render::normalize( sdk::vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return vec / screen_size;
}

sdk::vec2_t c_render::to_screen( sdk::vec2_t vec ) {
	const auto screen_size = get_screen_size( );

	return vec * screen_size;
}
