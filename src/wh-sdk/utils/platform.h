#pragma once

#ifndef _WIN32
#	include <unistd.h>
#endif

#ifdef _WIN32
#	define module(name) name ".dll"

#	define __rescall __thiscall

#	define dll_export extern "C" __declspec(dllexport)

#	define decl_hk(t, name, ...)                                  \
		static inline void *name##_hk;                                \
		static inline t(__rescall *name)(void *thisptr, __VA_ARGS__); \
		static t __fastcall name##_hk_fn(void *thisptr, int edx, __VA_ARGS__)

#	define hk_fn(t, name, ...) \
		t __fastcall name##_hk_fn(void *thisptr, int edx, __VA_ARGS__)

#	define sleep(ms) Sleep(ms)

#	define plat_module(name, win, linux) inline const char *name = win
#	define offset(name, win, linux) inline int name = win
#	define signature(name, win, linux) inline const char *name = win
#	define symbol(name, win, linux) inline const char *name = win
#else
#	define module(name) name ".so"

#	define __rescall __attribute__((__cdecl__))
#	define __cdecl __attribute__((__cdecl__))
#	define __fastcall __attribute__((__fastcall__))

#	define __forceinline inline __attribute__((always_inline))

#	define dll_export extern "C" __attribute__((visibility("default")))

#	define decl_hk(t, name, ...)                                    \
		static inline void *name##_hk;                                  \
		static inline t(__rescall *name)(void *thisptr, ##__VA_ARGS__); \
		static t __rescall name##_hk_fn(void *thisptr, ##__VA_ARGS__)

#	define hk_fn(t, name, ...) \
		t __rescall name##_hk_fn(void *thisptr, ##__VA_ARGS__)

#	define sleep(ms) usleep((ms) * 1000)

#	define plat_module(name, win, linux) inline const char *name = linux
#	define offset(name, win, linux) inline int name = linux
#	define signature(name, win, linux) inline const char *name = linux
#	define symbol(name, win, linux) inline const char *name = linux
#endif