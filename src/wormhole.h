#pragma once

#include "interfaces/interfaces.h"
#include "sdk/sdk.h"

class c_wormhole : public sdk::i_server_plugin_callbacks {
public:
	c_portal2 *portal2;

public:
	c_wormhole();

	virtual bool load(sdk::create_interface_fn interface_factory, sdk::create_interface_fn game_server_factory);
	virtual void unload();
	virtual void pause();
	virtual void un_pause();
	virtual const char *get_plugin_description();
	virtual void level_init(char const *p_map_name);
	virtual void server_activate(void *p_edict_list, int edict_count, int client_max);
	virtual void game_frame(bool simulating);
	virtual void level_shutdown();
	virtual void client_fully_connect(void *p_edict);
	virtual void client_active(void *p_entity);
	virtual void client_disconnect(void *p_entity);
	virtual void client_put_in_server(void *p_entity, char const *playername);
	virtual void set_command_client(int index);
	virtual void client_settings_changed(void *p_edict);
	virtual int client_connect(bool *b_allow_connect, void *p_entity, const char *psz_name, const char *psz_address, char *reject, int maxrejectlen);
	virtual int client_command(void *p_entity, const void *&args);
	virtual int network_id_validated(const char *psz_user_name, const char *psz_network_id);
	virtual void on_query_cvar_value_finished(int i_cookie, void *p_player_entity, int e_status, const char *p_cvar_name, const char *p_cvar_value);
	virtual void on_edict_allocated(void *edict);
	virtual void on_edict_freed(const void *edict);
};

extern c_wormhole wormhole;