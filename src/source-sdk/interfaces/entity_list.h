#pragma once

#include "util/util.h"

class i_client_entity_list {
public:
	__forceinline void* get_client_entity( int entnum ) {
		return util::call_virtual< 3, void* >( this, entnum );
	}
};
