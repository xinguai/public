/*
 * Copyright(C) guzai.huang@gmail.com
 */

#include "ops_string.h"

void
ops_strlow(u_char *dst, u_char *src, size_t n){
	while (n) {
		*dst = ops_tolower(*src);
		dst++;
		src++;
		n--;
	}
}
