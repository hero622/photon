#pragma once

#include "sdk/platform.h"

namespace math {
	constexpr float pi   = 3.1415926535897932384f;
	constexpr float pi_2 = pi * 2.f;

	__forceinline constexpr float deg_to_rad( float val ) {
		return val * ( pi / 180.f );
	}

	__forceinline constexpr float rad_to_deg( float val ) {
		return val * ( 180.f / pi );
	}
}  // namespace math
