#pragma once

#include "memory.h"
#include "platform.h"
#include "string.h"

namespace utils {}  // namespace utils

#define delete_ptr(ptr) \
	if (ptr) {             \
		delete ptr;           \
		ptr = nullptr;        \
	}