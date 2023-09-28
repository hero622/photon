#pragma once

#ifdef _WIN32
#	define plat_module(name, win, linux) inline const char *name = win
#	define offset(name, win, linux) inline int name = win
#	define signature(name, win, linux) inline const char *name = win
#	define symbol(name, win, linux) inline const char *name = win
#else
#	define plat_module(name, win, linux) inline const char *name = linux
#	define offset(name, win, linux) inline int name = linux
#	define signature(name, win, linux) inline const char *name = linux
#	define symbol(name, win, linux) inline const char *name = linux
#endif

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