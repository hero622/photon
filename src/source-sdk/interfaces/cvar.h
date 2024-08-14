#pragma once

#include "sdk/source_sdk.h"

class i_cvar {
public:
	void register_con_command( con_command_base* command_base ) {
		return util::call_virtual< 9, void >( this, command_base );
	}
	void unregister_con_command( con_command_base* command_base ) {
		return util::call_virtual< 10, void >( this, command_base );
	}
	con_command_base* find_command_base( const char* name ) {
		return util::call_virtual< 13, con_command_base* >( this, name );
	}
};
