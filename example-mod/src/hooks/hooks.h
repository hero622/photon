#pragma once

#include <photon.h>

namespace example_mod {
	namespace hooks {
		decl_hk( void, calc_view_model_lag, sdk::vec3_t &origin, sdk::vec3_t &angles, sdk::vec3_t &original_angles );

		bool initialize( );
		void uninitialize( );
	}  // namespace hooks
}  // namespace example_mod
