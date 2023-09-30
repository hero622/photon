#include "hooks.h"

bool whmod_hooks::initialize() {
	shared->hooks->initialize();
	hk_addr(calc_view_model_lag, utils::memory::pattern_scan(module("client"), "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 1C 56 6A 00 6A 00 8D 45 F4 8B F1 8B 4B 0C 50 51 E8 ? ? ? ?"));
	shared->hooks->enable_hook(calc_view_model_lag_addr);

	return true;
}

void whmod_hooks::uninitialize() {
	shared->hooks->disable_hook(calc_view_model_lag_addr);
	shared->hooks->remove_hook(calc_view_model_lag_addr);
	shared->hooks->uninitialize();
}

hk_fn(void, whmod_hooks::calc_view_model_lag, sdk::vec3_t &origin, sdk::vec3_t &angles, sdk::vec3_t &original_angles) {
	return;
}