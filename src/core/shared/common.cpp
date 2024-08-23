#include "sdk/common.h"

#include "core/interfaces/interfaces.h"
#include "core/mods/mods.h"
#include "util/util.h"

#include <stdarg.h>

void c_common::log( const char* fmt, ... ) {
	/* this sucks */
	va_list ap1, ap2;
	va_start( ap1, fmt );
	va_copy( ap2, ap1 );
	size_t sz = vsnprintf( NULL, 0, fmt, ap1 ) + 1;
	va_end( ap1 );
	char* buf = ( char* ) malloc( sz );
	vsnprintf( buf, sz, fmt, ap2 );
	va_end( ap2 );
	std::string str( buf );
	free( buf );
	interfaces::console->msg( str.c_str( ) );
}
void c_common::log( const color_t& clr, const char* fmt, ... ) {
	/* this sucks */
	va_list ap1, ap2;
	va_start( ap1, fmt );
	va_copy( ap2, ap1 );
	size_t sz = vsnprintf( NULL, 0, fmt, ap1 ) + 1;
	va_end( ap1 );
	char* buf = ( char* ) malloc( sz );
	vsnprintf( buf, sz, fmt, ap2 );
	va_end( ap2 );
	std::string str( buf );
	free( buf );
	interfaces::console->color_msg( clr, str.c_str( ) );
}
void c_common::log_warn( const char* fmt, ... ) {
	/* this sucks */
	va_list ap1, ap2;
	va_start( ap1, fmt );
	va_copy( ap2, ap1 );
	size_t sz = vsnprintf( NULL, 0, fmt, ap1 ) + 1;
	va_end( ap1 );
	char* buf = ( char* ) malloc( sz );
	vsnprintf( buf, sz, fmt, ap2 );
	va_end( ap2 );
	std::string str( buf );
	free( buf );
	interfaces::console->warning( str.c_str( ) );
}

void* c_common::get_interface( const char* module_name, const char* interface_name ) {
	using create_interface_fn = void* ( * ) ( const char*, int* );
	const auto fn             = util::get_sym_addr< create_interface_fn >( util::get_module_handle( module_name ), "CreateInterface" );

	if ( fn ) {
		void* result = nullptr;

		result = fn( interface_name, nullptr );

		if ( !result ) {
			util::console::log( "[!] couldn't find interface %s in %s.\n", interface_name, module_name );
			return nullptr;
		}

		util::console::log( "[+] found interface %s in %s at %p.\n", interface_name, module_name, result );

		return result;
	}

	util::console::log( "[!] couldn't find CreateInterface fn in %s.\n", module_name );

	return nullptr;
}

void c_common::post_event( void* sender, const char* msg ) {
	mods::post_event( sender, msg );
}

uint8_t* c_common::pattern_scan( const char* module_name, const char* signature ) {
	return util::pattern_scan( module_name, signature );
}

void* c_common::get_module_handle( const char* module_name ) {
	return util::get_module_handle( module_name );
}
