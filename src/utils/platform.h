#pragma once

#ifdef _WIN32
#	define module(name) name ".dll"

#	define __rescall __thiscall

#	define dll_export extern "C" __declspec(dllexport)

#	define platform_def(name, win, linux) static constexpr const char *name = win;
#else
#	define module(name) name ".so"

#	define __rescall __attribute__((__cdecl__))
#	define __cdecl __attribute__((__cdecl__))
#	define __fastcall __attribute__((__fastcall__))

#	define dll_export extern "C" __attribute__((visibility("default")))

#	define platform_def(name, win, linux) static constexpr const char *name = linux;
#endif