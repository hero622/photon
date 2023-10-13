#include "wormhole-sdk/hud.h"

std::vector<wh_api::c_hud *> &c_huds::get_list() {
	static std::vector<wh_api::c_hud *> list;
	return list;
}

void c_huds::reg(wh_api::c_hud *hud) {
	c_huds::get_list().push_back(hud);
}

void c_huds::unreg(wh_api::c_hud *hud) {
	std::vector<wh_api::c_hud *>::iterator it = std::find(c_huds::get_list().begin(), c_huds::get_list().end(), hud);
	if (it != c_huds::get_list().end())
		c_huds::get_list().erase(it);
}

void c_huds::paint_all() {
	for (const auto &hud : c_huds::get_list()) {
		hud->paint();
	}
}