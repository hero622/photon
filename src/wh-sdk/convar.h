#pragma once

#include "source_sdk.h"

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
	virtual ~c_command();

	virtual void reg();
	virtual void unreg();

	virtual void regall();
	virtual void unregall();

	virtual bool hook(const char *name, sdk::fn_command_callback_t detour, sdk::fn_command_callback_t &original);
	virtual bool unhook(const char *name, sdk::fn_command_callback_t original);
};

class c_variable {
	// public:
	// 	sdk::con_var *ptr;

	// 	bool is_registered;
	// 	bool is_reference;

	// 	static std::vector<c_command *> &get_list();
};

class c_convar {
public:
	c_command *cmds;
	c_variable *vars;
};

extern c_convar *cvar;

#define create_con_command(name, description)                     \
	static void name##_cbk(const sdk::c_command &args);              \
	static c_command name##_command(#name, name##_cbk, description); \
	static void name##_cbk(const sdk::c_command &args)

#define decl_hk_cmd(name)                       \
	static inline sdk::fn_command_callback_t name; \
	static void name##_hk(const sdk::c_command &args)

#define hk_cmd_fn(name) \
	void name##_hk(const sdk::c_command &args)