#include "sdk/interfaces.h"
#include "util/util.h"

void* c_portal2::get_interface( const char* module_name, const char* interface_name ) {
	using create_interface_fn = void* ( * ) ( const char*, int* );
	const auto fn             = util::get_sym_addr< create_interface_fn >( util::get_module_handle( module_name ), "CreateInterface" );

	if ( fn ) {
		void* result = nullptr;

		result = fn( interface_name, nullptr );

		if ( !result )
			return nullptr;

		util::console::log( "[+] found interface %s in %s at %p.\n", interface_name, module_name, result );

		return result;
	}

	util::console::log( "[!] couldn't find interface %s in %s.\n", interface_name, module_name );

	return nullptr;
}
