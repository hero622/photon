#pragma once

#include "../source_sdk.h"
#include "../utils.h"

class c_surface {
public:
	void *ptr;

	void(__rescall *start_drawing)(void *thisptr);
	void(__rescall *finish_drawing)();

	virtual void draw_set_color(int r, int g, int b, int a);
	virtual void draw_filled_rect(int x0, int y0, int x1, int y1);
	virtual void draw_outlined_rect(int x0, int y0, int x1, int y1);
	virtual void draw_line(int x0, int y0, int x1, int y1);
	virtual void draw_set_text_font(sdk::h_font font);
	virtual void draw_set_text_color(sdk::color_t col);
	virtual void draw_set_texture_file(int id, const char *filename, int hardware_filter, bool force_reload);
	virtual void draw_set_texture_rgba(int id, const unsigned char *rgba, int wide, int tall);
	virtual void draw_set_texture(int id);
	virtual void draw_get_texture_size(int id, int &wide, int &tall);
	virtual void draw_textured_rect(int x0, int y0, int x1, int y1);
	virtual bool is_texture_id_valid(int id);
	virtual int create_new_texture_id(bool procedural = false);
	virtual int get_font_tall(sdk::h_font font);
	virtual int get_text_size(sdk::h_font font, const wchar_t *text, int &wide, int &tall);

	c_surface(void *ptr);
};