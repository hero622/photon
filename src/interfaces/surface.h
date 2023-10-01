#pragma once

#include "sdk/sdk.h"
#include "utils/utils.h"

class c_surface {
public:
	void *ptr;

	void(__rescall *start_drawing)(void *thisptr);
	void(__rescall *finish_drawing)();

	void draw_set_color(int r, int g, int b, int a) {
		return utils::memory::call_virtual<void>(offsets::draw_set_color, ptr, r, g, b, a);
	}
	void draw_filled_rect(int x0, int y0, int x1, int y1) {
		return utils::memory::call_virtual<void>(offsets::draw_filled_rect, ptr, x0, y0, x1, y1);
	}
	void draw_outlined_rect(int x0, int y0, int x1, int y1) {
		return utils::memory::call_virtual<void>(offsets::draw_outlined_rect, ptr, x0, y0, x1, y1);
	}
	void draw_line(int x0, int y0, int x1, int y1) {
		return utils::memory::call_virtual<void>(offsets::draw_line, ptr, x0, y0, x1, y1);
	}
	void draw_set_text_font(sdk::h_font font) {
		return utils::memory::call_virtual<void>(offsets::draw_set_text_font, ptr, font);
	}
	void draw_set_text_color(sdk::color_t col) {
		return utils::memory::call_virtual<void>(offsets::draw_set_text_color, ptr, col);
	}
	void draw_set_texture_file(int id, const char *filename, int hardware_filter, bool force_reload) {
		return utils::memory::call_virtual<void>(offsets::draw_set_texture_file, ptr, id, filename, hardware_filter, force_reload);
	}
	void draw_set_texture_rgba(int id, const unsigned char *rgba, int wide, int tall) {
		return utils::memory::call_virtual<void>(offsets::draw_set_texture_rgba, ptr, id, rgba, wide, tall);
	}
	void draw_set_texture(int id) {
		return utils::memory::call_virtual<void>(offsets::draw_set_texture, ptr, id);
	}
	void draw_get_texture_size(int id, int &wide, int &tall) {
		return utils::memory::call_virtual<void>(offsets::draw_get_texture_size, ptr, id, wide, tall);
	}
	void draw_textured_rect(int x0, int y0, int x1, int y1) {
		return utils::memory::call_virtual<void>(offsets::draw_textured_rect, ptr, x0, y0, x1, y1);
	}
	bool is_texture_id_valid(int id) {
		return utils::memory::call_virtual<bool>(offsets::is_texture_id_valid, ptr, id);
	}
	int create_new_texture_id(bool procedural = false) {
		return utils::memory::call_virtual<int>(offsets::create_new_texture_id, ptr, procedural);
	}
	int get_font_tall(sdk::h_font font) {
		return utils::memory::call_virtual<int>(offsets::get_font_tall, ptr, font);
	}
	int get_text_size(sdk::h_font font, const wchar_t *text, int &wide, int &tall) {
		return utils::memory::call_virtual<int>(offsets::get_text_size, ptr, font, text, wide, tall);
	}

public:
	c_surface(void *ptr) {
		this->ptr = ptr;

		auto paint_traverse_ex = utils::memory::get_virtual(ptr, offsets::paint_traverse_ex);
		this->start_drawing = utils::memory::read<decltype(start_drawing)>(paint_traverse_ex + offsets::start_drawing);
		this->finish_drawing = utils::memory::read<decltype(finish_drawing)>(paint_traverse_ex + offsets::finish_drawing);
	}
};