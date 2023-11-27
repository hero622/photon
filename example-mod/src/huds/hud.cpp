#include "hud.h"

#include <wormhole.h>

c_example_hud *ex_hud;
c_example_thud *ex_thud;

bool huds::initialize( ) {
	wh->hud->reg( ex_hud = new c_example_hud( ) );
	wh->hud->reg( ex_thud = new c_example_thud( ) );

	return true;
}

void huds::uninitialize( ) {
	wh->hud->unreg( ex_hud );
	wh->hud->unreg( ex_thud );
}


void c_example_hud::paint( ) {
	wh->hud->draw_begin( this );

	wh->hud->draw_filled_rect( 0, 0, 20, 20, sdk::color_t( 255, 0, 0, 255 ) );
	wh->hud->draw_filled_rect( 20, 20, 20, 20, sdk::color_t( 0, 255, 0, 255 ) );
	wh->hud->draw_filled_rect( 40, 40, 20, 20, sdk::color_t( 0, 0, 255, 255 ) );
	wh->hud->draw_filled_rect( 0, 60, 80, 20, sdk::color_t( 255, 255, 255, 255 ) );

	wh->hud->draw_end( );
}
const char *c_example_hud::get_name( ) {
	return "example hud";
}


const char *c_example_thud::get_text( ) {
	return "0";
}
const char *c_example_thud::get_name( ) {
	return "example-thud";
}
