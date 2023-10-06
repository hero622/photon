#include "wh-sdk/convar.h"

#include "wh-sdk/wormhole.h"

c_convar *cvar;

std::vector<c_command *> &c_command::get_list() {
	static std::vector<c_command *> list;
	return list;
}

c_command::c_command() {
}

c_command::c_command(const char *name) {
	this->ptr = reinterpret_cast<sdk::con_command *>(portal2->cvar->find_command_base(name));
	this->is_registered = false;
	this->is_reference = true;
}

c_command::c_command(const char *p_name, sdk::fn_command_callback_t callback, const char *p_help_string, int flags, sdk::fn_command_completion_callback completion_func)
	: is_registered(false)
	, is_reference(false) {
	this->ptr = new sdk::con_command(p_name, callback, p_help_string, flags, completion_func);

	c_command::get_list().push_back(this);
}

c_command::~c_command() {
	if (!this->is_reference)
		delete_ptr(this->ptr);
}

void c_command::reg() {
	if (!this->is_registered) {
		*(void **)this->ptr = shared->portal2->cvar->con_command_vtable;
		shared->portal2->cvar->register_con_command(this->ptr);
	}
	this->is_registered = true;
}

void c_command::unreg() {
	if (this->is_registered)
		shared->portal2->cvar->unregister_con_command(this->ptr);
	this->is_registered = false;
}

void c_command::regall() {
	for (const auto &cmd : c_command::get_list()) {
		cmd->reg();
	}
}

void c_command::unregall() {
	for (const auto &cmd : c_command::get_list()) {
		cmd->unreg();
	}
}

bool c_command::hook(const char *name, sdk::fn_command_callback_t detour, sdk::fn_command_callback_t &original) {
	c_command cc(name);
	if (cc.ptr != nullptr) {
		original = cc.ptr->m_fn_command_callback;
		cc.ptr->m_fn_command_callback = detour;
		return true;
	}
	return false;
}

bool c_command::unhook(const char *name, sdk::fn_command_callback_t original) {
	c_command cc(name);
	if (cc.ptr != nullptr && original) {
		cc.ptr->m_fn_command_callback = original;
		return true;
	}
	return false;
}