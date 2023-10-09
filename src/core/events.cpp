#include "wormhole-sdk/events.h"

#include "mods.h"

void c_events::post(void *sender, const char *msg) {
	mods::post_event(sender, msg);
}