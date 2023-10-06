#pragma once

#include "../source_sdk.h"

class c_cvar {
public:
	void *ptr;

	virtual void register_con_command(sdk::con_command_base *p_command_base);
	virtual void unregister_con_command(sdk::con_command_base *p_command_base);
	virtual sdk::con_command_base *find_command_base(const char *name);

public:
	void *con_command_vtable;

	c_cvar(void *ptr);
};