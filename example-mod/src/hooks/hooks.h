#pragma once

#include <photon.h>

namespace example_mod {
	namespace hooks {
		DECL_HK( void, calc_view_model_lag, vec3_t& origin, vec3_t& angles, vec3_t& original_angles );

		bool initialize( );
		void uninitialize( );
	}  // namespace hooks
}  // namespace example_mod
