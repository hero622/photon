#include "mod.h"

expose_wormhole_mod(c_wormhole_mod);

bool c_wormhole_mod::load(c_wormhole *wormhole) {
	c_wormhole_mod::wh = wormhole;

	wh->portal2->console->msg("Example mod loaded.\n");

	return true;
}

void c_wormhole_mod::unload() {
	wh->portal2->console->msg("Example mod unloaded.\n");
}

void c_wormhole_mod::on_pre_tick() {
	wh->portal2->console->msg("Example mod: pre_tick\n");
}
void c_wormhole_mod::on_post_tick() {
	wh->portal2->console->msg("Example mod: post_tick\n");
}
void c_wormhole_mod::on_pre_frame() {
}
void c_wormhole_mod::on_post_frame() {
}
void c_wormhole_mod::on_session_start() {
}
void c_wormhole_mod::on_session_end() {
}