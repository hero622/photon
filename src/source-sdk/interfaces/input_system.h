#pragma once

class i_input_system {
public:
	__forceinline void enable_input( bool enable ) {
		return util::call_virtual< 10, void >( this, enable );
	}
	__forceinline bool is_button_down( e_button_code code ) {
		return util::call_virtual< 14, bool >( this, code );
	}
	__forceinline const char* button_code_to_string( e_button_code code ) {
		return util::call_virtual< 29, const char* >( this, code );
	}
	__forceinline void get_cursor_position( int* x, int* y ) {
		return util::call_virtual< 45, void >( this, x, y );
	}
};

class i_input_stack_system {
public:
	__forceinline void set_cursor_visible( void* context, bool visible ) {
		return util::call_virtual< 11, void >( this, context, visible );
	}
};
