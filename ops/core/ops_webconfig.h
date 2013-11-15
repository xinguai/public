#ifndef OPS_WEBCONFIG_H_
#define OPS_WEBCONFIG_H_

#include "ops_hash.h"
#include "ops_htable.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct {
	u_short min_len;
	u_short max_len;
}ops_mm_len;

void ops_webconfig_init();

void ops_webconfig_clear();

bool ops_webconfig_add(const ops_hash_elt_t* elt);

bool ops_webconfig_add_head(const ops_hash_elt_t* elt);

bool ops_webconfig_add_tail(const ops_hash_elt_t* elt);

bool ops_webconfig_add_blt(const ops_hash_elt_t* elt);


void *ops_webconfig_get(const ops_hash_elt_t* elt);

void* ops_webconfig_get_head(const ops_hash_elt_t* elt);

void* ops_webconfig_get_tail(const ops_hash_elt_t* elt);

void* ops_webconfig_get_blt(const ops_hash_elt_t* elt);

bool ops_webconfig_del(const ops_hash_elt_t *elt);

bool ops_webconfig_del_head(const ops_hash_elt_t *elt);

bool ops_webconfig_del_tail(const ops_hash_elt_t *elt);

bool ops_webconfig_del_blt(const ops_hash_elt_t *elt);

void* ops_webconfig_find_blt(const ops_hash_elt_t *elt);

#ifdef __cplusplus
}
#endif

#endif /* OPS_WEBCONFIG_H_ */
