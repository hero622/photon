#pragma once

#include "wormhole-sdk/wormhole.h"

#include <map>

namespace mods {
	struct mod_info_t {
		void *handle;
		wh_api::i_wormhole_mod *ptr;
	};

	inline std::map<std::string, mod_info_t> mod_list;

	bool load(const char *name);
	void unload(const char *name);
	void unloadall();

	void print();

	void post_event(const char *msg);
}  // namespace mods