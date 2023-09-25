#include "console.h"

#ifdef _WIN32
#	include <Windows.h>
#endif

void utils::console::alloc() {
#ifdef _WIN32
	AllocConsole();

	freopen_s(reinterpret_cast<_iobuf **>(__acrt_iob_func(0)), "conin$", "r", static_cast<_iobuf *>(__acrt_iob_func(0)));
	freopen_s(reinterpret_cast<_iobuf **>(__acrt_iob_func(1)), "conout$", "w", static_cast<_iobuf *>(__acrt_iob_func(1)));
	freopen_s(reinterpret_cast<_iobuf **>(__acrt_iob_func(2)), "conout$", "w", static_cast<_iobuf *>(__acrt_iob_func(2)));
#endif
}

void utils::console::free() {
#ifdef _WIN32
	fclose(static_cast<_iobuf *>(__acrt_iob_func(0)));
	fclose(static_cast<_iobuf *>(__acrt_iob_func(1)));
	fclose(static_cast<_iobuf *>(__acrt_iob_func(2)));

	FreeConsole();
#endif
}