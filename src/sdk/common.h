#pragma once

#include "source_sdk.h"

class c_common {
public:
	virtual void     log( const char* fmt, ... );
	virtual void     log( const color_t& clr, const char* fmt, ... );
	virtual void     log_warn( const char* fmt, ... );
	virtual void*    get_interface( const char* module_name, const char* interface_name );
	virtual void     post_event( void* sender, const char* msg );
	virtual uint8_t* pattern_scan( const char* module_name, const char* signature );
	virtual void*    get_module_handle( const char* module_name );
};
