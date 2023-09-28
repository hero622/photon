#pragma once

#include "interfaces.h"
#include "portal2.h"
#include "utils/utils.h"

class c_engine_api {
public:
	void *ptr;

public:
	void *engine;

	c_engine_api(void *ptr) {
		this->ptr = ptr;

		auto is_running_simulation = utils::memory::get_virtual(this->ptr, offsets::is_running_simulation);
		auto eng_addr = **reinterpret_cast<void ***>(is_running_simulation + offsets::eng);
		this->engine = reinterpret_cast<void *>(eng_addr);
	}
};