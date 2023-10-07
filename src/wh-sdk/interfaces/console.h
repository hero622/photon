#pragma once

#include "../source_sdk.h"
#include "../utils.h"

class c_console {
public:
	void(__cdecl *msg)(const char *p_msg_format, ...);
	void(__cdecl *color_msg)(const sdk::color_t &clr, const char *p_msg_format, ...);
	void(__cdecl *warning)(const char *p_msg_format, ...);
	void(__cdecl *dev_msg)(const char *p_msg_format, ...);
	void(__cdecl *dev_warning)(const char *p_msg_format, ...);

public:
	c_console(void *ptr);
};