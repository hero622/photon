#pragma once

namespace sdk {
	template <class t>
	class c_utl_vector;
	class c_utl_string;

	class con_command_base;

	class c_command {
	public:
		int arg_c() const { return m_n_argc; };
		const char *operator[](int n_index) const { return arg(n_index); };
		const char *arg(int n_index) const { return this->m_pp_argv[n_index]; };

	private:
		enum {
			command_max_argc = 64,
			command_max_length = 512,
		};
		int m_n_argc;
		int m_n_argv0_size;
		char m_p_arg_s_buffer[command_max_length];
		char m_p_argv_buffer[command_max_length];
		const char *m_pp_argv[command_max_argc];
	};

	typedef void (*fn_command_callback_t)(const c_command &command);
	typedef int (*fn_command_completion_callback)(const char *partial, char commands[64][64]);

	class con_command_base {
	public:
		con_command_base(const char *p_name, const char *p_help_string = 0, int flags = 0)
			: m_p_next(nullptr)
			, m_b_registered(false)
			, m_psz_name(p_name)
			, m_psz_help_string(p_help_string)
			, m_n_flags(flags) {
		}
		virtual ~con_command_base(void){};

	private:
		con_command_base *m_p_next;
		bool m_b_registered;
		const char *m_psz_name;
		const char *m_psz_help_string;
		int m_n_flags;
	};

	class con_command : public con_command_base {
	public:
		con_command(const char *p_name, fn_command_callback_t callback, const char *p_help_string = 0, int flags = 0, fn_command_completion_callback completion_func = 0)
			: con_command_base(p_name, p_help_string, flags)
			, m_fn_command_callback(callback)
			, m_fn_completion_callback(completion_func)
			, m_b_has_completion_callback(completion_func != nullptr)
			, m_b_using_new_command_callback(true)
			, m_b_using_command_callback_interface(false) {
		}

	private:
		union {
			void *m_fn_command_callback_v1;
			fn_command_callback_t m_fn_command_callback;
			void *m_p_command_callback;
		};
		union {
			fn_command_completion_callback m_fn_completion_callback;
			void *m_p_command_completion_callback;
		};
		bool m_b_has_completion_callback : 1;
		bool m_b_using_new_command_callback : 1;
		bool m_b_using_command_callback_interface : 1;
	};
}  // namespace sdk