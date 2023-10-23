#include "hooks.h"

#include "core/huds/huds.h"
#include "core/menu/menu.h"
#include "core/mods/mods.h"
#include "core/wormhole.h"
#include "wormhole-sdk/portal2.h"

bool hooks::initialize() {
	hk_virtual(wh->portal2->server_game_dll, game_frame, offsets::game_frame);
	hk_virtual(wh->portal2->engine, frame, offsets::frame);
	hk_virtual(wh->portal2->client_state, set_signon_state, offsets::set_signon_state);
	hk_virtual(wh->portal2->engine_vgui_internal, paint, offsets::paint);
	hk_virtual(wh->portal2->surface, lock_cursor, offsets::lock_cursor);
	hk_virtual(wh->portal2->base_client_dll, in_key_event, offsets::in_key_event);
	hk_virtual(wh->portal2->surface, on_screen_size_changed, offsets::on_screen_size_changed);

	hk_cmd(plugin_unload);

	return true;
}

void hooks::uninitialize() {
	unhk_cmd(plugin_unload);

	unhk(paint);
	unhk(set_signon_state);
	unhk(frame);
	unhk(game_frame);
}

hk_fn(void, hooks::game_frame, bool simulating) {
	wh->events->post(&wormhole, "pre_tick");

	game_frame(ecx, simulating);

	wh->events->post(&wormhole, "post_tick");
}

hk_fn(void, hooks::frame) {
	wh->events->post(&wormhole, "pre_frame");

	frame(ecx);

	wh->events->post(&wormhole, "post_frame");
}

hk_fn(void, hooks::set_signon_state, int state, int count, void *unk) {
	set_signon_state(ecx, state, count, unk);

	if (state == sdk::signonstate_full)
		wh->events->post(&wormhole, "session_start");
	else
		wh->events->post(&wormhole, "session_end");
}

hk_fn(void, hooks::paint, sdk::paint_mode_t mode) {
	paint(ecx, mode);

	wh->portal2->surface->start_drawing();

	if (mode == sdk::paint_uipanels) {
		wh->input->poll_input();  // not sure if this is the best place to call this

		huds::paint();

		wh->events->post(&wormhole, "paint");

		menu::paint();
		huds::paint_ui();
	}

	wh->portal2->surface->finish_drawing();
}

hk_fn(void, hooks::lock_cursor) {
	if (menu::open)
		wh->portal2->surface->unlock_cursor();
	else
		lock_cursor(ecx);
}

hk_fn(int, hooks::in_key_event, int eventcode, sdk::button_code_t keynum, const char *current_binding) {
	if (menu::open)
		return 0;

	return in_key_event(ecx, eventcode, keynum, current_binding);
}

hk_fn(void, hooks::on_screen_size_changed, int old_width, int old_height) {
	on_screen_size_changed(ecx, old_width, old_height);

	wh->events->post(&wormhole, "on_screen_size_changed");

	// recreate fonts
	menu::initialize();
}

hk_cmd_fn(hooks::plugin_unload) {
	if (args.arg_c() >= 2 && wormhole.get_plugin() && (!strcmp(args[1], "wormhole") || std::atoi(args[1]) == wormhole.plugin->index))
		wh->portal2->engine_client->client_cmd("wh_exit");
	else
		plugin_unload(args);
}