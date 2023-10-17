#include "wormhole-sdk/hud.h"

#include "core/huds/huds.h"

#include <iterator>

void c_huds::reg(wh_api::i_hud *hud) {
	huds::huds.push_back(hud);
}
void c_huds::reg(wh_api::i_thud *thud) {
	huds::thuds.push_back(thud);
}
void c_huds::unreg(wh_api::i_hud *hud) {
	std::vector<wh_api::i_hud *>::iterator it = std::find(huds::huds.begin(), huds::huds.end(), hud);
	if (it != huds::huds.end())
		huds::huds.erase(it);
	// huds::huds.erase(hud);
}
void c_huds::unreg(wh_api::i_thud *thud) {
	std::vector<wh_api::i_thud *>::iterator it = std::find(huds::thuds.begin(), huds::thuds.end(), thud);
	if (it != huds::thuds.end())
		huds::thuds.erase(it);
}