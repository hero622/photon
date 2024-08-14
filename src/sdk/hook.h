#pragma once

#include "ccon.h"

class c_hook {
public:
	virtual bool create_hook( void* src, void* dst, void*& hook, void** orig );
	virtual bool create_hook( void* src, void* dst, void*& hook, void** orig, const char* name );  // For debugging purposes.
	virtual bool remove_hook( void* hook );
	virtual bool enable_hook( void* hook );
	virtual bool disable_hook( void* hook );
};

#ifdef _DEBUG
#define HK_VIRTUAL( vtable, name, off ) \
	photon->hook->create_hook( reinterpret_cast< void* >( ( *reinterpret_cast< int** >( vtable ) )[ off ] ), reinterpret_cast< void* >( name##_hk_fn ), name##_hk, reinterpret_cast< void** >( &name ), #name )

#define HK_INLINE( name, addr ) \
	photon->hook->create_hook( reinterpret_cast< void* >( addr ), reinterpret_cast< void* >( name##_hk_fn ), name##_hk, reinterpret_cast< void** >( &name ), #name )
#else
#define HK_VIRTUAL( vtable, name, off ) \
	photon->hook->create_hook( reinterpret_cast< void* >( ( *reinterpret_cast< int** >( vtable ) )[ off ] ), reinterpret_cast< void* >( name##_hk_fn ), name##_hk, reinterpret_cast< void** >( &name ) )

#define HK_INLINE( name, addr ) \
	photon->hook->create_hook( reinterpret_cast< void* >( addr ), reinterpret_cast< void* >( name##_hk_fn ), name##_hk, reinterpret_cast< void** >( &name ) )
#endif

#define UNHK( name ) \
	photon->hook->remove_hook( name##_hk )

#define HK_CMD( name ) \
	photon->con->hook_cmd( #name, name##_hk, name )

#define UNHK_CMD( name ) \
	photon->con->unhook_cmd( #name, name )
