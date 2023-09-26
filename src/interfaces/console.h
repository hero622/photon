#pragma once

#include "sdk/sdk.h"

class i_console {
public:
	void(__cdecl *msg)(const char *p_msg_format, ...);
	void(__cdecl *color_msg)(const sdk::color_t &clr, const char *p_msg_format, ...);
	void(__cdecl *warning)(const char *p_msg_format, ...);
	void(__cdecl *dev_msg)(const char *p_msg_format, ...);
	void(__cdecl *dev_warning)(const char *p_msg_format, ...);
};