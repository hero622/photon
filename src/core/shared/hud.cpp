#include "wormhole-sdk/hud.h"

#include "core/huds/huds.h"

void c_huds::reg(wh_api::i_hud *hud) {
	huds::huds.push_back(hud);
}
void c_huds::reg(wh_api::i_thud *thud) {
	thud->font = 6;
	huds::thuds.push_back(thud);
}
void c_huds::unreg(wh_api::i_hud *hud) {
	for (int i = 0; i < huds::huds.size(); ++i) {
		if (huds::huds[i] == hud)
			huds::huds.erase(huds::huds.begin() + i);
	}
	// huds::huds.erase(hud);
}
void c_huds::unreg(wh_api::i_thud *thud) {
	for (int i = 0; i < huds::thuds.size(); ++i) {
		if (huds::thuds[i] == thud)
			huds::thuds.erase(huds::thuds.begin() + i);
	}
}