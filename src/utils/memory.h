#pragma once

#include <cstdint>

#ifdef _WIN32
// clang-format off
#	include <windows.h>
#	include <memoryapi.h>
// clang-format on
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#	define MAX_PATH 4096
#endif

#ifdef _WIN32
#	define module(name) name ".dll"
#else
#	define module(name) name ".so"
#endif

#define dll_export extern "C" __declspec(dllexport)

namespace utils {
	namespace memory {
		struct module_info_t {
			char name[MAX_PATH];
			uintptr_t base;
			uintptr_t size;
			char path[MAX_PATH];
		};

		template <typename t>
		__forceinline static t call_virtual(void *name, int offset) {
			int *table = *(int **)name;
			int address = table[offset];
			return (t)(address);
		}

		template <typename t>
		__forceinline static t get_sym_addr(void *module_handle, const char *sym) {
#ifdef _WIN32
			return (t)GetProcAddress((HMODULE)module_handle, sym);
#else
			return (t)dlsym(module_handle, sym);
#endif
		}

		__forceinline static void *get_module_handle(const char *module_name) {
#ifdef _WIN32
			return GetModuleHandleA(module_name);
#else
			return dlopen(module_name, RTLD_NOLOAD | RTLD_NOW);
#endif
		}
		std::uint8_t *pattern_scan(const char *module_name, const char *signature) noexcept;
	}  // namespace memory
}  // namespace utils