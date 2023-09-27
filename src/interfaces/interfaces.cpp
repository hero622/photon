#include "interfaces.h"

bool c_portal2::init() {
	console = new i_console(utils::memory::get_module_handle(modules::tier0));

	cvar = copy_interface<c_cvar, i_cvar>(modules::tier1, "VEngineCvar007");

	return true;
}

void c_portal2::shutdown() {
	delete_ptr(cvar);
	delete_ptr(console);
}

c_portal2 *portal2;