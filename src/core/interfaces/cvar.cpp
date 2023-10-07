#include "wormhole-sdk/interfaces/cvar.h"

#include "../portal2.h"
#include "wormhole-sdk/source_sdk.h"
#include "wormhole-sdk/utils.h"

void c_cvar::register_con_command(sdk::con_command_base *p_command_base) {
	return utils::memory::call_virtual<void>(offsets::register_con_command, ptr, p_command_base);
}
void c_cvar::unregister_con_command(sdk::con_command_base *p_command_base) {
	return utils::memory::call_virtual<void>(offsets::unregister_con_command, ptr, p_command_base);
}
sdk::con_command_base *c_cvar::find_command_base(const char *name) {
	return utils::memory::call_virtual<sdk::con_command_base *>(offsets::find_command_base, ptr, name);
}

c_cvar::c_cvar(void *ptr) {
	this->ptr = ptr;

	auto listdemo = reinterpret_cast<sdk::con_command *>(this->find_command_base("listdemo"));
	if (listdemo) {
		this->con_command_vtable = *(void **)listdemo;
	}
}