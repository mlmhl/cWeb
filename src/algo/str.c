/**
  * @file str.c
  * @author malin
  * @mail malinbupt@163.com 
  * @time 四 10/15 09:20:29 2015
  */

#include <stddef.h>
#include <math.h>

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


char *ftoa(double value, char *str, int precision, int flag) {
	if (value == 0.0) {
		*str = '0';
		*(str + 1) = '\0';
		return str;
	}
	
	if (precision <= 0) {
		return NULL;
	}
	if (value < 0) {
			value = -value;
			*str++ = '-';
	}
	char *res = str;

	if (flag == NORMAL) {	
		long long integer = (long long)value;
		long long tmp = integer;
		int cnt = 0;
		while (tmp > 0) {
			++cnt;
			tmp /= 10;
		}

		str += cnt;
		tmp = integer;
		while (tmp > 0) {
			*(--str) = '0' + tmp % 10;
			tmp /= 10;
		}

		str += cnt;
		integer = (value - integer) * pow(10, precision);
		if (integer > 0) {
			*str++ = '.';
			str += precision;
			while (integer > 0) {
				*(--str)= '0' + integer % 10;
				integer /= 10;
			}
		}
		str += precision;
		*str = '\0';
	}
	else if (flag == SCIENCE) {
		int exp = 0;
		--precision;
		if (value >= 1) {
			long long integer = (long long)value;
			while (integer > 10) {
				++exp;
				integer /= 10;
			}
			*str++ = '0' + integer;
			if (precision > 0) {
				*str++ = '.';
				int tmp = exp;
				if (tmp >= precision) {
					while (tmp >= precision) {
						--tmp;
						value /= 10;
					}
				}
				else {
					while (tmp < precision) {
						++tmp;
						value *= 10;
					}
				}
				integer = (long long)value;
				while (tmp > 0) {
					str[--tmp] = '0' + integer % 10;
					integer /= 10;
				}
				str += precision;
			}
		}
		else {
			while (value < 1) {
				--exp;
				value *= 10;
			}
			*str++ = '0' + (int)value;
			if (precision > 0) {
				*str++ = '.';
				while (precision-- > 0) {
					value -= (int)value;
					value *= 10;
					*str++ = '0' + (int)value;
				}
			}
		}
		
		if (*(str - 1) == '0') {
			while (*(--str) == '0');
			++str;
		}
		*str++ = 'e';
		itoa(exp, str, 10);
	}
	else {
		res = NULL;
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
