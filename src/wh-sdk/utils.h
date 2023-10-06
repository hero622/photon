#pragma once

namespace utils {}

#include "utils/memory.h"
#include "utils/platform.h"
#include "utils/string.h"

#define delete_ptr(ptr) \
	if (ptr) {             \
		delete ptr;           \
		ptr = nullptr;        \
	}