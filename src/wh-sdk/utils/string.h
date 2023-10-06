#pragma once

#include <string>

namespace utils {
	namespace string {
		std::string ssprintf(const char *fmt, ...);
		char *stristr(const char *str1, const char *str2);
	}  // namespace string
}  // namespace utils