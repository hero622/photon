#include "wormhole-sdk/interfaces/console.h"

#include "../portal2.h"
#include "wormhole-sdk/utils.h"

c_console::c_console(void *ptr) {
	this->msg = utils::memory::get_sym_addr<decltype(msg)>(ptr, symbols::msg);
	this->color_msg = utils::memory::get_sym_addr<decltype(color_msg)>(ptr, symbols::color_msg);
	this->warning = utils::memory::get_sym_addr<decltype(warning)>(ptr, symbols::warning);
	this->dev_msg = utils::memory::get_sym_addr<decltype(dev_msg)>(ptr, symbols::dev_msg);
	this->dev_warning = utils::memory::get_sym_addr<decltype(dev_warning)>(ptr, symbols::dev_warning);
}