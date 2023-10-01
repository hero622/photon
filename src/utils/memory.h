#pragma once

#include <cstdint>

#ifdef _WIN32
#	include <windows.h>
#	include <memoryapi.h>
#else
#	include <dlfcn.h>
#	include <sys/mman.h>
#endif

#include "platform.h"

namespace utils {
	namespace memory {
		template <typename t, typename... args_t>
		__forceinline t call_virtual(size_t index, void *name, args_t... args) {
			using fn_t = t(__rescall *)(void *, args_t...);

			auto fn = (*reinterpret_cast<fn_t **>(name))[index];
			return fn(name, args...);
		}

		__forceinline unsigned int get_virtual(void *name, unsigned int index) {
			return static_cast<unsigned int>((*static_cast<int **>(name))[index]);
		}

		template <typename t>
		__forceinline t read(uintptr_t source) {
			auto rel = *reinterpret_cast<int *>(source);
			return (t)(source + rel + sizeof(rel));
		}

		template <typename t>
		__forceinline t get_sym_addr(void *module_handle, const char *sym) {
#ifdef _WIN32
			return (t)GetProcAddress((HMODULE)module_handle, sym);
#else
			return (t)dlsym(module_handle, sym);
#endif
		}

		__forceinline void *get_module_handle(const char *module_name) {
#ifdef _WIN32
			return GetModuleHandleA(module_name);
#else
			return dlopen(module_name, RTLD_NOLOAD | RTLD_NOW);
#endif
		}

		std::uint8_t *pattern_scan(const char *module_name, const char *signature) noexcept;
	}  // namespace memory
}  // namespace utils