#include "wormhole-sdk/event.h"

#include "core/mods/mods.h"

void c_event::post( void *sender, const char *msg ) {
	mods::post_event( sender, msg );
}
