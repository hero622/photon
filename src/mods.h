#pragma once

#include "wormhole_api.h"

#include <map>

namespace mods {
	struct mod_info_t {
		void *handle;
		api::i_wormhole_mod *ptr;
	};

	inline std::map<std::string, mod_info_t> mod_list;

	bool load(const char *name);
	void unload(const char *name);
	void unloadall();

	void print();
}  // namespace mods