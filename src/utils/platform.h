#pragma once

#ifdef _WIN32
#	define module(name) name ".dll"

#	define __rescall __thiscall

#	define dll_export extern "C" __declspec(dllexport)

#	define decl_hk(t, name, ...)                                  \
		static inline t(__rescall *name)(void *thisptr, __VA_ARGS__); \
		static t __fastcall name##_hk(void *thisptr, int edx, __VA_ARGS__)

#	define hk_fn(t, name, ...) \
		t __fastcall name##_hk(void *thisptr, int edx, __VA_ARGS__)
#else
#	define module(name) name ".so"

#	define __rescall __attribute__((__cdecl__))
#	define __cdecl __attribute__((__cdecl__))
#	define __fastcall __attribute__((__fastcall__))

#	define dll_export extern "C" __attribute__((visibility("default")))
#endif