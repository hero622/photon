#include "plugin.h"

#include "photon-sdk/utils.h"

#include <cstring>
#include <iostream>

sdk::interface_reg_t *sdk::interface_reg_t::interface_regs = nullptr;

dll_export void *CreateInterface( const char *name, int *return_code ) {
	sdk::interface_reg_t *cur;

	for ( cur = sdk::interface_reg_t::interface_regs; cur; cur = cur->next ) {
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
