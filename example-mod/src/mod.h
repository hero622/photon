#pragma once

#include <wormhole.h>

class c_wormhole_mod : public wh_api::i_wormhole_mod {
public:
	virtual bool load( wh_api::c_shared *wh );
	virtual void unload( );
	virtual void on_event( const char *msg );
	virtual wh_api::wh_mod_info_t *get_info( );
	virtual void paint_menu( );
};

extern c_wormhole_mod mod;
