#include "photon-sdk/hook.h"

#include "deps/subhook/subhook.h"
#include "photon-sdk/utils.h"

bool c_hook::create_hook( void *src, void *dst, void *&hook, void **orig ) {
	hook = subhook_new( src, dst, SUBHOOK_TRAMPOLINE );
	*orig = subhook_get_trampoline( ( subhook_t ) hook );
	utils::console::log( "[+] hook: %p -> %p, trampoline: %p.\n", src, dst, *orig );
	return subhook_install( ( subhook_t ) hook ) == 0;
}

bool c_hook::create_hook( void *src, void *dst, void *&hook, void **orig, const char *name ) {
	hook = subhook_new( src, dst, SUBHOOK_TRAMPOLINE );
	*orig = subhook_get_trampoline( ( subhook_t ) hook );
	utils::console::log( "[+] hook %s: %p -> %p, trampoline: %p.\n", name, src, dst, *orig );
	return subhook_install( ( subhook_t ) hook ) == 0;
}

bool c_hook::remove_hook( void *hook ) {
	auto result = subhook_remove( ( subhook_t ) hook ) == 0;
	subhook_free( ( subhook_t ) hook );
	return result;
}

bool c_hook::enable_hook( void *hook ) {
	return subhook_install( ( subhook_t ) hook ) == 0;
}

bool c_hook::disable_hook( void *hook ) {
	return subhook_remove( ( subhook_t ) hook ) == 0;
}
