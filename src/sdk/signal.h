#pragma once

#include "platform.h"

#include <vector>

enum e_data_type : uint8_t {
	Void,
	Bool,
	Int8,
	Uint8,
	Int16,
	Uint16,
	Int32,
	Uint32,
	Int64,
	Uint64,
	Float,
	Double,
	Pointer,
	String,
	Wstring,
	M128,
	M256,
	M512,
	Object
};

enum e_callconv : uint8_t {
	Cdecl,
	Stdcall,
	Thiscall
};

enum class e_callback_type : bool {
	Pre,
	Post
};

enum class e_return_action : int32_t {
	Ignored,   // handler didn't take any action
	Handled,   // we did something, but real function should still be called
	Override,  // call real function, but use my return value
	Supercede  // skip real function; use my return value
};

struct signal_params_t {  // this shadows dyno::IHook
	template < class T >
	T get_arg( size_t index ) {
		auto callconv = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 12 ] )( this );
		auto reg      = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 13 ] )( this );
		auto argptr   = ( ( void*( __rescall* ) ( void*, size_t, void* ) )( *( int** ) callconv )[ 3 ] )( callconv, index, reg );

		return *( T* ) argptr;
	}

	template < class T >
	void set_arg( size_t index, T value ) {
		auto callconv = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 12 ] )( this );
		auto reg      = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 13 ] )( this );
		auto argptr   = ( ( void*( __rescall* ) ( void*, size_t, void* ) )( *( int** ) callconv )[ 3 ] )( callconv, index, reg );

		*( T* ) argptr = value;

		( ( void( __rescall* )( void*, size_t, void*, void* ) )( *( int** ) callconv )[ 4 ] )( callconv, index, reg, argptr );
	}

	template < class T >
	T get_return( ) {
		auto callconv = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 12 ] )( this );
		auto reg      = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 13 ] )( this );
		auto retptr   = ( ( void*( __rescall* ) ( void*, void* ) )( *( int** ) callconv )[ 5 ] )( callconv, reg );

		return *( T* ) retptr;
	}

	template < class T >
	void set_return( T value ) {
		auto callconv = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 12 ] )( this );
		auto reg      = ( ( void*( __rescall* ) ( void* ) )( *( int** ) this )[ 13 ] )( this );
		auto retptr   = ( ( void*( __rescall* ) ( void*, void* ) )( *( int** ) callconv )[ 5 ] )( callconv, reg );

		*( T* ) retptr = value;

		( ( void( __rescall* )( void*, void*, void* ) )( *( int** ) callconv )[ 6 ] )( callconv, reg, retptr );
	}
};

class signal_t {
	friend class c_signal;

private:
	void*                      addr;
	e_data_type                return_type;
	e_callconv                 callconv;
	std::vector< e_data_type > params;

public:
	virtual signal_t* with_return_type( e_data_type type );
	virtual signal_t* with_callconv( e_callconv callconv );
	virtual signal_t* with_parameters( const std::vector< e_data_type >& params );

	virtual signal_t* in_module( const char* name );
	virtual signal_t* in_interface( const char* name );
	virtual signal_t* at_address( void* address );
	virtual signal_t* from_vtable( size_t index );
	virtual signal_t* from_pattern( const char* pattern );

	virtual signal_t* add_callback( e_callback_type type, void* fn );
	virtual signal_t* remove_callback( e_callback_type type, void* fn );

	virtual signal_t* enable( );
	virtual signal_t* disable( );
};

class c_signal {
public:
	virtual signal_t* create( const char* name );
	virtual void      remove( const char* name );
	virtual void      enable( signal_t* signal );
	virtual void      disable( signal_t* signal );
	virtual signal_t* get( const char* name );
	virtual void      remove_all( );
};
