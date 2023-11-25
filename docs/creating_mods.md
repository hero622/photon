# THERE IS A CHANCE THIS IS OUTDATED

# Creating mods

An example mod can be found [here](https://github.com/Zyntex1/wormhole/tree/master/example-mod).

## Setup
- Download the [Wormhole SDK](https://github.com/Zyntex1/wormhole-sdk).
- Include `wormhole.h` in your main header file.
- Add a class that derives from `wh_api::i_wormhole_mod`.
- Create a global variable for your class.
- Use the `expose_wormhole_mod` macro, pass your class type and your class global var.
- Setup callbacks.
	- A pointer to shared classes gets passed to `load`, these classes are:
    	- `portal2`: Contains some interfaces of the game like `console` or `engine`.
    	- `hook`: This is **Wormhole**'s hooking class, but you actually won't use this a lot, because hooking is done mostly using macros.
      - `events`: You can post events to other mods through this.
      - `huds`: Register your custom huds through this.
      - `render`: Render wrapper around Portal 2's renderer.
      - `input`: Input wrapper around Portal 2's input system.
      - `menu`: Use this to add controls to the mod's settings menu.
	- You should expose this pointer globally (most macros assume that it's named `wh`).
	
Example of a simple mod:
```cpp
// mod.h
#include <wormhole.h>

class c_wormhole_mod : public wh_api::i_wormhole_mod {
public:
	virtual bool load(wh_api::c_shared *wh);
	virtual void unload();
	virtual void on_event(const char *msg);
	virtual wh_api::wh_mod_info_t get_name();
	virtual void paint_menu();
};

extern c_wormhole_mod mod;

// mod.cpp
#include "mod.h"

c_wormhole_mod mod;

expose_wormhole_mod(c_wormhole_mod, mod); 

wh_api::c_shared *wh;

bool c_wormhole_mod::load(wh_api::c_shared *wh) {
	::wh = wh;

	wh->portal2->console->msg("example mod loaded.\n");

	return true;
}

void c_wormhole_mod::unload() {
	wh->portal2->console->msg("example mod unloaded.\n");
}

void c_wormhole_mod::on_event(const char *msg) {
	// draws a white square in the top left corner
	if (!strcmp(msg, "paint")) {
		wh->portal2->surface->draw_set_color(255, 255, 255, 255);
		wh->portal2->surface->draw_filled_rect(0, 0, 100, 100);
	}
}

wh_api::wh_mod_info_t *c_wormhole_mod::get_name() {
	wh_api::wh_mod_info_t info;
	info.name = "example mod";
	info.version = "0.0.1";
	return info;
}

void c_wormhole_mod::paint_menu() {
	static bool example_value;
	wh->menu->checkbox(example_value, "example checkbox");
}
```
## Events
Wormhole posts events to all mods by default:
- `pre_tick`: called before `CServerGameDLL::GameFrame`.
- `post_tick`: called after `CServerGameDLL::GameFrame`.
- `pre_frame`: called before `CEngine::Frame`.
- `post_frame`: called after `CEngine::Frame`.
- `session_start`: called on `SIGNONSTATE_FULL`.
- `session_end`: called on `!SIGNONSTATE_FULL`.
- `paint`: called in `CMatSystemSurface::PaintTraverse` in between `CMatSystemSurface::StartDrawing` and `CMatSystemSurface::FinishDrawing`, you should do all your drawing (excluding huds) in here.
- `on_screen_size_changed`: called in `CMatSystemSurface::OnScreenSizeChanged`, you should recreate your fonts here.

Mods can also post their on events to all other mods, these are syntaxed as `modname:eventname`.
Mods receive their events in the `on_event` function, with the message being the only argument.

### Receiving events
```cpp
// draws a white square in the top left corner
void c_wormhole_mod::on_event(const char *msg) {
	if (!strcmp(msg, "paint")) {
		wh->portal2->surface->draw_set_color(255, 255, 255, 255);
		wh->portal2->surface->draw_filled_rect(0, 0, 100, 100);
	}
}
```

### Posting events
```cpp
// other mods will receive this as `example-mod:example-event`
wh->events->post("example-event");
```

## Interfaces
### Getting a direct pointer to the interface
```cpp
engine_client = get_interface<i_engine_client>(module("engine"), "VEngineClient015");
```

## Calling conventions
Windows uses `__stdcall` for global functions and `__thiscall` for member functions.
Functions with **varargs** should always use `__cdecl`.
For hooks on Windows we use `__fastcall`, with the `ecx` register being the pointer to the class object and `edx` just being unused.

Linux uses `__cdecl` everywhere.
For hooks the `ecx` register still gets passed in, but not the `edx` register (since that only comes from `__fastcall`).

Use the `__rescall` macro to automatically resolve `__thiscall` or `__cdecl` for the two platforms.

## Memory
### Calling virtual functions
```cpp
// utils::memory::call_virtual<offset, return_type>(ptr_to_object, args...);
utils::memory::call_virtual<7, void>(ptr, cmd_string);
```

### Reading addresses of virtual functions
```cpp
auto client_cmd_addr = utils::memory::get_virtual<7>(ptr);
```

### Reading functions at addresses
```cpp
auto client_cmd_addr = utils::memory::get_virtual<7>(this->ptr);

using get_client_state_fn = void *(*)();
auto get_client_state = utils::memory::read<get_client_state_fn>(client_cmd_addr + offsets::get_client_state);
this->client_state = get_client_state();
```

### Getting module handles
Use `module` macro to automatically resolve module extension for Windows and Linux.
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

// expands to void __fastcall game_frame(void *ecx, int edx, bool simulating)
// on Windows, edx register is unused
// expands to void __cdecl game_frame(void *ecx, bool simulating) on Linux
hk_fn(void, game_frame, bool simulating) {
	// calling original function, pass in ecx and all the other variables
	game_frame(ecx, simulating);
}
```

### Hooking virtual functions
```cpp
// hk_virtual(ptr_to_object, hook function, offset)
hk_virtual(wh->portal2->server_game_dll, game_frame, offsets::game_frame);
```

### Hooking functions by address
```cpp
hk_inline(game_frame, 0xdeadbeef);
```

## TBD
