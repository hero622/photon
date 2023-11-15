#include "hooks.h"

#include "../portal2.h"

bool example_mod::hooks::initialize( ) {
	hk_addr( calc_view_model_lag, utils::memory::pattern_scan( module( "client" ), signatures::calc_view_model_lag ) );

	return true;
}

void example_mod::hooks::uninitialize( ) {
	unhk( calc_view_model_lag );
}

hk_fn( void, example_mod::hooks::calc_view_model_lag, sdk::vec3_t &origin, sdk::vec3_t &angles, sdk::vec3_t &original_angles ) {
	return;
}
