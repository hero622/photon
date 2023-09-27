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

	namespace events {
		void on_pre_tick();
		void on_post_tick();
		void on_pre_frame();
		void on_post_frame();
		void on_session_start();
		void on_session_end();
	}  // namespace events
}  // namespace mods

#define event_handler(name)          \
	void mods::events::name() {         \
		for (const auto &mod : mod_list) { \
			mod.second.ptr->name();           \
		}                                  \
	}