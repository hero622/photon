#include "wormhole.h"

#include "interfaces/interfaces.h"
#include "utils/utils.h"

c_wormhole wormhole;

expose_single_interface_globalvar(c_wormhole, i_server_plugin_callbacks, interfaceversion_iserverplugincallbacks, wormhole);

c_wormhole::c_wormhole() {
}

bool c_wormhole::load(sdk::create_interface_fn interface_factory, sdk::create_interface_fn game_server_factory) {
	utils::console::alloc();

	if (interfaces::init()) {
		interfaces::console->color_msg({0, 255, 0, 255}, "Wormhole loaded.\n");

		return true;
	}

	return false;
}

void c_wormhole::unload() {
}

void c_wormhole::pause() {
}
void c_wormhole::un_pause() {
}
const char *c_wormhole::get_plugin_description() {
	return "wormhole";
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