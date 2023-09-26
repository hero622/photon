#include "plugin.h"

expose_wormhole_plugin(c_plugin);

bool c_plugin::load(c_wormhole *wormhole) {
	wormhole->portal2->console->msg("Example plugin loaded.\n");

	return true;
}

void c_plugin::unload() {
	delete this;
}