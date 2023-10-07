#include "interfaces.h"

#include "../portal2.h"
#include "wh-sdk/wormhole.h"

bool interfaces::initialize() {
	shared->portal2->console = new c_console(utils::memory::get_module_handle(modules::tier0));

	shared->portal2->cvar = copy_interface<c_cvar>(modules::tier1, "VEngineCvar007");
	shared->portal2->server_game_dll = get_interface<void *>(module("server"), "ServerGameDLL005");
	shared->portal2->engine_api = copy_interface<c_engine_api>(module("engine"), "VENGINE_LAUNCHER_API_VERSION004");
	shared->portal2->engine = shared->portal2->engine_api->engine;
	shared->portal2->engine_client = copy_interface<c_engine_client>(module("engine"), "VEngineClient015");
	shared->portal2->client_state = shared->portal2->engine_client->client_state;
	shared->portal2->server_plugin_helpers = get_interface<void *>(module("engine"), "ISERVERPLUGINHELPERS001");
	shared->portal2->surface = copy_interface<c_surface>(module("vguimatsurface"), "VGUI_Surface031");
	shared->portal2->i_engine_vgui_internal = get_interface<void *>(module("engine"), "VEngineVGui001");

	return true;
}

void interfaces::uninitialize() {
	delete_ptr(shared->portal2->surface);
	delete_ptr(shared->portal2->engine_client);
	delete_ptr(shared->portal2->engine_api);
	delete_ptr(shared->portal2->cvar);
	delete_ptr(shared->portal2->console);
}