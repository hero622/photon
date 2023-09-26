#include "plugin.h"

#include "utils/utils.h"

#include <iostream>

sdk::interface_reg_t *sdk::interface_reg_t::s_p_interface_regs = nullptr;

dll_export void *CreateInterface(const char *p_name, int *p_return_code) {
	sdk::interface_reg_t *p_cur;

	for (p_cur = sdk::interface_reg_t::s_p_interface_regs; p_cur; p_cur = p_cur->m_p_next) {
		if (!std::strcmp(p_cur->m_p_name, p_name)) {
			if (p_return_code) {
				*p_return_code = 0;
			}
			return p_cur->m_create_fn();
		}
	}

	if (p_return_code) {
		*p_return_code = 1;
	}
	return nullptr;
}