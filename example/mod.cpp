#include "mod.h"

#include "hooks/hooks.h"

expose_wormhole_mod(c_wormhole_mod);

bool c_wormhole_mod::load(api::c_shared *shared) {
	::shared = shared;

	if (whmod_hooks::initialize()) {
		shared->portal2->console->msg("example mod loaded.\n");
	}

	return true;
}

void c_wormhole_mod::unload() {
	whmod_hooks::uninitialize();

	shared->portal2->console->msg("example mod unloaded.\n");
}

void c_wormhole_mod::on_pre_tick() {
}
void c_wormhole_mod::on_post_tick() {
}
void c_wormhole_mod::on_pre_frame() {
}
void c_wormhole_mod::on_post_frame() {
}
void c_wormhole_mod::on_session_start() {
}
void c_wormhole_mod::on_session_end() {
}