#pragma once

#include "dependencies/minhook/MinHook.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(int, game_frame, bool simulating);
	decl_hk(int, frame);

	bool init();
	void shutdown();
};

#define hk(vtable, name, off) \
	MH_CreateHook(reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)), &name##_hk, reinterpret_cast<void **>(&name))