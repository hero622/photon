#pragma once

#include "sdk/source_sdk.h"

class i_command_line {
public:
	virtual void        create_cmd_line( const char* commandline )                       = 0;
	virtual void        create_cmd_line( int argc, char** argv )                         = 0;
	virtual const char* get_cmd_line( void ) const                                       = 0;
	virtual const char* check_parm( const char* psz, const char** value = 0 ) const      = 0;
	virtual void        remove_parm( const char* parm )                                  = 0;
	virtual void        append_parm( const char* parm, const char* values )              = 0;
	virtual const char* parm_value( const char* psz, const char* default_val = 0 ) const = 0;
	virtual int         parm_value( const char* psz, int default_val ) const             = 0;
	virtual float       parm_value( const char* psz, float default_val ) const           = 0;
	virtual int         parm_count( ) const                                              = 0;
	virtual int         find_parm( const char* psz ) const                               = 0;
	virtual const char* get_parm( int index ) const                                      = 0;
	virtual void        set_parm( int index, char const* new_parm )                      = 0;
};
