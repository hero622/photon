#include "wormhole-sdk/menu.h"

#include "core/menu/framework.h"

bool c_menu::button( sdk::vec2_t size, const char *label ) {
	return gui::framework::button( size, label );
}
void c_menu::checkbox( bool &val, const char *label ) {
	return gui::framework::checkbox( val, label );
}
void c_menu::slider( int &val, int min, int max, const char *label ) {
}
void c_menu::sliderf( float &val, float min, float max, const char *label ) {
}
void c_menu::colorpicker( sdk::color_t &val, const char *label ) {
}
void c_menu::combo( int &val, const char **items, const char *label ) {
}
void c_menu::multicombo( int &val, const char **items, const char *label ) {
}
