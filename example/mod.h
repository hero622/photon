#pragma once

#include <wormhole_api.h>

class c_wormhole_mod : public api::i_wormhole_mod {
public:
	c_wormhole *wh;

	virtual bool load(c_wormhole *wormhole);
	virtual void unload();
	virtual void on_pre_tick();
	virtual void on_post_tick();
	virtual void on_pre_frame();
	virtual void on_post_frame();
	virtual void on_session_start();
	virtual void on_session_end();
};