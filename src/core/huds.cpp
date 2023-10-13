#include "huds.h"

#include "wormhole-sdk/wormhole.h"

void c_thud_container::paint() {
	for (int i = 0; i < thuds.size(); ++i) {
		const auto &thud = thuds[i];

		auto txt = thud->get_text();

		int x = get_screen_pos().x;
		int y = get_screen_pos().y + i * padding;

		// utils::render::text();
	}
}

sdk::vec2_t c_thud_container::get_screen_pos() {
	return pos;
}

void c_thud_container::add(wh_api::c_thud *thud) {
	thuds.push_back(thud);
}

void c_thud_container::remove(wh_api::c_thud *thud) {
	std::vector<wh_api::c_thud *>::iterator it = std::find(thuds.begin(), thuds.end(), thud);
	if (it != thuds.end())
		thuds.erase(it);
}