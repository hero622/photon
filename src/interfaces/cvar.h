#pragma once

#include "portal2.h"
#include "sdk/sdk.h"
#include "utils/utils.h"

class c_cvar {
public:
	void *ptr;

	void register_con_command(sdk::con_command_base *p_command_base) {
		return utils::memory::call_virtual<void>(offsets::register_con_command, ptr, p_command_base);
	}
	void unregister_con_command(sdk::con_command_base *p_command_base) {
		return utils::memory::call_virtual<void>(offsets::unregister_con_command, ptr, p_command_base);
	}
	sdk::con_command_base *find_command_base(const char *name) {
		return utils::memory::call_virtual<sdk::con_command_base *>(offsets::find_command_base, ptr, name);
	}

public:
	void *con_command_vtable;

	c_cvar(void *ptr) {
		this->ptr = ptr;

		auto listdemo = reinterpret_cast<sdk::con_command *>(this->find_command_base("listdemo"));
		if (listdemo) {
			this->con_command_vtable = *(void **)listdemo;
		}
	}
};