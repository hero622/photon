#pragma once

#include "command.h"
#include "dependencies/minhook/MinHook.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(int, game_frame, bool simulating);
	decl_hk(int, frame);
	decl_hk(int, set_signon_state, int state, int count, void *unk);

	decl_hk_cmd(plugin_unload);

	bool init();
	void shutdown();
};

extern c_hooks *hooks;

#define hk_virtual(vtable, name, off) \
	MH_CreateHook(reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)), &name##_hk, reinterpret_cast<void **>(&name))

#define hk_cmd(name) \
	c_command::hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	c_command::unhook(#name, name);