// clang-format off
#include "sdk/photon.h"
#include "util/util.h"
#include "utest.h"
// clang-format on

/*
 * NOTE: linux asm is written using intel syntax, use compiler flag "-masm=intel"
 */

namespace dummy {
#ifdef _WIN32
	__declspec( naked ) int square( int num ) {
		__asm push ebp;                        // 55
		__asm mov  ebp, esp;                   // 8b ec
		__asm mov eax, dword ptr[ebp + 0x8];   // 8b 45 08
		__asm imul eax, dword ptr[ebp + 0x8];  // 0f af 45 08
		__asm pop ebp;                         // 5d
		__asm ret;                             // c3
	}
#else
#define __asm __asm__ __volatile__
	__attribute__( ( naked ) ) int square( int num ) {
		__asm( "push rbp" );                       // 55
		__asm( "mov rbp, rsp" );                   // 48 89 e5
		__asm( "mov dword ptr[rbp - 0x4], edi" );  // 89 7d fc
		__asm( "mov eax, dword ptr[rbp - 0x4]" );  // 8b 45 fc
		__asm( "imul eax, eax" );                  // 0f af c0
		__asm( "pop rbp" );                        // 5d
		__asm( "ret" );                            // c3
	}
#undef __asm
#endif
}  // namespace dummy

UTEST( memory, pattern_scan ) {
	// make sure the asm actually works lol
	ASSERT_EQ( dummy::square( 2 ), 4 );

	const uint8_t* square_addr = util::pattern_scan( OS( "photon.tests.exe", "photon.tests" ), OS( "55 8b ec 8b 45 08 0f af 45 08 5d c3", "55 48 80 e5 89 7d fc 8b 45 fc 0f af c0 5d c3" ) );

	// check if its nullptr
	ASSERT_NE( square_addr, nullptr );

	// check if bytes match
	ASSERT_EQ( square_addr[ 0 ], 0x55 );
	ASSERT_EQ( square_addr[ 1 ], OS( 0x8b, 0x48 ) );
	ASSERT_EQ( square_addr[ 2 ], OS( 0xec, 0x80 ) );
	ASSERT_EQ( square_addr[ 3 ], OS( 0x8b, 0xe5 ) );

	// call function just to make sure
	const auto square_fn = reinterpret_cast< int ( * )( int ) >( square_addr );
	ASSERT_EQ( square_fn( 2 ), 4 );
}
