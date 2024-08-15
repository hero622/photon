#include "signals.h"

SIGNAL( int, calc_view_model_lag, vec3_t&, origin, vec3_t&, angles, vec3_t&, original_angles );

SIGNAL_CALLBACK( void, calc_view_model_lag, vec3_t&, origin, vec3_t&, angles, vec3_t&, original_angles ) {
}

bool signals::initialize( ) {
	photon->signal->create( "calc_view_model_lag" )->in_module( MODULE( "client" ) )->from_pattern( OS( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 1C 56 6A 00", "56 53 83 EC 24 8B 74 24 30 8B 5C 24 34 6A 00" ) )->enable( &calc_view_model_lag_handler );
	photon->signal->get( "calc_view_model_lag" )->add_callback( &calc_view_model_lag_cbk );

	return true;
}

void signals::uninitialize( ) {
	photon->signal->get( "calc_view_model_lag" )->remove_callback( &calc_view_model_lag_cbk );
	photon->signal->remove( "calc_view_model_lag" );
}
