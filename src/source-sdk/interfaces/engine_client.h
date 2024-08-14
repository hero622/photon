#pragma once

#include "util/util.h"

class i_engine_client {
public:
	void cbuf_add_text( int target, const char* text, int tick_delay ) {
		auto client_cmd_addr = util::get_virtual< 7 >( this );
		util::read< void( __cdecl* )( int, const char*, int ) >( client_cmd_addr + OS( 0x2e, 0x23 ) )( target, text, tick_delay );
	}
	void client_cmd( const char* cmd_string ) {
		return util::call_virtual< 7, void >( this, cmd_string );
	}
	void execute_client_cmd( const char* cmd_string ) {
		return util::call_virtual< 104, void >( this, cmd_string );
	}
	int get_active_split_screen_slot( ) {
		return util::call_virtual< 127, int >( this );
	}
	void* get_input_context( int id ) {
		return util::call_virtual< 190, void* >( this, id );
	}

public:
	void cbuf_add( const char* text, int delay ) {
		auto target = get_active_split_screen_slot( );
		cbuf_add_text( target, text, delay );
	}
};
