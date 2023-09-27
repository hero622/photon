#pragma once

#include "portal2.h"
#include "utils/utils.h"

class c_engine_client {
public:
	void *vtable;

	void *client_state;

	c_engine_client(void *vtable) {
		this->vtable = vtable;

		auto client_cmd_addr = utils::memory::get_virtual(this->vtable, offsets::client_cmd);

		using get_client_state_fn = void *(*)();
		auto get_client_state = utils::memory::read<get_client_state_fn>((uintptr_t)client_cmd_addr + offsets::get_client_state);
		this->client_state = get_client_state();
	}
};