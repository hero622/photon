#include "core/interfaces/interfaces.h"
#include "sdk/photon.h"

void c_render::draw_filled_rect( int x, int y, int w, int h, color_t color ) {
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_filled_rect( x, y, x + w, y + h );
}

void c_render::draw_outlined_rect( int x, int y, int w, int h, color_t color ) {
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_outlined_rect( x, y, x + w, y + h );
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

void c_render::draw_polygon( int n, vertex_t* vertices, color_t color ) {
	// create plain white texture
	static int     plain_tex = interfaces::surface->create_new_texture_id( true );
	static uint8_t rgba[ 4 ] = { 255, 255, 255, 255 };
	interfaces::surface->draw_set_texture_rgba( plain_tex, rgba, 1, 1 );

	interfaces::surface->draw_set_texture( plain_tex );
	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );

	interfaces::surface->draw_textured_polygon( n, vertices );
}

void c_render::draw_outlined_polygon( int n, vertex_t* vertices, color_t color ) {
	int* x = new int[ n ];
	int* y = new int[ n ];

	for ( int i = 0; i < n; ++i ) {
		vec2_t& vert = vertices[ i ].position;

		x[ i ] = round( vert.x );
		y[ i ] = round( vert.y );

		// ok so valves polygon drawing function sucks so we just draw *parts of it* on our own
		draw_line( x[ i ], y[ i ], 1, 1, color );
	}

	interfaces::surface->draw_set_color( color.r, color.g, color.b, color.a );
	interfaces::surface->draw_poly_line( x, y, n );

	delete x;
	delete y;
}

// delete pointer afterwards!
static vertex_t* rounded_rect_verts( int x, int y, int w, int h, int rounding ) {
	const int n_verts = rounding * 4;

	vertex_t* verts = new vertex_t[ 4 * n_verts ];
	for ( int i = 0; i < 4; ++i ) {
		int _x = x + ( ( i < 2 ) ? ( w - rounding ) : rounding );
		int _y = y + ( ( i % 3 ) ? ( h - rounding ) : rounding );

		float a = 90.f * i;

		for ( int j = 0; j < n_verts; j++ ) {
			float _a = math::deg_to_rad( a + ( j / ( float ) ( n_verts - 1 ) ) * 90.f );

			verts[ ( i * n_verts ) + j ] = vertex_t( vec2_t( _x + rounding * sin( _a ), _y - rounding * cos( _a ) ) );
		}
	}

	return verts;
}

void c_render::draw_rounded_rect( int x, int y, int w, int h, color_t color, int rounding ) {
	if ( rounding < 2 )
		return;

	auto verts = rounded_rect_verts( x, y, w, h, rounding );

	draw_polygon( 4 * rounding * 4, verts, color );

	delete verts;
}

void c_render::draw_rounded_outlined_rect( int x, int y, int w, int h, color_t color, int rounding ) {
	if ( rounding < 2 )
		return;

	auto verts = rounded_rect_verts( x, y, w - 1, h - 1, rounding );

	draw_outlined_polygon( 4 * rounding * 4, verts, color );

	delete verts;
}
