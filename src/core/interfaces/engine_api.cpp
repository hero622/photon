#include "wh-sdk/interfaces/engine_api.h"

#include "../portal2.h"
#include "wh-sdk/utils.h"

c_engine_api::c_engine_api(void *ptr) {
	this->ptr = ptr;

	auto is_running_simulation = utils::memory::get_virtual(this->ptr, offsets::is_running_simulation);
	auto eng_addr = **reinterpret_cast<void ***>(is_running_simulation + offsets::eng);
	this->engine = reinterpret_cast<void *>(eng_addr);
}