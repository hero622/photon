#pragma once

#include "sdk/signal.h"

#include "sdk/photon.h"

#include <subhook/subhook.h>
#include <unordered_map>

static std::unordered_map< std::string, signal_t* > signals;

signal_t* signal_t::in_module( const char* name ) {
	module_name = name;
	addr        = photon->common->get_module_handle( module_name );
	return this;
}
signal_t* signal_t::in_interface( const char* name ) {
	addr = photon->common->get_interface( module_name, name );
	return this;
}
signal_t* signal_t::at_address( void* address ) {
	addr = address;
	return this;
}
signal_t* signal_t::from_vtable( size_t index ) {
	addr = reinterpret_cast< void* >( ( *reinterpret_cast< int** >( addr ) )[ index ] );
	return this;
}
signal_t* signal_t::from_pattern( const char* pattern ) {
	addr = photon->common->pattern_scan( module_name, pattern );
	return this;
}
signal_t* signal_t::add_callback( void* fn ) {
	callbacks.push_back( fn );
	return this;
}
signal_t* signal_t::remove_callback( void* fn ) {
	auto it = std::find( callbacks.begin( ), callbacks.end( ), fn );
	if ( it != callbacks.end( ) )
		callbacks.erase( it );
	return this;
}
signal_t* signal_t::enable( void* handler ) {
	photon->signal->enable( this, handler );
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
	signals.erase( name );
}
void c_signal::enable( signal_t* signal, void* handler ) {
	auto sig  = signal;
	sig->hook = subhook_new( sig->get_address( ), handler, SUBHOOK_TRAMPOLINE );
	sig->orig = subhook_get_trampoline( reinterpret_cast< subhook_t >( signal->hook ) );
	subhook_install( reinterpret_cast< subhook_t >( signal->hook ) );
}
void c_signal::disable( signal_t* signal ) {
	subhook_remove( reinterpret_cast< subhook_t >( signal->hook ) );
	subhook_free( reinterpret_cast< subhook_t >( signal->hook ) );
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
