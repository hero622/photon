#pragma once

#include "sdk/source_sdk.h"

class i_scheme {
public:
	__forceinline h_font get_font( const char* font_name, bool proportional = false ) {
		return util::call_virtual< OS( 3, 4 ), h_font >( this, font_name, proportional );
	}
};

class i_scheme_manager {
public:
	__forceinline i_scheme* get_i_scheme( unsigned long scheme ) {
		return util::call_virtual< OS( 8, 9 ), i_scheme* >( this, scheme );
	}
};
