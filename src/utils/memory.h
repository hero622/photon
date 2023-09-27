#pragma once

#include <cstdint>

#ifdef _WIN32
#	include <windows.h>
#	include <memoryapi.h>
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#endif

namespace utils {
	namespace memory {
		template <typename t>
		__forceinline static t call_virtual(void *name, int offset) {
			int *table = *(int **)name;
			int address = table[offset];
			return (t)(address);
		}

		inline unsigned int get_virtual(void *_class, unsigned int index) {
			return static_cast<unsigned int>((*static_cast<int **>(_class))[index]);
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