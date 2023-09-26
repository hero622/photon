#pragma once

#include "interfaces/interfaces.h"
#include "sdk/sdk.h"
#include "utils/utils.h"
#include "wormhole.h"

namespace api {
	class i_wormhole_plugin {
	public:
		virtual bool load(c_wormhole *wormhole) = 0;
		virtual void unload() = 0;;
	};
}  // namespace api

#define expose_wormhole_plugin(class_name)                                  \
	extern "C" __declspec(dllexport) api::i_wormhole_plugin *create_plugin() { \
		return new class_name();                                                  \
	}