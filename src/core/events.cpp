#include "wormhole-sdk/events.h"

#include "mods.h"

void c_events::post(const char *msg) {
	mods::post_event(msg);
}