#include "hooks.h"

#include "mods.h"
#include "portal2.h"
#include "wormhole.h"

bool hooks::initialize() {
	hk_virtual(portal2->server_game_dll, game_frame, offsets::game_frame);
	hk_virtual(portal2->engine, frame, offsets::frame);
	hk_virtual(portal2->client_state, set_signon_state, offsets::set_signon_state);
	hk_virtual(portal2->i_engine_vgui_internal, paint, offsets::paint);

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
	mods::events::on_pre_tick();

	hooks::game_frame(thisptr, simulating);

	mods::events::on_post_tick();
}

hk_fn(void, hooks::frame) {
	mods::events::on_pre_frame();

	hooks::frame(thisptr);

	mods::events::on_post_frame();
}

hk_fn(void, hooks::set_signon_state, int state, int count, void *unk) {
	hooks::set_signon_state(thisptr, state, count, unk);

	if (state == sdk::signonstate_full)
		mods::events::on_session_start();
	else
		mods::events::on_session_end();
}

hk_fn(void, hooks::paint, sdk::paint_mode_t mode) {
	hooks::paint(thisptr, mode);

	portal2->surface->start_drawing(portal2->surface->ptr);

	if (mode == sdk::paint_uipanels) {
	}

	portal2->surface->finish_drawing();
}

hk_cmd_fn(hooks::plugin_unload) {
	if (args.arg_c() >= 2 && wormhole.get_plugin() && std::atoi(args[1]) == wormhole.plugin->index)
		portal2->engine_client->client_cmd("wormhole_exit");
	else
		hooks::plugin_unload(args);
}