#pragma once

#include "dependencies/minhook/MinHook.h"
#include "utils/utils.h"

class c_hooks {
public:
	decl_hk(int, game_frame, bool simulating);

	inline unsigned int get_virtual(void *_class, unsigned int index) {
		return static_cast<unsigned int>((*static_cast<int **>(_class))[index]);
	}

	bool init();
	void shutdown();
};

#define hk(vtable, name, off) \
	MH_CreateHook(reinterpret_cast<void *>(get_virtual(vtable, off)), &name##_hk, reinterpret_cast<void **>(&name))