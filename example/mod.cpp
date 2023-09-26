#include "mod.h"

expose_wormhole_mod(c_wormhole_mod);

bool c_wormhole_mod::load(c_wormhole *wormhole) {
	c_wormhole_mod::wormhole = wormhole;

	wormhole->portal2->console->msg("Example mod loaded.\n");

	return true;
}

void c_wormhole_mod::unload() {
	wormhole->portal2->console->msg("Example mod unloaded.\n");
}