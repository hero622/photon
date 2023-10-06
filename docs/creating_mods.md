# Creating mods

An example mod can be found here: [wh-example-mod](https://github.com/Zyntex1/wh-example-mod)

## Setup
- Download the **Wormhole SDK**.
- Include `wormhole.h` in your main header file.
- Add a class that derives from `wh::i_wormhole_mod`.
- Use the `expose_wormhole_mod` macro, pass your class type, not a pointer.
- Setup `load` and `unload` callbacks.
	- A pointer to shared classes gets passed to `load`, these classes are:
    	- `portal2`: Contains some interfaces of the game like `console` or `engine`.
    	- `hook`: This is **Wormhole**'s hooking class, but you actually won't use this a lot, because hooking is done mostly using macros.
	- You should probably expose this pointer globally.
- Example of a simple mod:
```cpp
// mod.h
#include <wormhole.h>

class c_wormhole_mod : public wh::i_wormhole_mod {
public:
	virtual bool load(wh::c_shared *shared);  // called on plugin load
	virtual void unload();                    // called on plugin unload
	virtual void on_pre_tick();               // called before CServerGameDLL::GameFrame
	virtual void on_post_tick();              // called after CServerGameDll::GameFrame
	virtual void on_pre_frame();              // called before CEngine::Frame
	virtual void on_post_frame();             // called after CEngine::Frame
	virtual void on_session_start();          // called on SIGNONSTATE_FULL
	virtual void on_session_end();            // called on !SIGNONSTATE_FULL
};

// mod.cpp
#include "mod.h"

expose_wormhole_mod(c_wormhole_mod); 

wh::c_shared *shared;

bool c_wormhole_mod::load(wh::c_shared *shared) {
	::shared = shared;

	shared->portal2->console->msg("example mod loaded.\n");

	return true;
}

void c_wormhole_mod::unload() {
	shared->portal2->console->msg("example mod unloaded.\n");
}
```
## Interfaces
### Getting a direct pointer to the interface:
```cpp
engine_client = get_interface<i_engine_client>(module("engine"), "VEngineClient015");
```
### Create an instance of a class that has a pointer to the interface:
```cpp
class c_engine_client {
public:
	void *ptr;

	// important: the function will always pass the pointer 
	// of the interface as an argument to the constructor
	c_engine_client(void *ptr) {
		this->ptr = ptr;
	}
};
c_engine_client *engine_client;

engine_client = copy_interface<c_engine_client>(module("engine"), "VEngineClient015");
```

## Calling conventions
Windows uses `__stdcall` for global functions and `__thiscall` for member functions.
Linux uses `__cdecl` all the time.
Use the `__rescall` macro to automatically resolve the calling convention for both platforms.
**Note**: functions with **varargs** should always use `__cdecl`. 

## Memory
### Calling virtual functions
```cpp
// utils::memory::call_virtual<return_type>(offset, ptr_to_object, args...);
utils::memory::call_virtual<void>(offsets::client_cmd, ptr, sz_cmd_string);
```

### Reading addresses of virtual functions
```cpp
auto client_cmd_addr = utils::memory::get_virtual(ptr, offsets::client_cmd);
```

### Reading functions at addresses
```cpp
auto client_cmd_addr = utils::memory::get_virtual(this->ptr, offsets::client_cmd);

using get_client_state_fn = void *(*)();
auto get_client_state = utils::memory::read<get_client_state_fn>(client_cmd_addr + offsets::get_client_state);
this->client_state = get_client_state();
```

### Getting module handles
Use `module` macro to automatically resolve module name for Windows and Linux.
```cpp
void *module_handle = utils::memory::get_module_handle(module("client"));
```

### Getting functions from symbols
```cpp
void(__cdecl *msg)(const char *p_msg_format, ...);

this->msg = utils::memory::get_sym_addr<decltype(msg)>(modules::tier0, symbols::msg);
```

### Pattern/Signature scanning
```cpp
auto fn_addr = utils::memory::pattern_scan(module("client"), signatures::calc_view_model_lag);
```

## Hooking
### Declaring hooks
```cpp
// hooks.h

// decl_hk(return_type, name, args...)
decl_hk(void, game_frame, bool simulating);
```

### Declaring hook functions
```cpp
// hooks.cpp

// expands to void __thiscall game_frame(void *thisptr, int edx, bool simulating)
// on Windows, edx register is unused
// expands to void __cdecl game_frame(void *thisptr, bool simulating) on Linux
hk_fn(void, game_frame, bool simulating);
```

### Hooking virtual functions
```cpp
// hk_virtual(ptr_to_object, hook function, offset)
hk_virtual(shared->portal2->server_game_dll, game_frame, offsets::game_frame);
```

### Hooking functions by address
```cpp
hk_addr(game_frame, 0xdeadbeef);
```

## Console commands
### Creating commands
```cpp
create_con_command(example_command, "prints hello to the console.\n") {
    shared->portal2->console->msg("hello.");
}
```

## TBD