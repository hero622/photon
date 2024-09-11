#pragma once

#include "sdk/signal.h"

#include "sdk/photon.h"

#include <dynohook/conventions/x86_ms_thiscall.h>
#include <dynohook/manager.h>
#include <unordered_map>

static std::unordered_map< std::string, signal_t* > signals;

static const char* module_name;

signal_t* signal_t::with_return_type( e_data_type type ) {
	this->return_type = type;
	return this;
}
signal_t* signal_t::with_callconv( e_callconv callconv ) {
	this->callconv = callconv;
	return this;
}
signal_t* signal_t::with_parameters( const std::vector< e_data_type >& params ) {
	this->params = params;
	return this;
}
signal_t* signal_t::in_module( const char* name ) {
	module_name = name;
	this->addr  = photon->common->get_module_handle( module_name );
	return this;
}
signal_t* signal_t::in_interface( const char* name ) {
	this->addr = photon->common->get_interface( module_name, name );
	return this;
}
signal_t* signal_t::at_address( void* address ) {
	this->addr = address;
	return this;
}
signal_t* signal_t::from_vtable( size_t index ) {
	this->addr     = reinterpret_cast< void* >( ( *reinterpret_cast< int** >( addr ) )[ index ] );
	this->callconv = Thiscall;
	return this;
}
signal_t* signal_t::from_pattern( const char* pattern ) {
	this->addr = photon->common->pattern_scan( module_name, pattern );
	return this;
}
signal_t* signal_t::add_callback( e_callback_type type, void* fn ) {
	dyno::HookManager::Get( ).findDetour( addr )->addCallback( ( dyno::CallbackType ) type, ( dyno::CallbackHandler ) fn );
	return this;
}
signal_t* signal_t::remove_callback( e_callback_type type, void* fn ) {
	dyno::HookManager::Get( ).findDetour( addr )->removeCallback( ( dyno::CallbackType ) type, ( dyno::CallbackHandler ) fn );
	return this;
}
signal_t* signal_t::enable( ) {
	photon->signal->enable( this );
	return this;
}
signal_t* signal_t::disable( ) {
	photon->signal->disable( this );
	return this;
}

signal_t* c_signal::create( const char* name ) {
	auto signal = new signal_t( );
	signals.insert( std::make_pair( std::string( name ), signal ) );
	return signal;
}
void c_signal::remove( const char* name ) {
	get( name )->disable( );
	delete get( name );
	signals.erase( name );
}
void c_signal::enable( signal_t* signal ) {
	std::vector< dyno::DataObject > params;

	dyno::ConvFunc fn;
	std::transform( signal->params.begin( ), signal->params.end( ), std::back_inserter( params ), []( e_data_type x ) { return ( dyno::DataType ) x; } );
	switch ( signal->callconv ) {
	case Cdecl:
		fn = [ & ]( ) { return new dyno::x86MsCdecl( params, ( dyno::DataType ) signal->return_type ); };
		break;
	case Thiscall:
		fn = [ & ]( ) { return new dyno::x86MsThiscall( params, ( dyno::DataType ) signal->return_type ); };
		break;
	case Stdcall:
		fn = [ & ]( ) { return new dyno::x86MsStdcall( params, ( dyno::DataType ) signal->return_type ); };
		break;
	};

	dyno::HookManager::Get( ).hookDetour( signal->addr, fn );
}
void c_signal::disable( signal_t* signal ) {
	dyno::HookManager::Get( ).findDetour( signal->addr )->unhook( );
}
signal_t* c_signal::get( const char* name ) {
	return signals[ name ];
}
void c_signal::remove_all( ) {
	for ( auto& signal : signals ) {
		signal.second->disable( );
	}
	signals.clear( );
}
