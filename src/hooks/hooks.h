#pragma once

#include "command.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(void, game_frame, bool simulating);
	decl_hk(void, frame);
	decl_hk(void, set_signon_state, int state, int count, void *unk);
	decl_hk(void, paint, sdk::paint_mode_t mode);

	decl_hk_cmd(plugin_unload);

	void create_hook(void *target, void *detour, void **original) {
		*original = (void *)utils::memory::trampoline_hk((char *)target, (char *)detour, 6);
	}
	void remove_hook(void *target) {
	}

	bool init();
	void shutdown();
};

extern c_hooks *hooks;

#define hk_virtual(vtable, name, off)                                             \
	name##_addr = reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)); \
	shared->hooks->create_hook(name##_addr, name##_hk, reinterpret_cast<void **>(&name));

#define hk_addr(name, addr)                    \
	name##_addr = reinterpret_cast<void *>(addr); \
	shared->hooks->create_hook(name##_addr, name##_hk, reinterpret_cast<void **>(&name));

#define unhk(name) \
	shared->hooks->remove_hook(name##_addr)

#define hk_cmd(name) \
	shared->cmds->hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	shared->cmds->unhook(#name, name);