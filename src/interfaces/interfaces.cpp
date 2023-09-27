#include "interfaces.h"

bool c_portal2::init() {
	console = new i_console(utils::memory::get_module_handle(modules::tier0));

	cvar = copy_interface<c_cvar, i_cvar>(modules::tier1, "VEngineCvar007");
	server_game_dll = get_interface<i_server_game_dll>(module("server"), "ServerGameDLL005");
	engine_api = copy_interface<c_engine_api, void *>(module("engine"), "VENGINE_LAUNCHER_API_VERSION004");
	engine = engine_api->engine;
	engine_client = copy_interface<c_engine_client, void *>(module("engine"), "VEngineClient015");
	client_state = engine_client->client_state;

	return true;
}

void c_portal2::shutdown() {
	delete_ptr(engine_client);
	delete_ptr(engine_api);
	delete_ptr(cvar);
	delete_ptr(console);
}

c_portal2 *portal2;