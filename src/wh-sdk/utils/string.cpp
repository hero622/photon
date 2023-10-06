#include "string.h"

#include <stdarg.h>
#include <stdio.h>

std::string utils::string::ssprintf(const char *fmt, ...) {
	va_list ap1, ap2;
	va_start(ap1, fmt);
	va_copy(ap2, ap1);
	size_t sz = vsnprintf(NULL, 0, fmt, ap1) + 1;
	va_end(ap1);
	char *buf = (char *)malloc(sz);
	vsnprintf(buf, sz, fmt, ap2);
	va_end(ap2);
	std::string str(buf);
	free(buf);
	return str;
}

char *utils::string::stristr(const char *str1, const char *str2) {
// linux has function for this but windows doesnt
#ifdef _WIN32
	const char *p1 = str1;
	const char *p2 = str2;
	const char *r = *p2 == 0 ? str1 : 0;

	while (*p1 != 0 && *p2 != 0) {
		if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2)) {
			if (r == 0) {
				r = p1;
			}

			p2++;
		} else {
			p2 = str2;
			if (r != 0) {
				p1 = r + 1;
			}

			if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2)) {
				r = p1;
				p2++;
			} else {
				r = 0;
			}
		}

		p1++;
	}

	return *p2 == 0 ? (char *)r : 0;
#else
	return strcasestr(str1, str2);
#endif
}