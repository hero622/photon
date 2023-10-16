#include "menu.h"

#include "framework.h"
#include "wormhole-sdk/wormhole.h"

bool menu::initialize() {
	return true;
}

void menu::uninitialize() {
}

void menu::paint() {
	if (wh->input->get_key_press(sdk::key_insert))
		open = !open;

	if (!open)
		return;
}