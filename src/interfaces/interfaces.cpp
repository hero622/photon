#include "interfaces.h"

bool c_portal2::init() {
	console = new i_console(utils::memory::get_module_handle(modules::tier0));

	cvar = copy_interface<c_cvar, i_cvar>(modules::tier1, "VEngineCvar007");
	server_game_dll = get_interface<i_server_game_dll>(module("server"), "ServerGameDLL005");

	return true;
}

void c_portal2::shutdown() {
	delete_ptr(cvar);
	delete_ptr(console);
}

c_portal2 *portal2;