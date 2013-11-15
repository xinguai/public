/*
 * Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_STRING_H_
#define OPS_STRING_H_

#include "ops_config.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct {
	size_t    len;
	u_char   *data;
}ops_str_t;

#define ops_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ops_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)

//#define ops_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)


void ops_strlow(u_char *dst, u_char *src, size_t n);

static inline u_char* ops_strchr(const u_char *s, size_t len, int c){
	size_t ll = len;
	const u_char* p = s;

	for (; ll > 0; ll--, p++){
		if (*p == c){
			return p;
		}
	}
	return NULL;
}

static inline int ops_strncmp(const u_char *s1, size_t len1, const u_char *s2, size_t len2){
	if (len1 < len2){
		return  -1;
	}
	return strncmp(s1, s2, len2);
}

static inline void* ops_strstr(const u_char *s1, size_t len1, const u_char *s2, size_t len2){
	const u_char *p = s1;
	size_t  len_src = len1;

	for (; (p = ops_strchr(p, len_src, *s2)) != 0 ; p++, len_src--){
		len_src = len1 - (p - s1);
		if (ops_strncmp(p, len_src, s2, len2) == 0){
			return (p);
		}
	}

	return (0);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPS_STRING_H_ */
