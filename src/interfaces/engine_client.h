#pragma once

#include "portal2.h"
#include "utils/utils.h"

class c_engine_client {
public:
	void *ptr;

	void(__cdecl *cbuf_add_text)(int e_target, const char *p_text, int n_tick_delay);

	void client_cmd(const char *sz_cmd_string) {
		return utils::memory::call_virtual<void>(offsets::client_cmd, ptr, sz_cmd_string);
	}
	void execute_client_cmd(const char *sz_cmd_string) {
		return utils::memory::call_virtual<void>(offsets::execute_client_cmd, ptr, sz_cmd_string);
	}
	int get_active_split_screen_slot() {
		return utils::memory::call_virtual<int>(offsets::get_active_split_screen_slot, ptr);
	}

public:
	void *client_state;

	void cbuf_add(const char *text, int delay) {
		auto e_target = this->get_active_split_screen_slot();
		this->cbuf_add_text(e_target, text, delay);
	}

	void safe_unload() {
		this->client_cmd("wormhole_exit");
	}

	c_engine_client(void *ptr) {
		this->ptr = ptr;

		auto client_cmd_addr = utils::memory::get_virtual(this->ptr, offsets::client_cmd);

		this->cbuf_add_text = utils::memory::read<decltype(cbuf_add_text)>(client_cmd_addr + offsets::cbuf_add_text);

		using get_client_state_fn = void *(*)();
		auto get_client_state = utils::memory::read<get_client_state_fn>(client_cmd_addr + offsets::get_client_state);
		this->client_state = get_client_state();
	}
};