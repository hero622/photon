#include "menu.h"

#include "core/mods/mods.h"
#include "framework.h"
#include "wormhole-sdk/wormhole.h"

#include <filesystem>
#include <iostream>

bool menu::initialize() {
	wh->render->create_font(framework::fonts::normal, "Segoe UI Light", 22, false, sdk::fontflag_antialias);
	wh->render->create_font(framework::fonts::title, "Segoe UI Light", 32, false, sdk::fontflag_antialias);

	return true;
}

void menu::uninitialize() {
	// delete fonts here
}

void menu::paint() {
	if (wh->input->get_key_press(sdk::key_insert))
		open = !open;

	if (!open)
		return;

	const auto screen_size = wh->render->get_screen_size();

	const int tab_height = 50;
	const auto menu_size = sdk::vec2_t(700, 0.32f * screen_size.y);
	const auto menu_pos = sdk::vec2_t(screen_size.x / 2 - menu_size.x / 2, screen_size.y / 2 + tab_height / 2 + 8);

	static int tab = 1;

	menu::framework::begin(menu_pos, menu_size);

	if (menu::framework::tab(tab, sdk::vec2_t(screen_size.x / 2 - 100 - 130 - 6, screen_size.y / 2 - tab_height / 2), sdk::vec2_t(130, tab_height), "profiles")) {
	}
	if (menu::framework::tab(tab, sdk::vec2_t(screen_size.x / 2 - 100, screen_size.y / 2 - tab_height / 2), sdk::vec2_t(200, tab_height), "modules")) {
		for (const auto &mod : mods::mod_list) {
			if (menu::framework::mod(mod.second.ptr->get_name(), mod.first)) {
			}
		}
	}
	if (menu::framework::tab(tab, sdk::vec2_t(screen_size.x / 2 + 100 + 6, screen_size.y / 2 - tab_height / 2), sdk::vec2_t(130, tab_height), "settings")) {
		std::vector<std::string> mods;
		for (const auto &entry : std::filesystem::directory_iterator("wormhole")) {
			mods.push_back(entry.path().stem().string());
		}
		menu::framework::modlist(mods);
	}

	menu::framework::end();
}