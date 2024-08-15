#pragma once

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#define MODULE( name ) name ".dll"

#define __rescall __thiscall

#define DLL_EXPORT extern "C" __declspec( dllexport )

#define OS( win, linux ) win
#else
#define MODULE( name ) name ".so"

#define __rescall  __attribute__( ( __cdecl__ ) )
#define __cdecl    __attribute__( ( __cdecl__ ) )
#define __fastcall __attribute__( ( __fastcall__ ) )

#define __forceinline inline __attribute__( ( always_inline ) )

#define DLL_EXPORT extern "C" __attribute__( ( visibility( "default" ) ) )

#define OS( win, linux ) linux
#endif

#define DELETE_PTR( ptr ) \
	if ( ptr ) {             \
		delete ptr;             \
		ptr = nullptr;          \
	}

#define ARRAY_LEN( array ) ( sizeof( array ) / sizeof( *( array ) ) )
