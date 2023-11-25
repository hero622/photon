#include "hooks.h"

bool example_mod::hooks::initialize( ) {
	hk_inline( calc_view_model_lag, utils::memory::pattern_scan( module( "client" ), os( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 1C 56 6A 00", "56 53 83 EC 24 8B 74 24 30 8B 5C 24 34 6A 00" ) ) );

	return true;
}

void example_mod::hooks::uninitialize( ) {
	unhk( calc_view_model_lag );
}

hk_fn( void, example_mod::hooks::calc_view_model_lag, sdk::vec3_t &origin, sdk::vec3_t &angles, sdk::vec3_t &original_angles ) {
	return;
}
