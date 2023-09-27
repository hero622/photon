#pragma once

#include "portal2.h"
#include "sdk/sdk.h"
#include "utils/utils.h"

class i_console {
public:
	void(__cdecl *msg)(const char *p_msg_format, ...);
	void(__cdecl *color_msg)(const sdk::color_t &clr, const char *p_msg_format, ...);
	void(__cdecl *warning)(const char *p_msg_format, ...);
	void(__cdecl *dev_msg)(const char *p_msg_format, ...);
	void(__cdecl *dev_warning)(const char *p_msg_format, ...);

	i_console(void *tier0) {
		this->msg = utils::memory::get_sym_addr<decltype(msg)>(tier0, symbols::i_console::msg);
		this->color_msg = utils::memory::get_sym_addr<decltype(color_msg)>(tier0, symbols::i_console::color_msg);
		this->warning = utils::memory::get_sym_addr<decltype(warning)>(tier0, symbols::i_console::warning);
		this->dev_msg = utils::memory::get_sym_addr<decltype(dev_msg)>(tier0, symbols::i_console::dev_msg);
		this->dev_warning = utils::memory::get_sym_addr<decltype(dev_warning)>(tier0, symbols::i_console::dev_warning);
	}
};