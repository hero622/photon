#include "plugins.h"

#include "command.h"
#include "interfaces/interfaces.h"
#include "utils/utils.h"

#ifdef _WIN32
#	include <windows.h>
#	include <memoryapi.h>
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#	define MAX_PATH 4096
#endif

bool plugins::load(const char *name) {
#ifdef _WIN32
	auto lib = LoadLibraryA(utils::ssprintf("wormhole/%s.dll", name).c_str());
#else
	auto lib = dlopen(utils::ssprintf("wormhole/%s.so", name).c_str());
#endif

	if (lib) {
		using create_plugin_fn = api::i_wormhole_plugin *(*)();
		const auto fn = utils::memory::get_sym_addr<create_plugin_fn>(lib, "create_plugin");

		if (fn) {
			auto plugin = fn();

			if (!plugin)
				return false;

			auto result = plugin->load(&wormhole);

			if (!result)
				return false;

			plugin_list.insert(std::make_pair(name, plugin));

			return true;
		}
	}

	return false;
}

void plugins::unload(const char *name) {
	auto plugin = plugin_list[name];

	plugin->unload();

	delete_ptr(plugin);

	plugin_list.erase(name);
}

void plugins::unloadall() {
	for (const auto &plugin : plugin_list) {
		plugin.second->unload();
	}
}

void plugins::print() {
	portal2->console->msg("Loaded wormhole plugins (%d):\n", plugin_list.size());
	for (const auto &plugin : plugin_list) {
		portal2->console->msg("%s\n", plugin.first.c_str());
	}
}

create_con_command(wormhole_load, "wormhole_load <plugin name> - load a wormhole plugin.\n") {
	if (args.arg_c() >= 2) {
		plugins::load(args[1]);
	}
}

create_con_command(wormhole_unload, "wormhole_unload <plugin name> - unload a wormhole plugin.\n") {
	if (args.arg_c() >= 2) {
		plugins::unload(args[1]);
	}
}

create_con_command(wormhole_print, "prints a list of all loaded wormhole plugins.\n") {
	plugins::print();
}