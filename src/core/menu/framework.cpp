#include "framework.h"

#include "core/mods/mods.h"

void menu::framework::begin(sdk::vec2_t pos, sdk::vec2_t size) {
	cur_menu = menu_t();

	cur_menu.pos = pos;
	cur_menu.size = size;

	cur_menu.cursor = {12, 12};

	wh->render->draw_outlined_rect(cur_menu.pos.x, cur_menu.pos.y, cur_menu.size.x, cur_menu.size.y, colors::dark);
	wh->render->draw_filled_rect(cur_menu.pos.x + 1, cur_menu.pos.y + 1, cur_menu.size.x - 2, cur_menu.size.y - 2, colors::bg);
}

void menu::framework::end() {
}

bool menu::framework::tab(int &selected, sdk::vec2_t pos, sdk::vec2_t size, std::string title) {
	bool hover = wh->input->is_cursor_in_area(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
	bool active = (hover && wh->input->get_key_press(sdk::mouse_left)) || selected == cur_menu.tab_count;
	if (active)
		selected = cur_menu.tab_count;

	wh->render->draw_outlined_rect(pos.x, pos.y, size.x, size.y, active ? colors::white : hover ? colors::dark
	                                                                                            : colors::darker);
	wh->render->draw_filled_rect(pos.x + 1, pos.y + 1, size.x - 2, size.y - 2, colors::bg);

	const auto text_size = wh->render->get_text_size(fonts::title, title);

	wh->render->draw_text(pos.x + size.x / 2, pos.y + size.y / 2 - text_size.y / 2, fonts::title, colors::white, true, title);

	++cur_menu.tab_count;

	return active;
}

bool menu::framework::mod(std::string title, std::string subtitle) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t(220, 100);

	bool hover = wh->input->is_cursor_in_area(cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y);

	wh->render->draw_outlined_rect(cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::dark : colors::darker);
	wh->render->draw_filled_rect(cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg);

	wh->render->draw_text(cur_pos.x + size.x / 2, cur_pos.y + 8, fonts::title, colors::white, true, title);
	wh->render->draw_text(cur_pos.x + size.x / 2, cur_pos.y + 36, fonts::normal, colors::dark, true, subtitle);

	cur_menu.cursor += sdk::vec2_t(8, 68);
	bool result = button(sdk::vec2_t(size.x - 16, 26), "settings");
	cur_menu.cursor -= sdk::vec2_t(8, 68);

	++cur_menu.mod_count;

	if (cur_menu.mod_count % 3 == 0) {
		cur_menu.cursor.x = 12;
		cur_menu.cursor.y += size.y + 8;
	} else
		cur_menu.cursor.x += size.x + 8;

	return result;
}

bool menu::framework::button(sdk::vec2_t size, std::string title) {
	const auto cur_pos = cur_menu.pos + cur_menu.cursor;

	bool hover = wh->input->is_cursor_in_area(cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y);
	bool clicking = hover && wh->input->get_key_held(sdk::mouse_left);

	wh->render->draw_outlined_rect(cur_pos.x, cur_pos.y, size.x, size.y, hover ? clicking ? colors::white : colors::dark : colors::darker);
	wh->render->draw_filled_rect(cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg);

	const auto text_size = wh->render->get_text_size(fonts::normal, title);

	wh->render->draw_text(cur_pos.x + size.x / 2, cur_pos.y + size.y / 2 - text_size.y / 2, fonts::normal, colors::white, true, title);

	if (hover && wh->input->get_key_press(sdk::mouse_left))
		return true;

	return false;
}

void menu::framework::modlist(std::vector<std::string> items) {
	cur_menu.cursor.x = 12;

	auto cur_pos = cur_menu.pos + cur_menu.cursor;

	const auto size = sdk::vec2_t(cur_menu.size.x - 24, 40);

	for (const auto &mod : items) {
		bool hover = wh->input->is_cursor_in_area(cur_pos.x, cur_pos.y, cur_pos.x + size.x, cur_pos.y + size.y);

		wh->render->draw_outlined_rect(cur_pos.x, cur_pos.y, size.x, size.y, hover ? colors::dark : colors::darker);
		wh->render->draw_filled_rect(cur_pos.x + 1, cur_pos.y + 1, size.x - 2, size.y - 2, colors::bg);

		wh->render->draw_text(cur_pos.x + 8, cur_pos.y + 2, fonts::title, colors::white, false, mod);

		cur_menu.cursor.x = cur_menu.size.x - 24 - 80;
		cur_menu.cursor.y += 8;
		if (!mods::mod_list.count(mod)) {
			if (button(sdk::vec2_t(80, 24), "load"))
				mods::load(mod.c_str());
		} else {
			if (button(sdk::vec2_t(80, 24), "unload"))
				mods::unload(mod.c_str());
		}
		cur_menu.cursor.x = 12;
		cur_menu.cursor.y -= 8;

		cur_pos.y += 68;
	}
}