#pragma once

#include "utlvector.h"

namespace sdk {
	class c_command {
	public:
		int arg_c() const { return argc; };
		const char *operator[](int index) const { return arg(index); };
		const char *arg(int index) const { return this->argv[index]; };

		enum {
			command_max_argc = 64,
			command_max_length = 512,
		};
		int argc;
		int argv0_size;
		char arg_s_buffer[command_max_length];
		char argv_buffer[command_max_length];
		const char *argv[command_max_argc];
	};

	typedef void (*fn_command_callback_t)(const c_command &command);
	typedef int (*fn_command_completion_callback)(const char *partial, char commands[64][64]);

	class con_command_base {
	public:
		con_command_base(const char *name, const char *help_string = 0, int flags = 0)
			: next(nullptr)
			, registered(false)
			, name(name)
			, help_string(help_string)
			, flags(flags) {
		}
		virtual void _dtor(){};
#ifndef _WIN32
		virtual void _dtor1(){};
#endif

		con_command_base *next;
		bool registered;
		const char *name;
		const char *help_string;
		int flags;
	};

	class con_command : public con_command_base {
	public:
		con_command(const char *name, fn_command_callback_t callback, const char *help_string = 0, int flags = 0, fn_command_completion_callback completion_func = 0)
			: con_command_base(name, help_string, flags)
			, fn_command_callback(callback)
			, fn_completion_callback(completion_func)
			, has_completion_callback(completion_func != nullptr)
			, using_new_command_callback(true)
			, using_command_callback_interface(false) {
		}

		union {
			void *fn_command_callback_v1;
			fn_command_callback_t fn_command_callback;
			void *command_callback;
		};
		union {
			fn_command_completion_callback fn_completion_callback;
			void *command_completion_callback;
		};
		bool has_completion_callback : 1;
		bool using_new_command_callback : 1;
		bool using_command_callback_interface : 1;
	};

	class i_con_var {
	public:
		virtual void set_value(const char *p_value) = 0;
		virtual void set_value(float fl_value) = 0;
		virtual void set_value(int n_value) = 0;
		virtual const char *get_name(void) const = 0;
		virtual bool is_flag_set(int n_flag) const = 0;
	};

	typedef void (*fn_change_callback_t)(i_con_var *var, const char *p_old_value, float fl_old_value);

	class con_var : con_command_base {
	public:
		void *con_var_v_table;
		con_var *m_p_parent;
		const char *m_psz_default_value;
		char *m_psz_string;
		int m_string_length;
		float m_f_value;
		int m_n_value;
		bool m_b_has_min;
		float m_f_min_val;
		bool m_b_has_max;
		float m_f_max_val;
		utl_vector<fn_change_callback_t> m_fn_change_callback;

		con_var(const char *name, const char *p_default_value, int flags, const char *help_string, bool b_min, float f_min, bool b_max, float f_max)
			: con_command_base(name, help_string, flags)
			, con_var_v_table(nullptr)
			, m_p_parent(nullptr)
			, m_psz_default_value(p_default_value)
			, m_psz_string(nullptr)
			, m_string_length(0)
			, m_f_value(0.0f)
			, m_n_value(0)
			, m_b_has_min(b_min)
			, m_f_min_val(f_min)
			, m_b_has_max(b_max)
			, m_f_max_val(f_max)
			, m_fn_change_callback() {
		}
	};
}  // namespace sdk