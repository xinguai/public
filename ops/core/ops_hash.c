/*
 * Copyright right (C) guzai.huang@gmail.com
 */

#include "ops_hash.h"
#include "ops_string.h"

ops_uint_t
ops_hash_key(u_char *data, size_t len){
	ops_uint_t i, key;
	key = 0;

	for (i = 0; i < len; i++){
		key = ops_hash(key, data[i]);
	}

	return key;
}

ops_uint_t
ops_hash_key_lc(u_char *data, size_t len){
	ops_uint_t i, key;

	key = 0;

	for (i = 0; i < len; i++){
		key = ops_hash(key, ops_tolower(data[i]));
	}

	return key;
}

ops_uint_t
ops_hash_strlow(u_char *dst, u_char *src, size_t n){
	ops_uint_t key;

	key = 0;

	while (n--){
		*dst = ops_tolower(*src);
		key = ops_hash(key, *dst);
		dst++;
		src++;
	}

	return key;
}
