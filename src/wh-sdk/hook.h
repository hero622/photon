#pragma once

#include "convar.h"
#include "utils.h"

class c_hook {
public:
	virtual int create_hook(void *src, void *dst, void *&hook, void **orig);
	virtual int remove_hook(void *hook);
	virtual int enable_hook(void *hook);
	virtual int disable_hook(void *hook);
};

#define hk_virtual(vtable, name, off) \
	shared->hook->create_hook(reinterpret_cast<void *>(utils::memory::get_virtual(vtable, off)), reinterpret_cast<void *>(name##_hk_fn), name##_hk, reinterpret_cast<void **>(&name));

#define hk_addr(name, addr) \
	shared->hook->create_hook(reinterpret_cast<void *>(addr), reinterpret_cast<void *>(name##_hk_fn), name##_hk, reinterpret_cast<void **>(&name));

#define unhk(name) \
	shared->hook->remove_hook(name##_hk)

#define hk_cmd(name) \
	c_command::hook(#name, name##_hk, name);

#define unhk_cmd(name) \
	c_command::unhook(#name, name);