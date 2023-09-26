#pragma once

#include "sdk/sdk.h"

#include <vector>

class c_command {
private:
	sdk::con_command *ptr;

public:
	bool is_registered;

	static std::vector<c_command *> &get_list();

	c_command(const char *p_name, sdk::fn_command_callback_t callback, const char *p_help_string, int flags = 0, sdk::fn_command_completion_callback completion_func = 0);
	~c_command();

	void reg();
	void unreg();

	static void regall();
	static void unregall();
};

#define create_con_command(name, description)              \
	void name##_cbk(const sdk::c_command &args);              \
	c_command name##_command(#name, name##_cbk, description); \
	void name##_cbk(const sdk::c_command &args)