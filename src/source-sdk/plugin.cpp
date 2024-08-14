#include "plugin.h"

#include "util/util.h"

#include <cstring>
#include <iostream>

interface_reg_t* interface_reg_t::interface_regs = nullptr;

DLL_EXPORT void* CreateInterface( const char* name, int* return_code ) {
	interface_reg_t* cur;

	for ( cur = interface_reg_t::interface_regs; cur; cur = cur->next ) {
		if ( !std::strcmp( cur->name, name ) ) {
			if ( return_code ) {
				*return_code = 0;
			}
			return cur->create_fn( );
		}
	}

	if ( return_code ) {
		*return_code = 1;
	}
	return nullptr;
}
