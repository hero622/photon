#pragma once

#include "portal2.h"
#include "sdk/sdk.h"
#include "utils/utils.h"

class i_cvar {
public:
	void register_con_command(sdk::con_command_base *p_command_base) {
		using fn = void(__rescall *)(void *, sdk::con_command_base *);
		return utils::memory::call_virtual<fn>(this, offsets::i_cvar::register_con_command)(this, p_command_base);
	}
	void unregister_con_command(sdk::con_command_base *p_command_base) {
		using fn = void(__rescall *)(void *, sdk::con_command_base *);
		return utils::memory::call_virtual<fn>(this, offsets::i_cvar::unregister_con_command)(this, p_command_base);
	}
	sdk::con_command_base *find_command_base(const char *name) {
		using fn = sdk::con_command_base *(__rescall *)(void *, const char *);
		return utils::memory::call_virtual<fn>(this, offsets::i_cvar::find_command_base)(this, name);
	}
};

class c_cvar {
public:
	i_cvar *vtable;

	void *con_command_vtable;
	sdk::con_command_base *cmd_list;
};