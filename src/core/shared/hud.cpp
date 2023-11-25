#include "wormhole-sdk/hud.h"

#include "core/huds/huds.h"
#include "wormhole-sdk/wormhole.h"

#include <cstddef>

void c_hud::reg( wh_api::i_hud *hud ) {
	huds::huds.push_back( hud );
}
void c_hud::reg( wh_api::i_thud *thud ) {
	huds::thuds.push_back( thud );
}
void c_hud::unreg( wh_api::i_hud *hud ) {
	for ( std::size_t i = 0; i < huds::huds.size( ); ++i ) {
		if ( huds::huds[ i ] == hud )
			huds::huds.erase( huds::huds.begin( ) + i );
	}
}
void c_hud::unreg( wh_api::i_thud *thud ) {
	for ( std::size_t i = 0; i < huds::thuds.size( ); ++i ) {
		if ( huds::thuds[ i ] == thud )
			huds::thuds.erase( huds::thuds.begin( ) + i );
	}
}
