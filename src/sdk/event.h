#pragma once

class c_event {
public:
	virtual void post( void* sender, const char* msg );  // Post events to all other mods, pass in the pointer to your i_photon_mod, message/event name as a C string.
};
