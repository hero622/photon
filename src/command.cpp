#include "command.h"

#include "interfaces/interfaces.h"

std::vector<c_command *> &c_command::get_list() {
	static std::vector<c_command *> list;
	return list;
}

c_command::c_command(const char *p_name, sdk::fn_command_callback_t callback, const char *p_help_string, int flags, sdk::fn_command_completion_callback completion_func) {
	this->ptr = new sdk::con_command(p_name, callback, p_help_string, flags, completion_func);

	c_command::get_list().push_back(this);
}

c_command::~c_command() {
	delete this->ptr;
}

void c_command::reg() {
	*(void **)this->ptr = portal2->cvar->con_command_vtable;
	portal2->cvar->vtable->register_con_command(this->ptr);
	portal2->cvar->cmd_list = this->ptr;
}

void c_command::unreg() {
	portal2->cvar->vtable->unregister_con_command(this->ptr);
}

void c_command::regall() {
	for (const auto &cmd : c_command::get_list()) {
		cmd->reg();
	}
}

void c_command::unregall() {
	for (const auto &cmd : c_command::get_list()) {
		cmd->reg();
	}
}