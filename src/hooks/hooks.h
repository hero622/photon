#pragma once

#include "command.h"
#include "dependencies/minhook/MinHook.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(void, game_frame, bool simulating);
	decl_hk(void, frame);
	decl_hk(void, set_signon_state, int state, int count, void *unk);
	decl_hk(void, paint, sdk::paint_mode_t mode);

	decl_hk_cmd(plugin_unload);

	bool initialize() {
		return MH_Initialize() == MH_OK;
	}
	bool uninitialize() {
		return MH_Uninitialize() == MH_OK;
	}
	bool create_hook(void *target, void *detour, void **original) {
		return MH_CreateHook(target, detour, original) == MH_OK;
	}
	bool remove_hook(void *target) {
		return MH_RemoveHook(target) == MH_OK;
	}
	bool enable_hook(void *target) {
		return MH_EnableHook(target) == MH_OK;
	}
	bool disable_hook(void *target) {
		return MH_DisableHook(target) == MH_OK;
	}

	bool init();
	void shutdown();
};

extern c_hooks *hooks;

#define hk_virtual(vtable, name, off)                                                   \
	name##_addr = reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off));       \
	shared->hooks->create_hook(name##_addr, &name##_hk, reinterpret_cast<void **>(&name)); \
	shared->hooks->enable_hook(name##_addr)

#define hk_addr(name, addr)                                                             \
	name##_addr = reinterpret_cast<void *>(addr);                                          \
	shared->hooks->create_hook(name##_addr, &name##_hk, reinterpret_cast<void **>(&name)); \
	shared->hooks->enable_hook(name##_addr)

#define unhk(name)                         \
	shared->hooks->disable_hook(name##_addr); \
	shared->hooks->remove_hook(name##_addr)

#define hk_cmd(name) \
	c_command::hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	c_command::unhook(#name, name);