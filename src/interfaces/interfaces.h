#pragma once

#include "console.h"
#include "cvar.h"
#include "utils/utils.h"

#include <Windows.h>
#include <string>

platform_def(tier0, "tier0.dll", "libtier0.so");
platform_def(tier1, "vstdlib.dll", "libvstdlib.so");

class c_portal2 {
public:
	template <typename ret>
	ret *get_interface(const std::string &module_name, const std::string &interface_name) {
		using create_interface_fn = void *(*)(const char *, int *);
		const auto fn = utils::memory::get_sym_addr<create_interface_fn>(utils::memory::get_module_handle(module_name.c_str()), "CreateInterface");

		if (fn) {
			void *result = nullptr;

			result = fn(interface_name.c_str(), nullptr);

			if (!result)
				return nullptr;

			return static_cast<ret *>(result);
		}

		return nullptr;
	}

	template <typename ret, typename iface>
	ret *copy_interface(const std::string &module_name, const std::string &interface_name) {
		auto c = new ret();
		c->vtable = get_interface<iface>(module_name, interface_name);
		return c;
	}

	bool init();
	void shutdown();

	i_console *console;
	c_cvar *cvar;
};

extern c_portal2 *portal2;