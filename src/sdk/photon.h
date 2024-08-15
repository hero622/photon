#pragma once

#include "ccon.h"
#include "common.h"
#include "hud.h"
#include "input.h"
#include "menu.h"
#include "platform.h"
#include "render.h"
#include "signal.h"
#include "source_sdk.h"

namespace photon_api {
	class c_shared {
	public:
		c_common* common;  // Common functions.
		c_con*    con;     // Create ConVars and ConCommands.
		c_signal* signal;  // Photon's callback based hooking system.
		c_hud*    hud;     // Register HUDs and THUDs.
		c_render* render;  // Wrapper around Source's Surface interface.
		c_input*  input;   // Wrapper around Source's InputSystem interface.
		c_menu*   menu;    // Photon's own menu framework.
	};

	struct mod_info_t {
		const char* name;     // Name of mod.
		const char* version;  // Version of mod using semantic versioning.
	};

	class i_photon_mod {
	public:
		virtual bool       load( c_shared* photon )    = 0;  // Called on mod load, return if load was successful or not.
		virtual void       unload( )                   = 0;  // Called on mod unload.
		virtual void       on_event( const char* msg ) = 0;  // Callback for all events, `msg` contains event name.
		virtual mod_info_t get_info( )                 = 0;  // Return mod info here.
		virtual void       paint_menu( )               = 0;  // Called every frame when mod's settings menu is shown.
	};
}  // namespace photon_api

extern photon_api::c_shared* photon;  // Global pointer for shared classes.

#define EXPOSE_PHOTON_MOD( class_t, name ) \
	DLL_EXPORT class_t* create_mod( ) {       \
		return static_cast< class_t* >( &name ); \
	}
