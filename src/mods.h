#pragma once

#include "wormhole_api.h"

#include <map>

namespace mods {
	inline std::map<std::string, api::i_wormhole_mod *> mod_list;

	bool load(const char *name);
	void unload(const char *name);
	void unloadall();

	void print();
}  // namespace mods