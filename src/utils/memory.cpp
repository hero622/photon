#include "memory.h"

#include "string.h"

#include <cstring>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#	include <psapi.h>
#else
#	include <link.h>
#endif

bool utils::memory::get_module_info(const char *module_name, module_info_t *module_info) {
	if (g_module_info.empty()) {
#ifdef _WIN32
		HMODULE mods[1024];
		HANDLE proc = GetCurrentProcess();
		DWORD cb_needed;

		if (EnumProcessModules(proc, mods, sizeof(mods), &cb_needed)) {
			for (unsigned int i = 0; i < (cb_needed / sizeof(HMODULE)); ++i) {
				char sz_mod_name[MAX_PATH];

				if (GetModuleFileNameA(mods[i], sz_mod_name, sizeof(sz_mod_name))) {
					auto info = MODULEINFO();

					if (!GetModuleInformation(proc, mods[i], &info, sizeof(info)))
						continue;

					module_info_t mod_info;

					std::snprintf(mod_info.name, sizeof(mod_info.name), "%s", sz_mod_name);
					mod_info.addr = (std::uintptr_t)info.lpBaseOfDll;
					mod_info.size = (std::size_t)info.SizeOfImage;

					g_module_info.push_back(mod_info);
				}
			}
		}
#else
		dl_iterate_phdr([](struct dl_phdr_info *info, std::size_t, void *) {
			module_info_t mod_info;

			mod_info.name = info->dlpi_name;
			mod_info.addr = info->dlpi_addr + _info->dlpi_phdr[0].p_vaddr;
			mod_info.size = info->dlpi_phdr[0].p_memsz;

			g_module_info.push_back(mod_info);

			return 0;
		},
		                nullptr);
#endif
	}

	for (const auto &info : g_module_info) {
		if (!utils::string::stristr(info.name, module_name))
			continue;

		if (module_info != nullptr) {
			*module_info = info;
		}
		return true;
	}

	return false;
}

std::uint8_t *utils::memory::pattern_scan(const char *module_name, const char *signature) noexcept {
	module_info_t module_info;

	if (!get_module_info(module_name, &module_info))
		return nullptr;

	static auto pattern_to_byte = [](const char *pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char *>(pattern);
		auto end = const_cast<char *>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			} else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto size_of_image = module_info.size;

	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t *>(module_info.addr);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found)
			return &scan_bytes[i];
	}
}