#pragma once

#include "../utils.h"

class c_engine_client {
public:
	void *ptr;

	void(__cdecl *cbuf_add_text)(int e_target, const char *p_text, int n_tick_delay);

	virtual void client_cmd(const char *sz_cmd_string);
	virtual void execute_client_cmd(const char *sz_cmd_string);
	virtual int get_active_split_screen_slot();

public:
	void *client_state;

	virtual void cbuf_add(const char *text, int delay);
	virtual void safe_unload();

	c_engine_client(void *ptr);
};