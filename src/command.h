#pragma once

#include "sdk/sdk.h"

#include <vector>

class c_command {
public:
	sdk::con_command *ptr;

	bool is_registered;
	bool is_reference;

	static std::vector<c_command *> &get_list();

	c_command();
	c_command(const char *name);
	c_command(const char *p_name, sdk::fn_command_callback_t callback, const char *p_help_string, int flags = 0, sdk::fn_command_completion_callback completion_func = 0);
	~c_command();

	void reg();
	void unreg();

	void regall();
	void unregall();

	bool hook(const char *name, sdk::fn_command_callback_t detour, sdk::fn_command_callback_t &original);
	bool unhook(const char *name, sdk::fn_command_callback_t original);
};

extern c_command *cmds;

#define create_con_command(name, description)                     \
	static void name##_cbk(const sdk::c_command &args);              \
	static c_command name##_command(#name, name##_cbk, description); \
	static void name##_cbk(const sdk::c_command &args)

#define decl_hk_cmd(name)                       \
	static inline sdk::fn_command_callback_t name; \
	static void name##_hk(const sdk::c_command &args)

#define hk_cmd_fn(name) \
	void name##_hk(const sdk::c_command &args)