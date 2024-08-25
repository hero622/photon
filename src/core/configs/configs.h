#pragma once

#include "sdk/source_sdk.h"

#include <string>
#include <vector>

namespace configs {
	inline std::vector< std::string > cfgs;

	void iterate_cfgs( );
	bool initialize( );
	void uninitialize( );

	void save( const char* cfg_name );
	void load( const char* cfg_name );

	void begin( const char* module_name );
	void end( );
	void serialize( const char* var_name, const char*& var );
	void serialize( const char* var_name, bool& var );
	void serialize( const char* var_name, int32_t& var );
	void serialize( const char* var_name, uint32_t& var );
	void serialize( const char* var_name, float& var );
	void serialize( const char* var_name, color_t& var );
	void deserialize( const char* module_name );
};  // namespace configs
