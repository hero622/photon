#pragma once

#include "wormhole-sdk/hud.h"

class c_thud_container {
private:
	std::vector<wh_api::c_thud *> thuds;

public:
	sdk::vec2_t anchor;
	sdk::vec2_t pos;
	float size;
	float padding;

	void paint();
	sdk::vec2_t get_screen_pos();

	void add(wh_api::c_thud *thud);
	void remove(wh_api::c_thud *thud);
};