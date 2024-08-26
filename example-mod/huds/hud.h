#pragma once

#include <photon.h>

namespace huds {
	bool initialize( );
	void uninitialize( );
}  // namespace huds

class c_example_hud : public photon_api::i_hud {
	virtual void paint( );
};
