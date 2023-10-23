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
		const auto pos = wh->render->to_screen(thud->pos);
		wh->render->draw_text(pos.x, pos.y, wh->render->get_font(thud->font), {255, 255, 255, 255}, false, thud->get_text());
	}
}

void align_hud_element(wh_api::hud_t *hud, wh_api::hud_t *other_hud) {
	const auto screen_size = wh->render->get_screen_size();

	const auto clr = sdk::color_t(255, 0, 255, 255);

	const auto hud_pos = wh->render->to_screen(hud->pos);
	const auto other_hud_pos = wh->render->to_screen(other_hud->pos);

	int hud_rect[4] = {hud_pos.x, hud_pos.y, hud_pos.x + hud->bounds.x, hud_pos.y + hud->bounds.y};
	int other_hud_rect[4] = {other_hud_pos.x, other_hud_pos.y, other_hud_pos.x + other_hud->bounds.x, other_hud_pos.y + other_hud->bounds.y};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (i % 2 != j % 2)
				continue;

			if (abs(hud_rect[i] - other_hud_rect[j]) < 8) {
				hud_rect[i] = other_hud_rect[j];

				switch (j) {
				case 0:
					hud->pos.x = (other_hud_rect[j] - (i == 0 ? 0 : hud->bounds.x)) / screen_size.x;
					wh->render->draw_line(other_hud_rect[j] - 1, 0, 0, screen_size.y, clr);
					break;
				case 1:
					hud->pos.y = (other_hud_rect[j] - (i == 1 ? 0 : hud->bounds.y)) / screen_size.y;
					wh->render->draw_line(0, other_hud_rect[j] - 1, screen_size.x, 0, clr);
					break;
				case 2:
					hud->pos.x = (other_hud_rect[j] - (i == 0 ? 0 : hud->bounds.x)) / screen_size.x;
					wh->render->draw_line(other_hud_rect[j], 0, 0, screen_size.y, clr);
					break;
				case 3:
					hud->pos.y = (other_hud_rect[j] - (i == 1 ? 0 : hud->bounds.y)) / screen_size.y;
					wh->render->draw_line(0, other_hud_rect[j], screen_size.x, 0, clr);
					break;
				}
			}
		}
	}
}

void huds::paint_ui() {
	const auto clr = sdk::color_t(0, 255, 255, 255);

	const auto screen_size = wh->render->get_screen_size();

	static wh_api::hud_t *cur_hud;
	static sdk::vec2_t grab_pos;

	for (const auto &hud : huds) {
		sdk::vec2_t orig_cur_pos;

		const auto pos = wh->render->to_screen(hud->pos);

		if (wh->input->is_cursor_in_area(pos.x, pos.y, pos.x + hud->bounds.x, pos.y + hud->bounds.y)) {
			wh->render->draw_outlined_rect(pos.x - 1, pos.y - 1, hud->bounds.x + 2, hud->bounds.y + 2, clr);

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = hud;
				grab_pos = wh->input->get_cursor_position() - pos;
			}
		}
	}

	for (const auto &thud : thuds) {
		sdk::vec2_t orig_cur_pos;

		const auto pos = wh->render->to_screen(thud->pos);

		if (wh->input->is_cursor_in_area(pos.x, pos.y, pos.x + thud->bounds.x, pos.y + thud->bounds.y)) {
			wh->render->draw_outlined_rect(pos.x - 1, pos.y - 1, thud->bounds.x + 2, thud->bounds.y + 2, clr);

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = thud;
				grab_pos = wh->input->get_cursor_position() - pos;
			}
		}
	}

	if (cur_hud) {
		if (wh->input->get_key_held(sdk::mouse_left)) {
			auto hud = cur_hud;

			hud->pos = wh->render->normalize(wh->input->get_cursor_position() - grab_pos);

			for (const auto &other_hud : huds) {
				if (hud == other_hud)
					continue;

				align_hud_element(hud, other_hud);
			}
			for (const auto &other_hud : thuds) {
				if (hud == other_hud)
					continue;

				align_hud_element(hud, other_hud);
			}
		}

		if (wh->input->get_key_release(sdk::mouse_left)) {
			cur_hud = nullptr;
		}
	}
}