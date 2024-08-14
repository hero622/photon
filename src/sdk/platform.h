#pragma once

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#define MODULE( name ) name ".dll"

#define __rescall __thiscall

#define DLL_EXPORT extern "C" __declspec( dllexport )

#define DECL_HK( t, name, ... )                         \
	inline void* name##_hk;                                \
	inline t( __rescall* name )( void* ecx, __VA_ARGS__ ); \
	t __fastcall name##_hk_fn( void* ecx, int edx, __VA_ARGS__ )

#define HK_FN( t, name, ... ) \
	t __fastcall name##_hk_fn( void* ecx, int edx, __VA_ARGS__ )

#define OS( win, linux ) win
#else
#define MODULE( name ) name ".so"

#define __rescall  __attribute__( ( __cdecl__ ) )
#define __cdecl    __attribute__( ( __cdecl__ ) )
#define __fastcall __attribute__( ( __fastcall__ ) )

#define __forceinline inline __attribute__( ( always_inline ) )

#define DLL_EXPORT extern "C" __attribute__( ( visibility( "default" ) ) )

#define DECL_HK( t, name, ... )                           \
	inline void* name##_hk;                                  \
	inline t( __rescall* name )( void* ecx, ##__VA_ARGS__ ); \
	t __rescall name##_hk_fn( void* ecx, ##__VA_ARGS__ )

#define HK_FN( t, name, ... ) \
	t __rescall name##_hk_fn( void* ecx, ##__VA_ARGS__ )

#define OS( win, linux ) linux
#endif

#define DELETE_PTR( ptr ) \
	if ( ptr ) {             \
		delete ptr;             \
		ptr = nullptr;          \
	}

#define ARRAY_LEN( array ) ( sizeof( array ) / sizeof( *( array ) ) )
