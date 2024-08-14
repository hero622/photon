// i wanted to call this "con" but windows wasnt letting me >:(

#pragma once

#include "source_sdk.h"

#include <unordered_map>

class c_con {
public:
	virtual con_var* create_convar( const char* name, const char* default_value, int flags = 0 );                                                                                                       // Create ConVars.
	virtual con_var* create_convar( const char* name, const char* default_value, int flags, const char* help_string, fn_change_callback_t cbk = 0 );                                                    // Create ConVars.
	virtual con_var* create_convar( const char* name, const char* default_value, int flags, const char* help_string, bool has_min, float min, bool has_max, float max, fn_change_callback_t cbk = 0 );  // Create ConVars.
	virtual void     destruct_convar( const char* name );                                                                                                                                               // Destruct ConVars.

	// TODO: add completion func
	virtual con_command* create_concmd( const char* name, fn_command_callback_t cbk, const char* help_string = 0, int flags = 0 );  // Create ConCommands.
	virtual void         destruct_concmd( const char* name );                                                                       // Destruct ConCommands.

	virtual void hook_cmd( const char* name, fn_command_callback_t detour, fn_command_callback_t& original );  // Replace ConCommand callback to your own.
	virtual void unhook_cmd( const char* name, fn_command_callback_t original );                               // Revert ConCommand callback.
};

#define CONCMD_CBK( name ) \
	static void name##_cbk( const c_command& args )

#define DECL_HK_CMD( name )         \
	inline fn_command_callback_t name; \
	static void                  name##_hk( const c_command& args )

#define HK_CMD_FN( name ) \
	void name##_hk( const c_command& args )
