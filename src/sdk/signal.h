#pragma once

#include <vector>

// member functions only for now (__thiscall conv etc.)
class signal_t {
	friend class c_signal;

private:
	void*                addr;
	void*                orig;
	void*                hook;
	std::vector< void* > callbacks;
	const char*          module_name;

public:
	virtual signal_t* in_module( const char* name );
	virtual signal_t* in_interface( const char* name );
	virtual signal_t* at_address( void* address );
	virtual signal_t* from_vtable( size_t index );
	virtual signal_t* from_pattern( const char* pattern );

	virtual signal_t* add_callback( void* fn );
	virtual signal_t* remove_callback( void* fn );

	virtual signal_t* enable( void* handler );
	virtual signal_t* disable( );

	std::vector< void* >& get_callbacks( ) { return callbacks; }
	void*                 get_address( ) { return addr; }
	void*                 get_original( ) { return orig; }
};

class c_signal {
public:
	virtual signal_t* create( const char* name );
	virtual void      remove( const char* name );
	virtual void      enable( signal_t* signal, void* handler );
	virtual void      disable( signal_t* signal );
	virtual signal_t* get( const char* name );
};

#define _SIGNAL0( t, name )                                                                                                                \
	static t __fastcall name##_handler( void* ecx, int edx ) {                                                                                \
		t    ret = 0;                                                                                                                            \
		auto sig = photon->signal->get( #name );                                                                                                 \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                        \
			using orig_fn_t = t( __rescall* )( void* );                                                                                             \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void* ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx ); \
		}                                                                                                                                        \
		return ret;                                                                                                                              \
	}
#define _SIGNAL1( t, name, a1_t, a1 )                                                                                                                \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1 ) {                                                                                 \
		t    ret = 0;                                                                                                                                      \
		auto sig = photon->signal->get( #name );                                                                                                           \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                  \
			using orig_fn_t = t( __rescall* )( void*, a1_t );                                                                                                 \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1 ); \
		}                                                                                                                                                  \
		return ret;                                                                                                                                        \
	}
#define _SIGNAL2( t, name, a1_t, a1, a2_t, a2 )                                                                                                                \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1, a2_t a2 ) {                                                                                  \
		t    ret = 0;                                                                                                                                                \
		auto sig = photon->signal->get( #name );                                                                                                                     \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                            \
			using orig_fn_t = t( __rescall* )( void*, a1_t, a2_t );                                                                                                     \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t, a2_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1, a2 ); \
		}                                                                                                                                                            \
		return ret;                                                                                                                                                  \
	}
#define _SIGNAL3( t, name, a1_t, a1, a2_t, a2, a3_t, a3 )                                                                                                                \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1, a2_t a2, a3_t a3 ) {                                                                                   \
		t    ret = 0;                                                                                                                                                          \
		auto sig = photon->signal->get( #name );                                                                                                                               \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                                      \
			using orig_fn_t = t( __rescall* )( void*, a1_t, a2_t, a3_t );                                                                                                         \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t, a2_t, a3_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1, a2, a3 ); \
		}                                                                                                                                                                      \
		return ret;                                                                                                                                                            \
	}
#define _SIGNAL4( t, name, a1_t, a1, a2_t, a2, a3_t, a3_, a4_t, a4 )                                                                                                               \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1, a2_t a2, a3_t a3, a4_t a4 ) {                                                                                    \
		t    ret = 0;                                                                                                                                                                    \
		auto sig = photon->signal->get( #name );                                                                                                                                         \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                                                \
			using orig_fn_t = t( __rescall* )( void*, a1_t, a2_t, a3_t, a4_t );                                                                                                             \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t, a2_t, a3_t, a4_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1, a2, a3, a4 ); \
		}                                                                                                                                                                                \
		return ret;                                                                                                                                                                      \
	}
#define _SIGNAL5( t, name, a1_t, a1, a2_t, a2, a3_t, a3_, a4_t, a4, a5_t, a5 )                                                                                                               \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1, a2_t a2, a3_t a3, a4_t a4, a5_t a5 ) {                                                                                     \
		t    ret = 0;                                                                                                                                                                              \
		auto sig = photon->signal->get( #name );                                                                                                                                                   \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                                                          \
			using orig_fn_t = t( __rescall* )( void*, a1_t, a2_t, a3_t, a4_t, a5_t );                                                                                                                 \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t, a2_t, a3_t, a4_t, a5_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1, a2, a3, a4, a5 ); \
		}                                                                                                                                                                                          \
		return ret;                                                                                                                                                                                \
	}
