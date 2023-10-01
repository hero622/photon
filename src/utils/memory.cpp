#include "memory.h"

#include <vector>

bool utils::memory::detour(char *src, char *dst, const uintptr_t len) {
	if (len < 5) return false;

	int old_prot;
#ifdef _WIN32
	VirtualProtect(src, len, PAGE_EXECUTE_READWRITE, (DWORD *)&old_prot);
#else
	old_prot = mprotect(src, len, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif

	uintptr_t rel_addr = (uintptr_t)(dst - (uintptr_t)src) - 5;

	*src = 0xE9;
	*(uintptr_t *)((uintptr_t)src + 1) = rel_addr;

#ifdef _WIN32
	VirtualProtect(src, len, old_prot, (DWORD *)&old_prot);
#else
	old_prot = mprotect(src, len, old_prot);
#endif
	return true;
}

char *utils::memory::trampoline_hk(char *src, char *dst, const uintptr_t len) {
	if (len < 5) return 0;

#ifdef _WIN32
	void *trampoline = VirtualAlloc(0, len + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
#else
	void *trampoline = mmap((void *)0, len + 5, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0);
	mprotect((void *)0, len + 5, PROT_READ | PROT_WRITE | PROT_EXEC);
#endif

	memcpy(trampoline, src, len);

	uintptr_t trampoline_rel_addr = ((uintptr_t)src - (uintptr_t)trampoline) - 5;

	*(char *)((uintptr_t)trampoline + len) = 0xE9;

	*(uintptr_t *)((uintptr_t)trampoline + len + 1) = trampoline_rel_addr;

	detour(src, dst, len);

	return (char *)trampoline;
}

std::uint8_t *utils::memory::pattern_scan(const char *module_name, const char *signature) noexcept {
	const auto module_handle = get_module_handle(module_name);

	if (!module_handle)
		return nullptr;

	static auto pattern_to_byte = [](const char *pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char *>(pattern);
		auto end = const_cast<char *>(pattern) + std::strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;

				if (*current == '?')
					++current;

				bytes.push_back(-1);
			} else {
				bytes.push_back(std::strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	auto nt_headers =
		reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<std::uint8_t *>(module_handle) + dos_header->e_lfanew);

	auto size_of_image = nt_headers->OptionalHeader.SizeOfImage;
	auto pattern_bytes = pattern_to_byte(signature);
	auto scan_bytes = reinterpret_cast<std::uint8_t *>(module_handle);

	auto s = pattern_bytes.size();
	auto d = pattern_bytes.data();

	for (auto i = 0ul; i < size_of_image - s; ++i) {
		bool found = true;

		for (auto j = 0ul; j < s; ++j) {
			if (scan_bytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found)
			return &scan_bytes[i];
	}
}