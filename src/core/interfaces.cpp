#include "interfaces.h"

#include "wormhole-sdk/portal2.h"
#include "wormhole-sdk/wormhole.h"

bool interfaces::initialize() {
	shared->portal2->console = new c_console(utils::memory::get_module_handle(modules::tier0));

	shared->portal2->cvar = get_interface<i_cvar>(modules::tier1, "VEngineCvar007");
	shared->portal2->server_game_dll = get_interface<void *>(module("server"), "ServerGameDLL005");
	shared->portal2->engine_api = get_interface<void *>(module("engine"), "VENGINE_LAUNCHER_API_VERSION004");
	shared->portal2->engine_client = get_interface<i_engine_client>(module("engine"), "VEngineClient015");
	shared->portal2->server_plugin_helpers = get_interface<void *>(module("engine"), "ISERVERPLUGINHELPERS001");
	shared->portal2->surface = get_interface<i_surface>(module("vguimatsurface"), "VGUI_Surface031");
	shared->portal2->i_engine_vgui_internal = get_interface<void *>(module("engine"), "VEngineVGui001");

	shared->portal2->engine = **reinterpret_cast<void ***>(utils::memory::get_virtual(shared->portal2->engine_api, offsets::is_running_simulation) + offsets::eng);
	shared->portal2->client_state = utils::memory::read<void *(*)()>(utils::memory::get_virtual(shared->portal2->engine_client, offsets::client_cmd) + offsets::get_client_state)();

	return true;
}

void interfaces::uninitialize() {
	delete_ptr(shared->portal2->console);
}