#define _SIGNAL6( t, name, a1_t, a1, a2_t, a2, a3_t, a3_, a4_t, a4, a5_t, a5, a6_t, a6 )                                                                                                               \
	static t __fastcall name##_handler( void* ecx, int edx, a1_t a1, a2_t a2, a3_t a3, a4_t a4, a5_t a5, a6_t a6 ) {                                                                                      \
		t    ret = 0;                                                                                                                                                                                        \
		auto sig = photon->signal->get( #name );                                                                                                                                                             \
		for ( const auto& cbk : sig->get_callbacks( ) ) {                                                                                                                                                    \
			using orig_fn_t = t( __rescall* )( void*, a1_t, a2_t, a3_t, a4_t, a5_t, a6_t );                                                                                                                     \
			ret             = reinterpret_cast< t ( * )( orig_fn_t, void*, a1_t, a2_t, a3_t, a4_t, a5_t, a6_t ) >( cbk )( reinterpret_cast< orig_fn_t >( sig->get_original( ) ), ecx, a1, a2, a3, a4, a5, a6 ); \
		}                                                                                                                                                                                                    \
		return ret;                                                                                                                                                                                          \
	}

#define _SIGNAL_CALLBACK0( t, name )                                                             t name##_cbk( t( __rescall* original )( void* ), void* ecx )
#define _SIGNAL_CALLBACK1( t, name, a1_t, a1 )                                                   t name##_cbk( t( __rescall* original )( void*, a1_t ), void* ecx, a1_t a1 )
#define _SIGNAL_CALLBACK2( t, name, a1_t, a1, a2_t, a2 )                                         t name##_cbk( t( __rescall* original )( void*, a1_t, a2_t ), void* ecx, a1_t a1, a2_t a2 )
#define _SIGNAL_CALLBACK3( t, name, a1_t, a1, a2_t, a2, a3_t, a3 )                               t name##_cbk( t( __rescall* original )( void*, a1_t, a2_t, a3_t ), void* ecx, a1_t a1, a2_t a2, a3_t a3 )
#define _SIGNAL_CALLBACK4( t, name, a1_t, a1, a2_t, a2, a3_t, a3, a4_t, a4 )                     t name##_cbk( t( __rescall* original )( void*, a1_t, a2_t, a3_t, a4_t ), void* ecx, a1_t a1, a2_t a2, a3_t a3, a4_t a4 )
#define _SIGNAL_CALLBACK5( t, name, a1_t, a1, a2_t, a2, a3_t, a3, a4_t, a4, a5_t, a5 )           t name##_cbk( t( __rescall* original )( void*, a1_t, a2_t, a3_t, a4_t, a5_t ), void* ecx, a1_t a1, a2_t a2, a3_t a3, a4_t a4, a5_t a5 )
#define _SIGNAL_CALLBACK6( t, name, a1_t, a1, a2_t, a2, a3_t, a3, a4_t, a4, a5_t, a5, a6_t, a6 ) t name##_cbk( t( __rescall* original )( void*, a1_t, a2_t, a3_t, a4_t, a5_t, a6_t ), void* ecx, a1_t a1, a2_t a2, a3_t a3, a4_t a4, a5_t a5, a6_t a6 )

#define _SIGNAL_NAME( _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, NAME, ... ) NAME

#define SIGNAL( t, name, ... )          _SIGNAL_NAME( _0 __VA_OPT__(, ) __VA_ARGS__, _SIGNAL6, _6, _SIGNAL5, _5, _SIGNAL4, _4, _SIGNAL3, _3, _SIGNAL2, _2, _SIGNAL1, _1, _SIGNAL0 )( t, name __VA_OPT__(, ) __VA_ARGS__ )
#define SIGNAL_CALLBACK( t, name, ... ) _SIGNAL_NAME( _0 __VA_OPT__(, ) __VA_ARGS__, _SIGNAL_CALLBACK6, _6, _SIGNAL_CALLBACK5, _5, _SIGNAL_CALLBACK4, _4, _SIGNAL_CALLBACK3, _3, _SIGNAL_CALLBACK2, _2, _SIGNAL_CALLBACK1, _1, _SIGNAL_CALLBACK0 )( t, name __VA_OPT__(, ) __VA_ARGS__ )
