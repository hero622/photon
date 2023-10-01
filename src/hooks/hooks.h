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

	void *create_hook(void *target, void *detour, void *original, int jmps) {
		auto fn_addr = (uintptr_t)utils::memory::pattern_scan(module("gameoverlayrenderer"), "55 8B EC 51 8B 45 10 C7");

		typedef void *(__cdecl * fn_t)(void *, void *, void *, int);
		fn_t fn = (fn_t)fn_addr;

		return fn(target, detour, original, jmps);
	}
	void remove_hook(void *target, bool should_log = false) {
		auto fn_addr = utils::memory::pattern_scan(module("gameoverlayrenderer"), "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 81 EC ? ? ? ? 56 8B 75 08");

		typedef void(__cdecl * fn_t)(void *, bool);
		fn_t fn = (fn_t)fn_addr;

		fn(target, should_log);
	}

	bool init();
	void shutdown();
};

extern c_hooks *hooks;

#define hk_virtual(vtable, name, off)                                             \
	name##_addr = reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)); \
	shared->hooks->create_hook(name##_addr, &name##_hk, &name, 0);

#define hk_addr(name, addr)                    \
	name##_addr = reinterpret_cast<void *>(addr); \
	shared->hooks->create_hook(name##_addr, &name##_hk, &name, 0);

#define unhk(name) \
	shared->hooks->remove_hook(name##_addr)

#define hk_cmd(name) \
	shared->cmds->hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	shared->cmds->unhook(#name, name);