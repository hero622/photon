#pragma once

#include "util/util.h"

class c_game_rules {
public:
	bool is_multiplayer( ) {
		return util::call_virtual< OS( 33, 34 ), bool >( this );
	}
};
