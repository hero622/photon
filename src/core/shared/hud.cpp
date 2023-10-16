#include "wormhole-sdk/wormhole.h"

#include <unordered_map>

std::unordered_map<wh_api::i_hud *, hud_instance_t> &c_huds::get_huds() {
	static std::unordered_map<wh_api::i_hud *, hud_instance_t> list;
	return list;
}

std::unordered_map<wh_api::i_thud *, thud_instance_t> &c_huds::get_thuds() {
	static std::unordered_map<wh_api::i_thud *, thud_instance_t> list;
	return list;
}

void c_huds::reg(wh_api::i_hud *hud) {
	auto instance = hud_instance_t();
	get_huds()[hud] = instance;
}

void c_huds::reg(wh_api::i_thud *hud) {
	auto instance = thud_instance_t();
	get_thuds()[hud] = instance;
}

void c_huds::unreg(wh_api::i_hud *hud) {
	get_huds().erase(hud);
}

void c_huds::unreg(wh_api::i_thud *hud) {
	get_thuds().erase(hud);
}

void c_huds::paint() {
	for (const auto &hud : get_huds()) {
		hud.first->paint();
	}

	for (const auto &thud : get_thuds()) {
		// resolve hud position based on alignment later
		// have some formatting system
		// read font in later
		wh->render->text(thud.second.pos.x, thud.second.pos.y, 5, {255, 255, 255, 255}, false, utils::string::ssprintf("%s:%s", thud.first->get_name(), thud.first->get_text()));
	}
}