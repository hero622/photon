#include "wormhole-sdk/hud.h"

#include "core/huds/huds.h"
#include "wormhole-sdk/wormhole.h"

#include <cstddef>

void c_hud::reg( wh_api::i_hud *hud ) {
	huds::huds.push_back( hud );
}
void c_hud::reg( wh_api::i_thud *thud ) {
	huds::thuds.push_back( thud );
}
void c_hud::unreg( wh_api::i_hud *hud ) {
	for ( std::size_t i = 0; i < huds::huds.size( ); ++i ) {
		if ( huds::huds[ i ] == hud )
			huds::huds.erase( huds::huds.begin( ) + i );
	}
}
void c_hud::unreg( wh_api::i_thud *thud ) {
	for ( std::size_t i = 0; i < huds::thuds.size( ); ++i ) {
		if ( huds::thuds[ i ] == thud )
			huds::thuds.erase( huds::thuds.begin( ) + i );
	}
}

static wh_api::i_hud *cur_hud;

static void setup_draw( int &x, int &y, int w, int h ) {
	cur_hud->bounds.x = std::fmax( cur_hud->bounds.x, x + w );
	cur_hud->bounds.y = std::fmax( cur_hud->bounds.y, y + h );

	const auto pos = wh->render->to_screen( cur_hud->pos );
	const auto anchor = cur_hud->anchor * cur_hud->bounds;

	const auto screen_pos = pos - anchor;

	x += screen_pos.x;
	y += screen_pos.y;
}

void c_hud::draw_begin( wh_api::i_hud *hud ) {
	cur_hud = hud;
}
void c_hud::draw_end( ) {
	cur_hud = nullptr;
}
void c_hud::draw_filled_rect( int x, int y, int w, int h, sdk::color_t color ) {
	if ( !cur_hud ) return;

	setup_draw( x, y, w, h );

	wh->render->draw_filled_rect( x, y, w, h, color );
}
void c_hud::draw_outlined_rect( int x, int y, int w, int h, sdk::color_t color ) {
	if ( !cur_hud ) return;

	setup_draw( x, y, w, h );

	wh->render->draw_outlined_rect( x, y, w, h, color );
}
void c_hud::draw_line( int x, int y, int w, int h, sdk::color_t color ) {
	if ( !cur_hud ) return;

	setup_draw( x, y, w, h );

	wh->render->draw_line( x, y, w, h, color );
}
void c_hud::draw_text( int x, int y, sdk::h_font font, sdk::color_t color, bool center, const char *text ) {
	if ( !cur_hud ) return;

	const auto text_size = wh->render->get_text_size( font, text );

	setup_draw( x, y, text_size.x, text_size.y );

	wh->render->draw_text( x, y, font, color, center, text );
}
void c_hud::draw_texture( int x, int y, int w, int h, const char *texture, sdk::color_t color ) {
	if ( !cur_hud ) return;

	setup_draw( x, y, w, h );

	wh->render->draw_texture( x, y, w, h, texture, color );
}
