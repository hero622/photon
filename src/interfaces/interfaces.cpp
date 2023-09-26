#include "interfaces.h"

bool c_portal2::init() {
	console = new i_console();
	auto h_tier0 = utils::memory::get_module_handle(tier0);
	console->msg = utils::memory::get_sym_addr<decltype(i_console::msg)>(h_tier0, symbols::i_console::msg);
	console->color_msg = utils::memory::get_sym_addr<decltype(i_console::color_msg)>(h_tier0, symbols::i_console::color_msg);
	console->warning = utils::memory::get_sym_addr<decltype(i_console::warning)>(h_tier0, symbols::i_console::warning);
	console->dev_msg = utils::memory::get_sym_addr<decltype(i_console::dev_msg)>(h_tier0, symbols::i_console::dev_msg);
	console->dev_warning = utils::memory::get_sym_addr<decltype(i_console::dev_warning)>(h_tier0, symbols::i_console::dev_warning);

	cvar = copy_interface<c_cvar, i_cvar>(tier1, "VEngineCvar007");
	cvar->cmd_list = *(sdk::con_command_base **)((uintptr_t)cvar + offsets::i_cvar::cmd_list);
	auto listdemo = reinterpret_cast<sdk::con_command *>(cvar->vtable->find_command_base("listdemo"));
	if (listdemo) {
		cvar->con_command_vtable = *(void **)listdemo;
	}

	return true;
}

c_portal2 *portal2;