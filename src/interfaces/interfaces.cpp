#include "interfaces.h"

bool interfaces::init() {
	console = new i_console();
	auto h_tier0 = utils::memory::get_module_handle(module(tier0));
	console->msg = utils::memory::get_sym_addr<decltype(i_console::msg)>(h_tier0, "Msg");
	console->color_msg = utils::memory::get_sym_addr<decltype(i_console::color_msg)>(h_tier0, concolormsg_sym);
	console->warning = utils::memory::get_sym_addr<decltype(i_console::warning)>(h_tier0, "Warning");
	console->dev_msg = utils::memory::get_sym_addr<decltype(i_console::dev_msg)>(h_tier0, devmsg_sym);
	console->dev_warning = utils::memory::get_sym_addr<decltype(i_console::dev_warning)>(h_tier0, devwarningmsg_sym);

	return true;
}