#include "wormhole-sdk/wormhole.h"

#include <array>

std::array<bool, 256> g_cur_keys, g_old_keys;
int g_cursor_x, g_cursor_y;

void c_input::poll_input() {
	std::copy(g_cur_keys.begin(), g_cur_keys.end(), g_old_keys.begin());

	for (std::size_t i = 0; i < 256; ++i) {
		g_cur_keys[i] = wh->portal2->input_system->is_button_down((sdk::button_code_t)i);
	}

	wh->portal2->input_system->get_cursor_position(&g_cursor_x, &g_cursor_y);
}

bool c_input::get_key_held(unsigned int code) {
	return g_cur_keys[code];
}

bool c_input::get_key_release(unsigned int code) {
	return !g_cur_keys[code] && g_old_keys[code];
}

bool c_input::get_key_press(unsigned int code) {
	return g_cur_keys[code] && !g_old_keys[code];
}

sdk::vec2_t c_input::get_cursor_position() {
	return sdk::vec2_t(g_cursor_x, g_cursor_y);
}

bool c_input::is_cursor_in_area(int x0, int y0, int x1, int y1) {
	return g_cursor_x > x0 && g_cursor_y > y0 && g_cursor_x < x1 && g_cursor_y < y1;
}