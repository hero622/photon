#pragma once

#include <wormhole.h>

class c_example_hud : public wh_api::i_hud {
	virtual void paint( );
	virtual const char *get_name( );
};

class c_example_thud : public wh_api::i_thud {
	virtual const char *get_text( );
	virtual const char *get_name( );
};
