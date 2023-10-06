#include "interfaces.h"

#include "../portal2.h"
#include "wh-sdk/interfaces.h"

bool interfaces::initialize() {
	portal2->console = new c_console(utils::memory::get_module_handle(modules::tier0));

	portal2->cvar = copy_interface<c_cvar>(modules::tier1, "VEngineCvar007");
	portal2->server_game_dll = get_interface<void *>(module("server"), "ServerGameDLL005");
	portal2->engine_api = copy_interface<c_engine_api>(module("engine"), "VENGINE_LAUNCHER_API_VERSION004");
	portal2->engine = portal2->engine_api->engine;
	portal2->engine_client = copy_interface<c_engine_client>(module("engine"), "VEngineClient015");
	portal2->client_state = portal2->engine_client->client_state;
	portal2->server_plugin_helpers = get_interface<void *>(module("engine"), "ISERVERPLUGINHELPERS001");
	portal2->surface = copy_interface<c_surface>(module("vguimatsurface"), "VGUI_Surface031");
	portal2->i_engine_vgui_internal = get_interface<void *>(module("engine"), "VEngineVGui001");

	return true;
}

void interfaces::uninitialize() {
	delete_ptr(portal2->surface);
	delete_ptr(portal2->engine_client);
	delete_ptr(portal2->engine_api);
	delete_ptr(portal2->cvar);
	delete_ptr(portal2->console);
}