#include "hooks.h"

#include "mods.h"

bool c_hooks::init() {
	if (MH_Initialize() != MH_OK)
		return false;

	hk(portal2->server_game_dll, game_frame, offsets::i_server_game_dll::game_frame);

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		return false;

	return true;
}

void c_hooks::shutdown() {
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