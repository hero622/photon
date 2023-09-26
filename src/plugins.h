#pragma once

#include "wormhole_api.h"

#include <map>

namespace plugins {
	inline std::map<std::string, api::i_wormhole_plugin *> plugin_list;

	bool load(const char *name);
	void unload(const char *name);

	void print();
}  // namespace plugins