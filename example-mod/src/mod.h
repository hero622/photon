#pragma once

#include <photon.h>

class c_photon_mod : public photon_api::i_photon_mod {
public:
	virtual bool load( photon_api::c_shared *photon );
	virtual void unload( );
	virtual void on_event( const char *msg );
	virtual photon_api::mod_info_t get_info( );
	virtual void paint_menu( );
};

extern c_photon_mod mod;
