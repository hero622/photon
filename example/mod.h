#pragma once

#include <wormhole_api.h>

class c_wormhole_mod : public api::i_wormhole_mod {
public:
	c_wormhole *wormhole;

	virtual bool load(c_wormhole *wormhole);
	virtual void unload();
};