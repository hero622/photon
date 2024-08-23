// i wanted to call this "con" but windows wasnt letting me >:(

#pragma once

#include "source_sdk.h"

#include <unordered_map>

class c_con {
public:
	// TODO: add completion func
	virtual con_command* create_concmd( const char* name, fn_command_callback_t cbk, const char* help_string = 0, int flags = 0 );  // Create ConCommands.
	virtual void         destruct_concmd( const char* name );                                                                       // Destruct ConCommands.

	virtual void hook_cmd( const char* name, fn_command_callback_t detour );  // Replace ConCommand callback to your own.
	virtual void unhook_cmd( const char* name );                              // Revert ConCommand callback.

	virtual con_var* find_con_var( const char* name );  // Find a ConVar's pointer by name.
};
