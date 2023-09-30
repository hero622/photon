#pragma once

#include "mod.h"

namespace whmod_hooks {
	decl_hk(void, calc_view_model_lag, sdk::vec3_t &origin, sdk::vec3_t &angles, sdk::vec3_t &original_angles);

	bool initialize();
	void uninitialize();
}  // namespace whmod_hooks