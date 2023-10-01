#pragma once

#include "console.h"
#include "memory.h"
#include "platform.h"

namespace utils {
	std::string ssprintf(const char *fmt, ...);
}  // namespace utils

#define delete_ptr(ptr) \
	if (ptr) {             \
		delete ptr;           \
		ptr = nullptr;        \
	}