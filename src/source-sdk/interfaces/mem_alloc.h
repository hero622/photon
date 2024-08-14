#pragma once

#include "util/util.h"

struct _crt_mem_state;

typedef size_t ( *mem_alloc_fail_handler_t )( size_t );

class i_mem_alloc {
public:
	virtual void*                    alloc( size_t size )                                                                                       = 0;
	virtual void*                    realloc( void* mem, size_t size )                                                                          = 0;
	virtual void                     free( void* mem )                                                                                          = 0;
	virtual void*                    expand_no_longer_supported( void* mem, size_t size )                                                       = 0;
	virtual void*                    alloc( size_t size, const char* file_name, int line )                                                      = 0;
	virtual void*                    realloc( void* mem, size_t size, const char* file_name, int line )                                         = 0;
	virtual void                     free( void* mem, const char* file_name, int line )                                                         = 0;
	virtual void*                    expand_no_longer_supported( void* mem, size_t size, const char* file_name, int line )                      = 0;
	virtual size_t                   get_size( void* mem )                                                                                      = 0;
	virtual void                     push_alloc_dbg_info( const char* file_name, int line )                                                     = 0;
	virtual void                     pop_alloc_dbg_info( )                                                                                      = 0;
	virtual long                     crt_set_break_alloc( long new_break_alloc )                                                                = 0;
	virtual int                      crt_set_report_mode( int report_type, int report_mode )                                                    = 0;
	virtual int                      crt_is_valid_heap_pointer( const void* mem )                                                               = 0;
	virtual int                      crt_is_valid_pointer( const void* mem, unsigned int size, int access )                                     = 0;
	virtual int                      crt_check_memory( void )                                                                                   = 0;
	virtual int                      crt_set_dbg_flag( int new_flag )                                                                           = 0;
	virtual void                     crt_mem_checkpoint( _crt_mem_state* state )                                                                = 0;
	virtual void                     dump_stats( )                                                                                              = 0;
	virtual void                     dump_stats_file_base( char const* file_base )                                                              = 0;
	virtual void*                    crt_set_report_file( int rpt_type, void* file )                                                            = 0;
	virtual void*                    crt_set_report_hook( void* new_hook )                                                                      = 0;
	virtual int                      crt_dbg_report( int rpt_type, const char* file, int line, const char* module, const char* msg )            = 0;
	virtual int                      heapchk( )                                                                                                 = 0;
	virtual bool                     is_debug_heap( )                                                                                           = 0;
	virtual void                     get_actual_dbg_info( const char*& file_name, int& line )                                                   = 0;
	virtual void                     register_allocation( const char* file_name, int line, int logical_size, int actual_size, unsigned time )   = 0;
	virtual void                     register_deallocation( const char* file_name, int line, int logical_size, int actual_size, unsigned time ) = 0;
	virtual int                      get_version( )                                                                                             = 0;
	virtual void                     compact_heap( )                                                                                            = 0;
	virtual mem_alloc_fail_handler_t set_alloc_fail_handler( mem_alloc_fail_handler_t mem_alloc_fail_handler )                                  = 0;
	virtual void                     dump_block_stats( void* )                                                                                  = 0;
	virtual size_t                   memory_alloc_failed( )                                                                                     = 0;
	virtual uint32_t                 get_debug_info_size( )                                                                                     = 0;
	virtual void                     save_debug_info( void* debug_info )                                                                        = 0;
	virtual void                     restore_debug_info( const void* debug_info )                                                               = 0;
	virtual void                     init_debug_info( void* debug_info, const char* root_file_name, int line )                                  = 0;
	virtual void                     global_memory_status( size_t* used_memory, size_t* free_memory )                                           = 0;
};
