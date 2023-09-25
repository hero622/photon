#pragma once

#include "i_console.h"
#include "utils/utils.h"

#include <Windows.h>
#include <string>

platform_def(tier0, "tier0.dll", "libtier0.so");
platform_def(concolormsg_sym, "?ConColorMsg@@YAXABVColor@@PBDZZ", "_Z11ConColorMsgRK5ColorPKcz");
platform_def(devmsg_sym, "?DevMsg@@YAXPBDZZ", "_Z6DevMsgPKcz");
platform_def(devwarningmsg_sym, "?DevWarning@@YAXPBDZZ", "_Z10DevWarningPKcz");

namespace interfaces {
	template <typename ret>
	ret *get_interface(const std::string &module_name, const std::string &interface_name) {
		using create_interface_fn = void *(*)(const char *, int *);
		const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

		if (fn) {
			void *result = nullptr;

			result = fn(interface_name.c_str(), nullptr);

			if (!result)
				return nullptr;

			return static_cast<ret *>(result);
		}

		return nullptr;
	}

	bool init();

	inline i_console *console;
}  // namespace interfaces