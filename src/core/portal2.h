#pragma once

#include "wh-sdk/utils.h"

namespace modules {
	plat_module(tier0, "tier0.dll", "libtier0.so");
	plat_module(tier1, "vstdlib.dll", "libvstdlib.so");
}  // namespace modules

namespace offsets {
	offset(register_con_command, 9, 9);
	offset(unregister_con_command, 10, 10);
	offset(find_command_base, 13, 13);
	offset(cmd_list, 48, 48);
	offset(game_frame, 4, 4);
	offset(is_running_simulation, 12, 12);
	offset(frame, 5, 6);
	offset(eng, 2, 4);
	offset(client_cmd, 7, 7);
	offset(get_client_state, 4, 9);
	offset(set_signon_state, 15, 36);
	offset(cbuf_add_text, 46, 35);
	offset(get_active_split_screen_slot, 127, 127);
	offset(execute_client_cmd, 104, 104);
	offset(draw_set_color, 14, 13);
	offset(draw_filled_rect, 15, 15);
	offset(draw_outlined_rect, 16, 16);
	offset(draw_line, 18, 18);
	offset(draw_set_text_font, 22, 22);
	offset(draw_set_text_color, 23, 24);
	offset(get_font_tall, 72, 72);
	offset(get_text_size, 77, 77);
	offset(draw_set_texture_file, 35, 35);
	offset(draw_set_texture_rgba, 36, 36);
	offset(draw_set_texture, 37, 37);
	offset(draw_get_texture_size, 38, 38);
	offset(draw_textured_rect, 39, 39);
	offset(is_texture_id_valid, 40, 40);
	offset(create_new_texture_id, 41, 41);
	offset(paint, 14, 15);
	offset(paint_traverse_ex, 117, 117);
	offset(start_drawing, 127, 1341);
	offset(finish_drawing, 603, 355);
}  // namespace offsets

namespace signatures {

}

namespace symbols {
	symbol(msg, "Msg", "Msg");
	symbol(color_msg, "?ConColorMsg@@YAXABVColor@@PBDZZ", "_Z11ConColorMsgRK5ColorPKcz");
	symbol(warning, "Warning", "Warning");
	symbol(dev_msg, "?DevMsg@@YAXPBDZZ", "_Z6DevMsgPKcz");
	symbol(dev_warning, "?DevWarning@@YAXPBDZZ", "_Z10DevWarningPKcz");
}  // namespace symbols