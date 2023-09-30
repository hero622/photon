#include "interfaces.h"

c_portal2 *portal2;

bool c_portal2::init() {
	console = new c_console(utils::memory::get_module_handle(modules::tier0));

	cvar = copy_interface<c_cvar>(modules::tier1, "VEngineCvar007");
	server_game_dll = get_interface<void *>(module("server"), "ServerGameDLL005");
	engine_api = copy_interface<c_engine_api>(module("engine"), "VENGINE_LAUNCHER_API_VERSION004");
	engine = engine_api->engine;
	engine_client = copy_interface<c_engine_client>(module("engine"), "VEngineClient015");
	client_state = engine_client->client_state;
	server_plugin_helpers = get_interface<void *>(module("engine"), "ISERVERPLUGINHELPERS001");
	surface = copy_interface<c_surface>(module("vguimatsurface"), "VGUI_Surface031");
	i_engine_vgui_internal = get_interface<void *>(module("engine"), "VEngineVGui001");

	return true;
}

void c_portal2::shutdown() {
	delete_ptr(surface);
	delete_ptr(engine_client);
	delete_ptr(engine_api);
	delete_ptr(cvar);
	delete_ptr(console);
}