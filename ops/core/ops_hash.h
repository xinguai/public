/*
 * Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_HASH_H_
#define OPS_HASH_H_

#include "ops_config.h"
#include "ops_string.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
#endif

#define container_of(ptr, type, member) ({\
		const typeof( ((type*)0)->member )* __mptr= (ptr);\
		(type*) ((char*)__mptr - offsetof(type, member));})

typedef struct {
	void*        value;
	u_short      len;
    //ops_uint_t*  unused;
}ops_hash_elt_t; 

#define ops_hash(key, c)   ((ops_uint_t) key * 31 + c)

ops_uint_t ops_hash_key(u_char *data, size_t len);
ops_uint_t ops_hash_key_lc(u_char *data, size_t len);
ops_uint_t ops_hash_strlow(u_char *dst, u_char *src, size_t n);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OPS_HASH_H_ */
