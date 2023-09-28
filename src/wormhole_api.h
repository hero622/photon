#pragma once

#include "interfaces/interfaces.h"
#include "sdk/sdk.h"
#include "utils/utils.h"
#include "wormhole.h"

namespace api {
	class i_wormhole_mod {
	public:
		virtual bool load(c_wormhole *wormhole) = 0;
		virtual void unload() = 0;
		virtual void on_pre_tick() = 0;       // called before CServerGameDLL::GameFrame
		virtual void on_post_tick() = 0;      // called after CServerGameDll::GameFrame
		virtual void on_pre_frame() = 0;      // called before CEngine::Frame
		virtual void on_post_frame() = 0;     // called after CEngine::Frame
		virtual void on_session_start() = 0;  // called on SIGNONSTATE_FULL
		virtual void on_session_end() = 0;    // called on !SIGNONSTATE_FULL
	};
}  // namespace api

#define expose_wormhole_mod(class_name)                               \
	extern "C" __declspec(dllexport) api::i_wormhole_mod *create_mod() { \
		return new class_name();                                            \
	}