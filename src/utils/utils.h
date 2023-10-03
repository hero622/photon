#pragma once

#include "memory.h"
#include "platform.h"

#include <string>

namespace utils {
	std::string ssprintf(const char *fmt, ...);
}  // namespace utils

#define delete_ptr(ptr) \
	if (ptr) {             \
		delete ptr;           \
		ptr = nullptr;        \
	}