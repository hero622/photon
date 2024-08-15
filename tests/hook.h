// // clang-format off
// #include "sdk/photon.h"
// #include "util/util.h"
// #include "utest.h"
// // clang-format on

// namespace dummy {
// 	static int add( int a, int b ) {
// 		return a + b;
// 	}

// 	class math {
// 	public:
// 		virtual int subtract( int a, int b ) {
// 			return a - b;
// 		}
// 	};
// }  // namespace dummy

// /*
//  * inspired by SST lol
//  */

// inline void* add_hk;
// inline int ( *add )( int a, int b );
// int add_hk_fn( int a, int b ) {
// 	return add( a, b ) + 5;
// }

// DECL_HK( int, subtract, int a, int b );
// HK_FN( int, subtract, int a, int b ) {
// 	return subtract( ecx, a, b ) + 5;
// }

// dummy::math* math;

// UTEST( hook, inline_hook ) {
// 	// test before hook
// 	ASSERT_EQ( dummy::add( 5, 5 ), 10 );

// 	// hook inline
// 	HK_INLINE( add, &dummy::add );

// 	// test after hook
// 	ASSERT_EQ( dummy::add( 5, 5 ), 15 );
// }

// /*
//  * NOTE: this is kinda unnecesarry since its technically still an inline hook
//  * but that might change in the future so might aswell have it here
//  */
// UTEST( hook, virtual_hook ) {
// 	// instantiate dummy class
// 	math = new dummy::math( );

// 	// test before hook
// 	ASSERT_EQ( math->subtract( 5, 5 ), 0 );

// 	// hook virtual
// 	HK_VIRTUAL( math, subtract, 0 );

// 	// test after hook
// 	ASSERT_EQ( math->subtract( 5, 5 ), 5 );
// }

// UTEST( hook, unhook ) {
// 	// test before unhook
// 	ASSERT_EQ( dummy::add( 5, 5 ), 15 );
// 	ASSERT_EQ( math->subtract( 5, 5 ), 5 );

// 	// unhook functions
// 	UNHK( subtract );
// 	UNHK( add );

// 	// test after unhook
// 	ASSERT_EQ( dummy::add( 5, 5 ), 10 );
// 	ASSERT_EQ( math->subtract( 5, 5 ), 0 );
// }
