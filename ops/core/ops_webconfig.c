#include "ops_htable.h"
#include "ops_hash.h"
#include "ops_string.h"
#include "ops_webconfig.h"

typedef struct {
    struct htable ht;
    struct htable htwc_head;
    struct htable htwc_tail;
    struct htable htwc_blt;
}ops_hash_combined_t;

// typedef struct{
//     int            unused;
//     ops_hash_elt_t hash_elt;
//     short          test;
// }ops_web_t;

ops_hash_combined_t combined;
static ops_mm_len mm, mm_head, mm_tail, mm_blt;

static size_t hash(const void *elem, void *unused){
    ops_hash_elt_t* elt = (ops_hash_elt_t*)elem;
    return ops_hash_key(elt->value, elt->len);
}

static bool objcmp(const void *elem, void *cmpdata){
    ops_hash_elt_t *elt = (ops_hash_elt_t*)elem;
    ops_hash_elt_t *eltcmp = (ops_hash_elt_t*)cmpdata;

    if (elt->len == eltcmp->len){
        return (strncmp(elt->value, eltcmp->value, elt->len) == 0);
    }

    return false;
}

static void ops_htable_combined_init(ops_hash_combined_t* combined){
    mm.max_len = mm_tail.max_len = mm_head.max_len = mm_blt.max_len = 0;
    mm.min_len = mm_head.min_len = mm_tail.min_len = mm_blt.min_len = 1000;
    htable_init(&combined->ht, hash, &mm);
    htable_init(&combined->htwc_head, hash, &mm_head);
    htable_init(&combined->htwc_tail, hash, &mm_tail);
    htable_init(&combined->htwc_blt, hash, &mm_blt);
}

static void ops_htable_combined_clear(ops_hash_combined_t* combined){
    htable_clear(&combined->ht);
    htable_clear(&combined->htwc_head);
    htable_clear(&combined->htwc_tail);
    htable_clear(&combined->htwc_blt);
}

void ops_webconfig_init(){
    ops_htable_combined_init(&combined);
}

void ops_webconfig_clear(){
    ops_htable_combined_clear(&combined);
}


bool ops_webconfig_add(const ops_hash_elt_t* elt){
    bool ret = htable_add(&combined.ht, hash(elt, NULL), elt);

    if (ret){
        ops_mm_len *mm = (ops_mm_len*)combined.ht.priv;

        if (mm->min_len > elt->len){
            mm->min_len = elt->len;
        }

        if (mm->max_len < elt->len){
            mm->max_len = elt->len;
        }
    }

    return ret;
}

bool ops_webconfig_add_head(const ops_hash_elt_t* elt){
    bool ret = htable_add(&combined.htwc_head, hash(elt, NULL), elt);

    if (ret){
        ops_mm_len *mm = (ops_mm_len*)combined.htwc_head.priv;

        if (mm->min_len > elt->len){
            mm->min_len = elt->len;
        }

        if (mm->max_len < elt->len){
            mm->max_len = elt->len;
        }
    }

    return ret;
}

bool ops_webconfig_add_tail(const ops_hash_elt_t* elt){
    bool ret = htable_add(&combined.htwc_tail, hash(elt, NULL), elt);

    if (ret){
        ops_mm_len *mm = (ops_mm_len*)combined.htwc_tail.priv;

        if (mm->min_len > elt->len){
            mm->min_len = elt->len;
        }

        if (mm->max_len < elt->len){
            mm->max_len = elt->len;
        }
    }

    return ret;
}

bool ops_webconfig_add_blt(const ops_hash_elt_t* elt){
    bool ret = htable_add(&combined.htwc_blt, hash(elt, NULL), elt);

    if (ret){
        ops_mm_len *mm = (ops_mm_len*)combined.htwc_blt.priv;

        if (mm->min_len > elt->len){
            mm->min_len = elt->len;
        }

        if (mm->max_len < elt->len){
            mm->max_len = elt->len;
        }
    }

    return ret;
}


void *ops_webconfig_get(const ops_hash_elt_t* elt){
   return  htable_get(&combined.ht, hash(elt, NULL), objcmp, elt);
}

void* ops_webconfig_get_head(const ops_hash_elt_t* elt){
    ops_hash_elt_t elt_dummy ;
    void *e;
    ops_mm_len *mm = (ops_mm_len*)combined.htwc_head.priv;
    u_short len = mm->min_len;

    if (elt->len < mm->min_len){
        return NULL;
    }

    for (; len <= mm->max_len; len++){
        elt_dummy.value = elt->value + (elt->len - len);
        elt_dummy.len = len;
        if ((e = htable_get(&combined.htwc_head, hash(&elt_dummy, NULL), objcmp, &elt_dummy))){
            return e;
        }
    }

    return NULL;

    //return htable_get(&combined.htwc_head, hash(elt, NULL), objcmp, elt);
}

void* ops_webconfig_get_tail(const ops_hash_elt_t* elt){
    ops_hash_elt_t elt_dummy;
    void *e = NULL;
    ops_mm_len *mm = (ops_mm_len*)combined.htwc_tail.priv;
    u_short len = mm->min_len;

    if (elt->len < mm->min_len){
        return NULL;
    }
    elt_dummy.value = elt->value;
    elt_dummy.len = len;
    ops_uint_t hash_key = hash(&elt_dummy, NULL);

    e = htable_get(&combined.htwc_tail, hash_key, objcmp, &elt_dummy);
    if (e){
        return e;
    }

    for (++len; len <= mm->max_len; len++){
        elt_dummy.len = len;
        hash_key = ops_hash(hash_key, *((u_char*)elt_dummy.value + len -1) );
        e = htable_get(&combined.htwc_tail, hash_key, objcmp, &elt_dummy);
        if (e){
            return e;
        }
    }

    return NULL;
    //return htable_get(&combined.htwc_tail, hash(elt, NULL), objcmp, elt);
}

void* ops_webconfig_get_blt(const ops_hash_elt_t* elt){
    return htable_get(&combined.htwc_blt, hash(elt, NULL), objcmp, elt);
}

bool ops_webconfig_del(const ops_hash_elt_t *elt){
    return htable_del(&combined.ht, hash(elt, NULL), elt);
}

bool ops_webconfig_del_head(const ops_hash_elt_t *elt){
    return htable_del(&combined.htwc_head, hash(elt, NULL), elt);
}

bool ops_webconfig_del_tail(const ops_hash_elt_t *elt){
    return htable_del(&combined.htwc_tail, hash(elt, NULL), elt);
}

bool ops_webconfig_del_blt(const ops_hash_elt_t *elt){
    return htable_del(&combined.htwc_blt, hash(elt, NULL), elt);
}

void* ops_webconfig_find_blt(const ops_hash_elt_t *elt){
    void *p;
    ops_hash_elt_t* sp;
    struct htable_iter iter;

    for (p = htable_first(&combined.htwc_blt, &iter); p ; 
         p = htable_next(&combined.htwc_blt, &iter)){
        sp = (ops_hash_elt_t*)p;
        if (ops_strstr(elt->value, elt->len, sp->value, sp->len)){
            return p;
        }
    }
    
    return NULL;
}
