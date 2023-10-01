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

		__forceinline void unprotect(void *addr, size_t len) {
#ifdef _WIN32
			DWORD old_prot;
			VirtualProtect(addr, len, PAGE_EXECUTE_READWRITE, &old_prot);
#else
			uintptr_t start_page = (uintptr_t)addr & 0xFFFFF000;
			uintptr_t end_page = ((uintptr_t)addr + len) & 0xFFFFF000;
			uintptr_t page_len = end_page - start_page + 0x1000;
			mprotect((void *)start_page, page_len, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif
		}

		__forceinline void *reserve(void *addr, size_t len) {
#ifdef _WIN32
			return VirtualAlloc(0, len, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
			uintptr_t start_page = (uintptr_t)addr & 0xFFFFF000;
			uintptr_t end_page = ((uintptr_t)addr + len) & 0xFFFFF000;
			uintptr_t page_len = end_page - start_page + 0x1000;
			void *result = mmap((void *)start_page, page_len, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
			mprotect((void *)start_page, page_len, PROT_READ | PROT_WRITE | PROT_EXEC);
			return result;
#endif
		}

		bool detour(char *src, char *dst, const uintptr_t len);
		char *trampoline_hk(char *src, char *dst, const uintptr_t len);

		std::uint8_t *pattern_scan(const char *module_name, const char *signature) noexcept;
	}  // namespace memory
}  // namespace utils