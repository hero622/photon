#pragma once

#include "util/util.h"

class i_mem_alloc {
public:
	__forceinline void* alloc( size_t size ) {
		return util::call_virtual< OS( 1, 0 ), void* >( this, size );
	}
	__forceinline void* realloc( void* mem, size_t size ) {
		return util::call_virtual< OS( 3, 1 ), void* >( this, mem, size );
	}
	__forceinline void free( void* mem ) {
		return util::call_virtual< OS( 5, 2 ), void >( this, mem );
	}
};
