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
	};
}  // namespace api

#define expose_wormhole_mod(class_name)                               \
	extern "C" __declspec(dllexport) api::i_wormhole_mod *create_mod() { \
		return new class_name();                                            \
	}