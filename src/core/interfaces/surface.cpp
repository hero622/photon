#include "wormhole-sdk/interfaces/surface.h"

#include "../portal2.h"
#include "wormhole-sdk/source_sdk.h"
#include "wormhole-sdk/utils.h"

void c_surface::draw_set_color(int r, int g, int b, int a) {
	return utils::memory::call_virtual<void>(offsets::draw_set_color, ptr, r, g, b, a);
}
void c_surface::draw_filled_rect(int x0, int y0, int x1, int y1) {
	return utils::memory::call_virtual<void>(offsets::draw_filled_rect, ptr, x0, y0, x1, y1);
}
void c_surface::draw_outlined_rect(int x0, int y0, int x1, int y1) {
	return utils::memory::call_virtual<void>(offsets::draw_outlined_rect, ptr, x0, y0, x1, y1);
}
void c_surface::draw_line(int x0, int y0, int x1, int y1) {
	return utils::memory::call_virtual<void>(offsets::draw_line, ptr, x0, y0, x1, y1);
}
void c_surface::draw_set_text_font(sdk::h_font font) {
	return utils::memory::call_virtual<void>(offsets::draw_set_text_font, ptr, font);
}
void c_surface::draw_set_text_color(sdk::color_t col) {
	return utils::memory::call_virtual<void>(offsets::draw_set_text_color, ptr, col);
}
void c_surface::draw_set_texture_file(int id, const char *filename, int hardware_filter, bool force_reload) {
	return utils::memory::call_virtual<void>(offsets::draw_set_texture_file, ptr, id, filename, hardware_filter, force_reload);
}
void c_surface::draw_set_texture_rgba(int id, const unsigned char *rgba, int wide, int tall) {
	return utils::memory::call_virtual<void>(offsets::draw_set_texture_rgba, ptr, id, rgba, wide, tall);
}
void c_surface::draw_set_texture(int id) {
	return utils::memory::call_virtual<void>(offsets::draw_set_texture, ptr, id);
}
void c_surface::draw_get_texture_size(int id, int &wide, int &tall) {
	return utils::memory::call_virtual<void>(offsets::draw_get_texture_size, ptr, id, wide, tall);
}
void c_surface::draw_textured_rect(int x0, int y0, int x1, int y1) {
	return utils::memory::call_virtual<void>(offsets::draw_textured_rect, ptr, x0, y0, x1, y1);
}
bool c_surface::is_texture_id_valid(int id) {
	return utils::memory::call_virtual<bool>(offsets::is_texture_id_valid, ptr, id);
}
int c_surface::create_new_texture_id(bool procedural) {
	return utils::memory::call_virtual<int>(offsets::create_new_texture_id, ptr, procedural);
}
int c_surface::get_font_tall(sdk::h_font font) {
	return utils::memory::call_virtual<int>(offsets::get_font_tall, ptr, font);
}
int c_surface::get_text_size(sdk::h_font font, const wchar_t *text, int &wide, int &tall) {
	return utils::memory::call_virtual<int>(offsets::get_text_size, ptr, font, text, wide, tall);
}

c_surface::c_surface(void *ptr) {
	this->ptr = ptr;

	auto paint_traverse_ex = utils::memory::get_virtual(ptr, offsets::paint_traverse_ex);
	this->start_drawing = utils::memory::read<decltype(start_drawing)>(paint_traverse_ex + offsets::start_drawing);
	this->finish_drawing = utils::memory::read<decltype(finish_drawing)>(paint_traverse_ex + offsets::finish_drawing);
}