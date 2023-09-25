#pragma once

#include <iostream>

namespace utils {
	namespace console {
		void alloc();
		void release();

		template <typename... Args>
		void log(const char *fmt, const Args &...args) {
			printf(fmt, args...);
		}
	}  // namespace console
}  // namespace utils