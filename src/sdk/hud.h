#pragma once

#include "source_sdk.h"

#include <vector>

namespace photon_api {
	enum hud_type {
		hudtype_hud,
		hudtype_thud,
	};

	struct hud_t {
		hud_type type;    // Determines if the hud is a THUD or not.
		vec2_t   pos;     // Values between (0, 1).
		vec2_t   anchor;  // Values between (0, 1).
		float    scale;   // Scale value to multiply by. (CURRENTLY USELESS)
		vec2_t   bounds;  // Screen boundaries.
	};

	class i_hud : public hud_t {
	public:
		virtual void        paint( )    = 0;
		virtual const char* get_name( ) = 0;
	};

	class i_thud : public hud_t {
	public:
		unsigned long font;                        // Don't modify this!
		const char*   format = "{name}: {value}";  // Don't modify this!

		virtual const char* get_text( ) = 0;  // Return C string value.
		virtual const char* get_name( ) = 0;  // Return C string name.
	};
}  // namespace photon_api

class c_hud {
public:
	virtual void reg( photon_api::i_hud* hud );      // Register a HUD component by pointer.
	virtual void reg( photon_api::i_thud* thud );    // Register a THUD component by pointer.
	virtual void unreg( photon_api::i_hud* hud );    // Unregister a HUD component by pointer.
	virtual void unreg( photon_api::i_thud* thud );  // Unregister a THUD component by pointer.

	virtual void draw_begin( photon_api::i_hud* hud );
	virtual void draw_end( );
	virtual void draw_filled_rect( int x, int y, int w, int h, color_t color );
	virtual void draw_outlined_rect( int x, int y, int w, int h, color_t color, int stroke_width = 1 );
	virtual void draw_line( int x, int y, int w, int h, color_t color );
	virtual void draw_text( int x, int y, h_font font, color_t color, bool center, const char* text );
	virtual void draw_texture( int x, int y, int w, int h, const char* texture, color_t color = color_t( 255, 255, 255, 255 ) );
};
