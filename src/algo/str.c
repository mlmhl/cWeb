/**
  * @file str.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 09:20:29 2015
  */

#include <stddef.h>

#include "str.h"
#include "../util/vector.h"


int str_contains(const char *str, char c) {
	while (*str != '\0' && *str != c)
		++str;
	return *str == c;
}


char *itoa(int value, char *str, int base) {
	if (str == NULL) {
		return str;
	}
	if (value == 0) {
		str[0] = '0';
		str[1] = '\0';
		return str;
	}

	long long val = value;
	char *res = str;
	if (value < 0) {
		val = -val;
		*str++ = '-';
	}

	long long tmp = val;
	long long len = 0;
	while (tmp > 0) {
		tmp /= base;
		++len;
	}

	str[len] = '\0';
	while (val > 0) {
		int digit = val % base;
		val /= base;
		str[--len] = digit + (digit > 9 ? 'A' - 10 : '0');
	}

	return res;
}


void str_split(char *str, const char *delim, vector *res) {
	if (str == NULL) {
		return;
	}

	while (1) {
		while (str_contains(delim, *str)) {
			++str;
		}
		if (*str == '\0') {
			break;
		}

		v_push_ptr(res, (void*)str);
		while (!str_contains(delim, *str) && *str != '\0') {
			++str;
		}
		if (*str == '\0') {
			break;
		}
		*str++ = '\0';
	}
}
