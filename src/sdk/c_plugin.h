#pragma once

#define interfaceversion_iserverplugincallbacks "ISERVERPLUGINCALLBACKS002"

namespace sdk {
	typedef void *(*create_interface_fn)(const char *p_name, int *p_return_code);
	typedef void *(*instantiate_interface_fn)();

	struct interface_reg_t {
		instantiate_interface_fn m_create_fn;
		const char *m_p_name;
		interface_reg_t *m_p_next;
		static interface_reg_t *s_p_interface_regs;

		interface_reg_t(instantiate_interface_fn fn, const char *p_name)
			: m_p_name(p_name) {
			m_create_fn = fn;
			m_p_next = s_p_interface_regs;
			s_p_interface_regs = this;
		}
	};

	class i_server_plugin_callbacks {
	public:
		virtual bool load(create_interface_fn interface_factory, create_interface_fn game_server_factory) = 0;
		virtual void unload() = 0;
		virtual void pause() = 0;
		virtual void un_pause() = 0;
		virtual const char *get_plugin_description() = 0;
		virtual void level_init(char const *p_map_name) = 0;
		virtual void server_activate(void *p_edict_list, int edict_count, int client_max) = 0;
		virtual void game_frame(bool simulating) = 0;
		virtual void level_shutdown() = 0;
		virtual void client_fully_connect(void *p_edict) = 0;
		virtual void client_active(void *p_entity) = 0;
		virtual void client_disconnect(void *p_entity) = 0;
		virtual void client_put_in_server(void *p_entity, char const *playername) = 0;
		virtual void set_command_client(int index) = 0;
		virtual void client_settings_changed(void *p_edict) = 0;
		virtual int client_connect(bool *b_allow_connect, void *p_entity, const char *psz_name, const char *psz_address, char *reject, int maxrejectlen) = 0;
		virtual int client_command(void *p_entity, const void *&args) = 0;
		virtual int network_id_validated(const char *psz_user_name, const char *psz_network_id) = 0;
		virtual void on_query_cvar_value_finished(int i_cookie, void *p_player_entity, int e_status, const char *p_cvar_name, const char *p_cvar_value) = 0;
		virtual void on_edict_allocated(void *edict) = 0;
		virtual void on_edict_freed(const void *edict) = 0;
	};

	struct c_plugin {
		char m_sz_name[128];                    // 0
		bool m_b_disable;                       // 128
		i_server_plugin_callbacks *m_p_plugin;  // 132
		int m_i_plugin_interface_version;       // 136
		void *m_p_plugin_module;                // 140
	};
}  // namespace sdk

#define expose_single_interface_globalvar(class_name, interface_name, version_name, global_var_name) \
	static void *__create##class_name##interface_name##_interface() {                                   \
		return static_cast<sdk::interface_name *>(&global_var_name);                                       \
	}                                                                                                   \
	static sdk::interface_reg_t __g_create##class_name##interface_name##_reg(__create##class_name##interface_name##_interface, version_name);