#pragma once

class i_input_system {
public:
	void enable_input( bool enable ) {
		return util::call_virtual< 10, void >( this, enable );
	}
	bool is_button_down( button_code_t code ) {
		return util::call_virtual< 14, bool >( this, code );
	}
	void get_cursor_position( int* x, int* y ) {
		return util::call_virtual< 45, void >( this, x, y );
	}
};

class i_input_stack_system {
public:
	void set_cursor_visible( void* context, bool visible ) {
		return util::call_virtual< 11, void >( this, context, visible );
	}
};
