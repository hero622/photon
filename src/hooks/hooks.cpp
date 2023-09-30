#include "hooks.h"

#include "mods.h"
#include "wormhole.h"

c_hooks *hooks;

bool c_hooks::init() {
	if (MH_Initialize() != MH_OK)
		return false;

	hk_virtual(portal2->server_game_dll, game_frame, offsets::game_frame);
	hk_virtual(portal2->engine, frame, offsets::frame);
	hk_virtual(portal2->client_state, set_signon_state, offsets::set_signon_state);

	hk_cmd(plugin_unload);

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	return true;
}

void c_hooks::shutdown() {
	unhk_cmd(plugin_unload);

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

hk_fn(int, c_hooks::game_frame, bool simulating) {
	mods::events::on_pre_tick();
	auto result = c_hooks::game_frame(thisptr, simulating);
	mods::events::on_post_tick();
	return result;
}

hk_fn(int, c_hooks::frame) {
	mods::events::on_pre_frame();
	auto result = c_hooks::frame(thisptr);
	mods::events::on_post_frame();
	return result;
}

hk_fn(int, c_hooks::set_signon_state, int state, int count, void *unk) {
	auto result = c_hooks::set_signon_state(thisptr, state, count, unk);

	if (state == sdk::signonstate_full)
		mods::events::on_session_start();
	else
		mods::events::on_session_end();

	return result;
}

hk_cmd_fn(c_hooks::plugin_unload) {
	if (args.arg_c() >= 2 && wormhole.get_plugin() && std::atoi(args[1]) == wormhole.plugin->index)
		portal2->engine_client->client_cmd("wormhole_exit");
	else
		c_hooks::plugin_unload(args);
}