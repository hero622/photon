#pragma once

#include "interfaces.h"
#include "portal2.h"
#include "utils/utils.h"

class i_engine {
public:
};

class i_engine_api {
public:
};

class c_engine_api {
public:
	i_engine_api *vtable;

	i_engine *engine;

	c_engine_api(i_engine_api *vtable) {
		this->vtable = vtable;

		auto is_running_simulation = utils::memory::get_virtual(this->vtable, offsets::is_running_simulation);
		auto eng_addr = **reinterpret_cast<void ***>(is_running_simulation + offsets::eng);
		this->engine = reinterpret_cast<i_engine *>(eng_addr);
	}
};