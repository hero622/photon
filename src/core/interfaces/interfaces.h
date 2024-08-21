#pragma once

#include "source-sdk/interfaces.h"

#ifdef _WIN32
#include <d3d9.h>
#endif

namespace interfaces {
#ifdef _WIN32
	inline IDirect3DDevice9* d3d_device;
#endif

	inline c_console*            console;
	inline c_font_manager*       font_manager;
	inline i_command_line*       command_line;
	inline i_cvar*               cvar;
	inline i_engine_client*      engine_client;
	inline i_input_stack_system* input_stack_system;
	inline i_input_system*       input_system;
	inline i_mem_alloc*          mem_alloc;
	inline i_scheme_manager*     scheme_manager;
	inline i_scheme*             scheme;
	inline i_surface*            surface;
	inline void*                 client_state;
	inline void*                 engine_api;
	inline void*                 engine;
	inline void*                 server_plugin_helpers;

	bool initialize( );
	void uninitialize( );
};  // namespace interfaces
