#pragma once

#include "convar.h"
#include "hook.h"
#include "interfaces.h"
#include "source_sdk.h"
#include "utils.h"

namespace wh {
	class c_shared {
	public:
		c_portal2 *portal2;
		c_hook *hook;
		c_convar *cvar;
	};

	class i_wormhole_mod {
	public:
		virtual bool load(c_shared *shared) = 0;  // called on plugin load
		virtual void unload() = 0;                // called on plugin unload
		virtual void on_pre_tick() = 0;           // called before CServerGameDLL::GameFrame
		virtual void on_post_tick() = 0;          // called after CServerGameDll::GameFrame
		virtual void on_pre_frame() = 0;          // called before CEngine::Frame
		virtual void on_post_frame() = 0;         // called after CEngine::Frame
		virtual void on_session_start() = 0;      // called on SIGNONSTATE_FULL
		virtual void on_session_end() = 0;        // called on !SIGNONSTATE_FULL
	};
}  // namespace wh

extern wh::c_shared *shared;

#define expose_wormhole_mod(class_name)        \
	dll_export wh::i_wormhole_mod *create_mod() { \
		return new class_name();                     \
	}