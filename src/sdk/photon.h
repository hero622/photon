#pragma once

#include "ccon.h"
#include "common.h"
#include "config.h"
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
		c_common* common;  // common functions.
		c_con*    con;     // create convars and concommands.
		c_signal* signal;  // photon's callback based hooking system.
		c_hud*    hud;     // register huds and thuds.
		c_render* render;  // wrapper around source's surface interface.
		c_input*  input;   // wrapper around source's inputsystem interface.
		c_menu*   menu;    // photon's own menu framework.
		c_config* config;  // photon's config system.
	};

	struct mod_info_t {
		const char* name;     // name of mod.
		const char* author;   // author of mod.
		const char* version;  // version of mod using semantic versioning.
	};

	class i_photon_mod {
	public:
		virtual bool       load( c_shared* photon )    = 0;  // called on mod load, return if load was successful or not.
		virtual void       unload( )                   = 0;  // called on mod unload.
		virtual void       on_event( const char* msg ) = 0;  // callback for all events, `msg` contains event name.
		virtual mod_info_t get_info( )                 = 0;  // return mod info here.
		virtual void       paint_menu( )               = 0;  // called every frame when mod's settings menu is shown.
	};
}  // namespace photon_api

extern photon_api::c_shared* photon;  // global pointer for shared classes.

#define EXPOSE_PHOTON_MOD( class_t, name ) \
	DLL_EXPORT class_t* create_mod( ) {       \
		return static_cast< class_t* >( &name ); \
	}
