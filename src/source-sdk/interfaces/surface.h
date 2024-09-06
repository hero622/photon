#pragma once

#include "sdk/source_sdk.h"
#include "util/util.h"

class i_surface {
public:
	__forceinline void draw_set_color( int r, int g, int b, int a ) {
		return util::call_virtual< OS( 14, 13 ), void >( this, r, g, b, a );
	}
	__forceinline void draw_filled_rect( int x0, int y0, int x1, int y1 ) {
		return util::call_virtual< 15, void >( this, x0, y0, x1, y1 );
	}
	__forceinline void draw_outlined_rect( int x0, int y0, int x1, int y1 ) {
		return util::call_virtual< 17, void >( this, x0, y0, x1, y1 );
	}
	__forceinline void draw_line( int x0, int y0, int x1, int y1 ) {
		return util::call_virtual< 18, void >( this, x0, y0, x1, y1 );
	}
	__forceinline void draw_poly_line( int* x, int* y, int num_points ) {
		return util::call_virtual< 19, void >( this, x, y, num_points );
	}
	__forceinline int draw_get_texture_id( const char* filename ) {
		return util::call_virtual< 33, int >( this, filename );
	}
	__forceinline void draw_set_texture_file( int id, const char* filename, int hardware_filter, bool force_reload ) {
		return util::call_virtual< 35, void >( this, id, filename, hardware_filter, force_reload );
	}
	__forceinline void draw_set_texture_rgba( int id, const unsigned char* rgba, int wide, int tall ) {
		return util::call_virtual< 36, void >( this, id, rgba, wide, tall );
	}
	__forceinline void draw_set_texture( int id ) {
		return util::call_virtual< 37, void >( this, id );
	}
	__forceinline void draw_get_texture_size( int id, int& wide, int& tall ) {
		return util::call_virtual< 38, void >( this, id, &wide, &tall );
	}
	__forceinline void draw_textured_rect( int x0, int y0, int x1, int y1 ) {
		return util::call_virtual< 39, void >( this, x0, y0, x1, y1 );
	}
	__forceinline bool is_texture_id_valid( int id ) {
		return util::call_virtual< 40, bool >( this, id );
	}
	__forceinline int create_new_texture_id( bool procedural = false ) {
		return util::call_virtual< 41, int >( this, procedural );
	}
	__forceinline void get_screen_size( int& wide, int& tall ) {
		return util::call_virtual< 42, void >( this, &wide, &tall );
	}
	__forceinline void unlock_cursor( ) {
		return util::call_virtual< 64, void >( this );
	}
	__forceinline h_font create_font( ) {
		return util::call_virtual< 69, h_font >( this );
	}
	__forceinline bool set_font_glyph_set( h_font font, const char* font_name, int tall, int weight, int blur, int scanlines, int flags, int range_min = 0, int range_max = 0 ) {
		return util::call_virtual< 70, bool >( this, font, font_name, tall, weight, blur, scanlines, flags, range_min, range_max );
	}
	__forceinline void get_text_size( h_font font, const wchar_t* text, int& wide, int& tall ) {
		return util::call_virtual< 77, void >( this, font, text, &wide, &tall );
	}
	__forceinline void draw_textured_polygon( int n, vertex_t* vertices, bool clip_vertices = true ) {
		return util::call_virtual< 104, void >( this, n, vertices, clip_vertices );
	}
	__forceinline void draw_filled_rect_fade( int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool horizontal ) {
		return util::call_virtual< 121, void >( this, x0, y0, x1, y1, alpha0, alpha1, horizontal );
	}
	__forceinline void get_clip_rect( int& x0, int& y0, int& x1, int& y1 ) {
		return util::call_virtual< 144, void >( this, &x0, &y0, &x1, &y1 );
	}
	__forceinline void set_clip_rect( int x0, int y0, int x1, int y1 ) {
		return util::call_virtual< 145, void >( this, x0, y0, x1, y1 );
	}
	__forceinline void draw_colored_text( h_font font, int x, int y, int r, int g, int b, int a, const char* text ) {
		// !!! this is __cdecl because it has varargs
		using fn_t = void( __cdecl* )( void*, h_font, int, int, int, int, int, int, const char* );
		return ( *reinterpret_cast< fn_t** >( this ) )[ 160 ]( this, font, x, y, r, g, b, a, text );
	}
	__forceinline void start_drawing( ) {
		static auto paint_traverse_ex = util::get_virtual< 117 >( this );
		return paint_traverse_ex.rel32< void( __rescall* )( void* ) >( OS( 0x7f, 0x53d ) )( this );
	}
	__forceinline void finish_drawing( ) {
		static auto paint_traverse_ex = util::get_virtual< 117 >( this );
		return paint_traverse_ex.rel32< void( __rescall* )( ) >( OS( 0x25b, 0x163 ) )( );
	}

private:
	char pad_0000[ 0x280 ];  // 0x0000
public:
	bool enable_clipping;  // 0x0280
};
