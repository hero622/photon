#include "mods.h"

#include "command.h"
#include "interfaces/interfaces.h"
#include "utils/utils.h"

#ifdef _WIN32
#	include <windows.h>
#	include <memoryapi.h>
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#endif

bool mods::load(const char *name) {
#ifdef _WIN32
	void *lib = LoadLibraryA(utils::string::ssprintf("wormhole/%s.dll", name).c_str());
#else
	void *lib = dlopen(utils::string::ssprintf("wormhole/%s.so", name).c_str(), RTLD_LAZY);
#endif

	if (lib) {
		using create_mod_fn = api::i_wormhole_mod *(*)();
		const auto fn = utils::memory::get_sym_addr<create_mod_fn>(lib, "create_mod");

		if (fn) {
			auto mod = fn();

			if (!mod) return false;

			auto result = mod->load(shared);

			if (!result)
				return false;

			mod_info_t info;
			info.handle = lib;
			info.ptr = mod;

			mod_list.insert(std::make_pair(name, info));

			return true;
		}
	}

	return false;
}

void mods::unload(const char *name) {
	auto mod = mod_list[name];

	mod.ptr->unload();

	delete_ptr(mod.ptr);

#ifdef _WIN32
	FreeLibrary((HMODULE)mod.handle);
#else
	dlclose(mod.handle);
#endif

	mod_list.erase(name);
}

void mods::unloadall() {
	for (const auto &mod : mod_list) {
		mod.second.ptr->unload();
	}
}

void mods::print() {
	portal2->console->msg("Loaded wormhole mods (%d):\n", mod_list.size());
	for (const auto &mod : mod_list) {
		portal2->console->msg("%s\n", mod.first.c_str());
	}
}

event_handler(on_pre_tick);
event_handler(on_post_tick);
event_handler(on_pre_frame);
event_handler(on_post_frame);
event_handler(on_session_start);
event_handler(on_session_end);

create_con_command(wormhole_load, "wormhole_load <mod name> - load a wormhole mod.\n") {
	if (args.arg_c() >= 2) {
		mods::load(args[1]);
	}
}

create_con_command(wormhole_unload, "wormhole_unload <mod name> - unload a wormhole mod.\n") {
	if (args.arg_c() >= 2) {
		mods::unload(args[1]);
	}
}

create_con_command(wormhole_print, "prints a list of all loaded wormhole mods.\n") {
	mods::print();
}