#include "dx9.h"

#include "sdk/photon.h"

#ifdef _WIN32
#include <d3d9.h>

SIGNAL_CALLBACK( long, __stdcall, d3d9_reset, IDirect3DDevice9*, device, D3DPRESENT_PARAMETERS*, presentation_parameters ) {
	return original( device, presentation_parameters );
}
SIGNAL_CALLBACK( long, __stdcall, d3d9_present, IDirect3DDevice9*, device, const RECT*, source_rect, const RECT*, dest_rect, HWND, dest_window_override, const RGNDATA*, dirty_region ) {
	return original( device, source_rect, dest_rect, dest_window_override, dirty_region );
}
#endif

bool dx9::initialize( ) {
#ifdef _WIN32
	photon->signal->get( "d3d9_reset" )->add_callback( &d3d9_reset_cbk );
	photon->signal->get( "d3d9_present" )->add_callback( &d3d9_present_cbk );
#endif

	return true;
}

void dx9::uninitialize( ) {
}
