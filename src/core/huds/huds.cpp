#include "huds.h"

#include "wormhole-sdk/wormhole.h"

#include <cmath>

sdk::vec2_t get_abs_pos(wh_api::hud_t *hud) {
	const auto pos = wh->render->to_screen(hud->pos);
	const auto anchor = hud->anchor * hud->bounds;

	return pos - anchor;
}

void set_abs_pos(wh_api::hud_t *hud, sdk::vec2_t pos) {
	const auto anchor = hud->anchor * hud->bounds;

	hud->pos = wh->render->normalize(pos + anchor);
}

void set_hud_alignment(wh_api::hud_t *hud) {
	const auto screen_size = wh->render->get_screen_size();

	const auto center = get_abs_pos(hud) + sdk::vec2_t(hud->bounds.x / 2, hud->bounds.y / 2);

	if ((int)center.x < screen_size.x / 2)
		hud->anchor.x = 0.0f;
	if ((int)center.x == screen_size.x / 2)
		hud->anchor.x = 0.5f;
	if ((int)center.x > screen_size.x / 2)
		hud->anchor.x = 1.0f;

	if ((int)center.y < screen_size.y / 2)
		hud->anchor.y = 0.0f;
	if ((int)center.y == screen_size.y / 2)
		hud->anchor.y = 0.5f;
	if ((int)center.y > screen_size.y / 2)
		hud->anchor.y = 1.0f;
}

void align_hud_element(wh_api::hud_t *hud, wh_api::hud_t *other_hud) {
	const auto screen_size = wh->render->get_screen_size();

	const auto clr = sdk::color_t(255, 0, 255, 255);

	const auto hud_pos = get_abs_pos(hud);
	const auto other_hud_pos = get_abs_pos(other_hud);

	int hud_rect[6] = {
		hud_pos.x,
		hud_pos.y,
		hud_pos.x + hud->bounds.x,
		hud_pos.y + hud->bounds.y,
		hud_pos.x + hud->bounds.x / 2,
		hud_pos.y + hud->bounds.y / 2};
	int other_hud_rect[6] = {
		other_hud_pos.x,
		other_hud_pos.y,
		other_hud_pos.x + other_hud->bounds.x,
		other_hud_pos.y + other_hud->bounds.y,
		other_hud_pos.x + other_hud->bounds.x / 2,
		other_hud_pos.y + other_hud->bounds.y / 2};
	int safezone_rect[6] = {
		huds::safezone.x,
		huds::safezone.y,
		screen_size.x - huds::safezone.x,
		screen_size.y - huds::safezone.y,
		screen_size.x / 2,
		screen_size.y / 2};

	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 6; ++j) {
			if (i % 2 != j % 2)
				continue;

			if (abs(hud_rect[i] - safezone_rect[j]) < 8) {
				if (j % 2 == 0) {
					hud->pos.x = (safezone_rect[j] - (hud_rect[i] - hud_rect[0]) + hud->anchor.x * hud->bounds.x) / screen_size.x;
					wh->render->draw_line(safezone_rect[j], 0, 0, screen_size.y, clr);
				} else {
					hud->pos.y = (safezone_rect[j] - (hud_rect[i] - hud_rect[1]) + hud->anchor.y * hud->bounds.y) / screen_size.y;
					wh->render->draw_line(0, safezone_rect[j], screen_size.x, 0, clr);
				}
			}

			if (abs(hud_rect[i] - other_hud_rect[j]) < 8) {
				if (j % 2 == 0) {
					hud->pos.x = (other_hud_rect[j] - (hud_rect[i] - hud_rect[0]) + hud->anchor.x * hud->bounds.x) / screen_size.x;
					wh->render->draw_line(other_hud_rect[j], 0, 0, screen_size.y, clr);
				} else {
					hud->pos.y = (other_hud_rect[j] - (hud_rect[i] - hud_rect[1]) + hud->anchor.y * hud->bounds.y) / screen_size.y;
					wh->render->draw_line(0, other_hud_rect[j], screen_size.x, 0, clr);
				}
			}
		}
	}
}

void huds::paint() {
	for (const auto &hud : huds) {
		hud->paint();

		for (const auto &draw_call : hud->draw_calls) {
			hud->bounds.x = std::fmax(hud->bounds.x, draw_call->x + draw_call->w);
			hud->bounds.y = std::fmax(hud->bounds.y, draw_call->y + draw_call->h);
		}
		for (const auto &draw_call : hud->draw_calls) {
			const auto pos = get_abs_pos(hud);

			draw_call->x += pos.x;
			draw_call->y += pos.y;

			draw_call->call();
		}

		hud->draw_calls.clear();
	}

	for (const auto &thud : thuds) {
		// todo: some formatting system
		auto text = std::string(thud->format);

		utils::string::replace(text, "{name}", std::string(thud->get_name()));
		utils::string::replace(text, "{value}", std::string(thud->get_text()));

		const auto font = wh->render->get_font(thud->font);

		thud->bounds = wh->render->get_text_size(font, text);

		const auto pos = get_abs_pos(thud);

		wh->render->draw_text(pos.x, pos.y, font, {255, 255, 255, 255}, false, text);
	}
}

void huds::paint_ui() {
	const auto clr = sdk::color_t(0, 255, 255, 255);

	const auto screen_size = wh->render->get_screen_size();

	static wh_api::hud_t *cur_hud;
	static sdk::vec2_t grab_pos;

	for (const auto &hud : huds) {
		sdk::vec2_t orig_cur_pos;

		const auto pos = get_abs_pos(hud);

		if (wh->input->is_cursor_in_area(pos.x, pos.y, pos.x + hud->bounds.x, pos.y + hud->bounds.y)) {
			wh->render->draw_outlined_rect(pos.x - 1, pos.y - 1, hud->bounds.x + 2, hud->bounds.y + 2, clr);
			wh->render->draw_filled_rect(pos.x + hud->anchor.x * hud->bounds.x - 3, pos.y + hud->anchor.y * hud->bounds.y - 3, 6, 6, clr);

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = hud;
				grab_pos = wh->input->get_cursor_position() - pos;
			}
		}
	}

	for (const auto &thud : thuds) {
		sdk::vec2_t orig_cur_pos;

		const auto pos = get_abs_pos(thud);

		if (wh->input->is_cursor_in_area(pos.x, pos.y, pos.x + thud->bounds.x, pos.y + thud->bounds.y)) {
			wh->render->draw_outlined_rect(pos.x - 1, pos.y - 1, thud->bounds.x + 2, thud->bounds.y + 2, clr);
			wh->render->draw_filled_rect(pos.x + thud->anchor.x * thud->bounds.x - 3, pos.y + thud->anchor.y * thud->bounds.y - 3, 6, 6, clr);

			if (wh->input->get_key_press(sdk::mouse_left)) {
				cur_hud = thud;
				grab_pos = wh->input->get_cursor_position() - pos;
			}
		}
	}

	if (cur_hud) {
		if (wh->input->get_key_held(sdk::mouse_left)) {
			const auto hud = cur_hud;

			set_hud_alignment(hud);

			set_abs_pos(hud, wh->input->get_cursor_position() - grab_pos);

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