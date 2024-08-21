// clang-format off
#include "sdk/photon.h"
#include "util/util.h"
#include "utest.h"
// clang-format on

namespace dummy {
	static int __cdecl add( int a, int b ) {
		return a + b;
	}

	class math {
	public:
		virtual int subtract( int a, int b ) {
			return a - b;
		}
	};
}  // namespace dummy

SIGNAL( int, __cdecl, add, int, a, int, b );
SIGNAL( int, __rescall, subtract, int, a, int, b );

SIGNAL_CALLBACK( int, __cdecl, add, int, a, int, b ) {
	return original( a, b ) + 5;
}
SIGNAL_CALLBACK( int, __rescall, subtract, int, a, int, b ) {
	return original( ecx, a, b ) + 5;
}

dummy::math* maff;

UTEST( hook, signal_create ) {
	// instantiate dummy class
	maff = new dummy::math( );

	// test before hook
	ASSERT_EQ( dummy::add( 5, 5 ), 10 );
	ASSERT_EQ( maff->subtract( 5, 5 ), 0 );

	// signal for static fn
	auto add_signal = photon->signal->create( "add" )->at_address( &dummy::add )->enable( &add_handler );

	ASSERT_NE( add_signal, nullptr );

	// signal for virtual method
	auto subtract_signal = photon->signal->create( "subtract" )->at_address( maff )->from_vtable( 0 )->enable( &subtract_handler );

	ASSERT_NE( subtract_signal, nullptr );
}

UTEST( hook, signal_callback ) {
	// signal for static fn
	photon->signal->get( "add" )->add_callback( &add_cbk );

	ASSERT_EQ( dummy::add( 5, 5 ), 15 );

	// signal for virtual method
	photon->signal->get( "subtract" )->add_callback( &subtract_cbk );

	ASSERT_EQ( maff->subtract( 5, 5 ), 5 );
}

UTEST( hook, signal_remove ) {
	// unhook functions
	photon->signal->remove( "add" );
	photon->signal->remove( "subtract" );

	// test after unhook
	ASSERT_EQ( dummy::add( 5, 5 ), 10 );
	ASSERT_EQ( maff->subtract( 5, 5 ), 0 );
}
