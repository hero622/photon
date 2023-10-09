#include "hooks.h"

#include "mods.h"
#include "wormhole-sdk/portal2.h"
#include "wormhole.h"

bool hooks::initialize() {
	hk_virtual(wh->portal2->server_game_dll, game_frame, offsets::game_frame);
	hk_virtual(wh->portal2->engine, frame, offsets::frame);
	hk_virtual(wh->portal2->client_state, set_signon_state, offsets::set_signon_state);
	hk_virtual(wh->portal2->i_engine_vgui_internal, paint, offsets::paint);

	hk_cmd(plugin_unload);

	return true;
}

void hooks::uninitialize() {
	unhk_cmd(plugin_unload);

	unhk(paint);
	unhk(set_signon_state);
	unhk(frame);
	unhk(game_frame);
}

hk_fn(void, hooks::game_frame, bool simulating) {
	wh->events->post("pre_tick");

	game_frame(ecx, simulating);

	wh->events->post("post_tick");
}

hk_fn(void, hooks::frame) {
	wh->events->post("pre_frame");

	frame(ecx);

	wh->events->post("post_frame");
}

hk_fn(void, hooks::set_signon_state, int state, int count, void *unk) {
	set_signon_state(ecx, state, count, unk);

	if (state == sdk::signonstate_full)
		wh->events->post("session_start");
	else
		wh->events->post("session_end");
}

hk_fn(void, hooks::paint, sdk::paint_mode_t mode) {
	paint(ecx, mode);

	wh->portal2->surface->start_drawing();

	if (mode == sdk::paint_uipanels) {
		wh->events->post("paint");
	}

	wh->portal2->surface->finish_drawing();
}

hk_cmd_fn(hooks::plugin_unload) {
	if (args.arg_c() >= 2 && wormhole.get_plugin() && std::atoi(args[1]) == wormhole.plugin->index)
		wh->portal2->engine_client->client_cmd("wormhole_exit");
	else
		plugin_unload(args);
}