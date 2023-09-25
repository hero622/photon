#pragma once

#include "i_console.h"
#include "utils/utils.h"

#include <Windows.h>
#include <string>

#ifdef _WIN32
#	define tier0 "tier0"
#	define concolormsg_sym "?ConColorMsg@@YAXABVColor@@PBDZZ"
#	define devmsg_sym "?DevMsg@@YAXPBDZZ"
#	define devwarningmsg_sym "?DevWarning@@YAXPBDZZ"
#else
#	define tier0 "libtier0"
#	define con_olormsg_sym "_Z11ConColorMsgRK5ColorPKcz"
#	define devmsg_sym "_Z6DevMsgPKcz"
#	define devwarningmsg_sym "_Z10DevWarningPKcz"
#endif

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