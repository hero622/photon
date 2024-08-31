#pragma once

#include "sdk/source_sdk.h"

class c_demo_player {
public:
	bool is_playing_back( ) {
		return util::call_virtual< OS( 6, 7 ), bool >( this );
	}
};
