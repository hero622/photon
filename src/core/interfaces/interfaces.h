#pragma once

#include "source-sdk/interfaces.h"

namespace interfaces {
	inline c_console*            console;
	inline i_cvar*               cvar;
	inline address_t             engine_api;
	inline i_engine_client*      engine_client;
	inline address_t             server_plugin_helpers;
	inline i_surface*            surface;
	inline i_input_system*       input_system;
	inline i_scheme_manager*     scheme_manager;
	inline i_input_stack_system* input_stack_system;
	inline i_mem_alloc*          mem_alloc;
	inline i_command_line*       command_line;
	inline address_t             engine;
	inline address_t             client_state;
	inline c_font_manager*       font_manager;
	inline i_scheme*             scheme;
	inline c_game_rules**        game_rules;
	inline c_host_state*         host_state;
	inline i_client_entity_list* entity_list;
	inline c_demo_player*        demo_player;

	bool initialize( );
	void uninitialize( );
};  // namespace interfaces
