#pragma once

#include "command.h"
#include "deps/subhook/subhook.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(void, game_frame, bool simulating);
	decl_hk(void, frame);
	decl_hk(void, set_signon_state, int state, int count, void *unk);
	decl_hk(void, paint, sdk::paint_mode_t mode);

	decl_hk_cmd(plugin_unload);

	void create_hook(void *src, void *dst, subhook_t &hook, void **orig) {
		hook = subhook_new(src, dst, SUBHOOK_TRAMPOLINE);
		*orig = subhook_get_trampoline(hook);
		subhook_install(hook);
	}
	void remove_hook(subhook_t hook) {
		subhook_remove(hook);
		subhook_free(hook);
	}
	void enable_hook(subhook_t hook) {
		subhook_install(hook);
	}
	void disable_hook(subhook_t hook) {
		subhook_remove(hook);
	}

	bool init();
	void shutdown();
};

extern c_hooks *hooks;

#define hk_virtual(vtable, name, off) \
	shared->hooks->create_hook(reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)), name##_hk_fn, name##_hk, reinterpret_cast<void **>(&name));

#define hk_addr(name, addr) \
	shared->hooks->create_hook(reinterpret_cast<void *>(addr), name##_hk_fn, name##_hk, reinterpret_cast<void **>(&name));

#define unhk(name) \
	shared->hooks->remove_hook(name##_hk)

#define hk_cmd(name) \
	shared->cmds->hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	shared->cmds->unhook(#name, name);