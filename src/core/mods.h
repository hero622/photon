#pragma once

#include "wormhole-sdk/wormhole.h"

#include <unordered_map>

namespace mods {
	struct mod_info_t {
		void *handle;
		wh_api::i_wormhole_mod *ptr;
	};

	inline std::unordered_map<std::string, mod_info_t> mod_list;

	bool load(const char *name);
	void unload(const char *name);
	void unloadall();

	void print();

	void post_event(void *sender, const char *msg);
}  // namespace mods