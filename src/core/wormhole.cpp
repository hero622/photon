#include "wormhole.h"

#include "hooks.h"
#include "interfaces/interfaces.h"
#include "mods.h"

#include <cstring>

c_wormhole wormhole;

expose_single_interface_globalvar(c_wormhole, i_server_plugin_callbacks, interfaceversion_iserverplugincallbacks, wormhole);

wh::c_shared *shared;

c_wormhole::c_wormhole() {
	this->plugin = new c_plugin();
	shared = new wh::c_shared();
	shared->portal2 = portal2 = new c_portal2();
	shared->hook = hook = new c_hook();
}

bool c_wormhole::load(sdk::create_interface_fn interface_factory, sdk::create_interface_fn game_server_factory) {
	if (interfaces::initialize()) {
		if (hooks::initialize()) {
			c_command::regall();

			this->search_plugin();

			portal2->console->color_msg({0, 255, 0, 255}, "Wormhole loaded.\n");

			return true;
		}
	}

	return false;
}

bool c_wormhole::get_plugin() {
	auto server_plugin_helpers = reinterpret_cast<uintptr_t>(portal2->server_plugin_helpers);
	auto m_size = *reinterpret_cast<int *>(server_plugin_helpers + c_server_plugin_size);
	if (m_size > 0) {
		auto m_plugins = *reinterpret_cast<uintptr_t *>(server_plugin_helpers + c_server_plugin_plugins);
		for (auto i = 0; i < m_size; ++i) {
			auto ptr = *reinterpret_cast<sdk::c_plugin **>(m_plugins + sizeof(uintptr_t) * i);
			if (!std::strcmp(ptr->name, wormhole_plugin_sig)) {
				this->plugin->ptr = ptr;
				this->plugin->index = i;
				return true;
			}
		}
	}
	return false;
}
void c_wormhole::search_plugin() {
	this->find_plugin_thread = std::thread([this]() {
		sleep(1000);
		if (this->get_plugin()) {
			this->plugin->ptr->disable = true;
		}
	});
	this->find_plugin_thread.detach();
}

void c_wormhole::unload() {
	if (unloading) return;
	unloading = true;

	mods::unloadall();

	c_command::unregall();

	hooks::uninitialize();

	if (wormhole.get_plugin()) {
		auto unload_cmd = std::string("plugin_unload ") + std::to_string(wormhole.plugin->index);
		portal2->engine_client->cbuf_add(unload_cmd.c_str(), safe_unload_delay);
	}

	portal2->console->msg("Goodbye.\n");

	interfaces::uninitialize();
}

create_con_command(wormhole_exit, "unloads wormhole.\n") {
	wormhole.unload();
}

void c_wormhole::pause() {
}
void c_wormhole::un_pause() {
}
const char *c_wormhole::get_plugin_description() {
	return wormhole_plugin_sig;
}
void c_wormhole::level_init(char const *p_map_name) {
}
void c_wormhole::server_activate(void *p_edict_list, int edict_count, int client_max) {
}
void c_wormhole::game_frame(bool simulating) {
}
void c_wormhole::level_shutdown() {
}
void c_wormhole::client_fully_connect(void *p_edict) {
}
void c_wormhole::client_active(void *p_entity) {
}
void c_wormhole::client_disconnect(void *p_entity) {
}
void c_wormhole::client_put_in_server(void *p_entity, char const *playername) {
}
void c_wormhole::set_command_client(int index) {
}
void c_wormhole::client_settings_changed(void *p_edict) {
}
int c_wormhole::client_connect(bool *b_allow_connect, void *p_entity, const char *psz_name, const char *psz_address, char *reject, int maxrejectlen) {
	return 0;
}
int c_wormhole::client_command(void *p_entity, const void *&args) {
	return 0;
}
int c_wormhole::network_id_validated(const char *psz_user_name, const char *psz_network_id) {
	return 0;
}
void c_wormhole::on_query_cvar_value_finished(int i_cookie, void *p_player_entity, int e_status, const char *p_cvar_name, const char *p_cvar_value) {
}
void c_wormhole::on_edict_allocated(void *edict) {
}
void c_wormhole::on_edict_freed(const void *edict) {
}