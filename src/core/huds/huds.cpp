#include "huds.h"

#include "wormhole-sdk/wormhole.h"

void *g_active_hud;

void huds::paint() {
	for (const auto &hud : huds) {
		hud->paint();
	}

	for (const auto &thud : thuds) {
		// resolve hud position based on alignment later
		// have some formatting system
		// read font in later
		wh->render->draw_text(thud->pos.x, thud->pos.y, 6, {255, 255, 255, 255}, false, utils::string::ssprintf("%s: %s", thud->get_name(), thud->get_text()));
	}
}

void huds::paint_ui() {
	wh->input->get_cursor_position();

	static void *cur_hud;
	static sdk::vec2_t grab_pos;

	for (const auto &hud : huds) {
		sdk::vec2_t orig_cur_pos;

		if (wh->input->is_cursor_in_area(hud->pos.x, hud->pos.y, hud->pos.x + hud->bounds.x, hud->pos.y + hud->bounds.y)) {
			wh->render->draw_outlined_rect(hud->pos.x - 1, hud->pos.y - 1, hud->bounds.x + 2, hud->bounds.y + 2, {0, 255, 255, 255});

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = hud;
				grab_pos = wh->input->get_cursor_position() - hud->pos;
			}
		}
	}

	for (const auto &thud : thuds) {
		sdk::vec2_t orig_cur_pos;

		sdk::vec2_t bounds = wh->render->get_text_size(6, utils::string::ssprintf("%s: %s", thud->get_name(), thud->get_text()));
		if (wh->input->is_cursor_in_area(thud->pos.x, thud->pos.y, thud->pos.x + bounds.x, thud->pos.y + bounds.y)) {
			wh->render->draw_outlined_rect(thud->pos.x - 1, thud->pos.y - 1, bounds.x + 2, bounds.y + 2, {0, 255, 255, 255});

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = thud;
				grab_pos = wh->input->get_cursor_position() - thud->pos;
			}
		}
	}

	if (cur_hud) {
		if (wh->input->get_key_held(sdk::mouse_left)) {
			// this cast is fine as long as both i_hud and i_thud have the pos at the same idx
			((wh_api::i_hud *)cur_hud)->pos = wh->input->get_cursor_position() - grab_pos;
		}

		if (wh->input->get_key_release(sdk::mouse_left)) {
			cur_hud = nullptr;
		}
	}
}