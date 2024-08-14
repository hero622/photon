#pragma once

#include "sdk/source_sdk.h"
#include "util/util.h"

class c_console {
public:
	void( __cdecl* msg )( const char* msg_format, ... );
	void( __cdecl* color_msg )( const color_t& clr, const char* msg_format, ... );
	void( __cdecl* warning )( const char* msg_format, ... );
	void( __cdecl* dev_msg )( const char* msg_format, ... );
	void( __cdecl* dev_warning )( const char* msg_format, ... );

public:
	c_console( void* ptr ) {
		msg         = util::get_sym_addr< decltype( msg ) >( ptr, OS( "Msg", "Msg" ) );
		color_msg   = util::get_sym_addr< decltype( color_msg ) >( ptr, OS( "?ConColorMsg@@YAXABVColor@@PBDZZ", "_Z11ConColorMsgRK5ColorPKcz" ) );
		warning     = util::get_sym_addr< decltype( warning ) >( ptr, OS( "Warning", "Warning" ) );
		dev_msg     = util::get_sym_addr< decltype( dev_msg ) >( ptr, OS( "?DevMsg@@YAXPBDZZ", "_Z6DevMsgPKcz" ) );
		dev_warning = util::get_sym_addr< decltype( dev_warning ) >( ptr, OS( "?DevWarning@@YAXPBDZZ", "_Z10DevWarningPKcz" ) );
	}
};
