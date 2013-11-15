/*
 *	Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_TIMES_H_
#define OPS_TIMES_H_

#include "ops_config.h"
#include "ops_time.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

typedef struct {
	time_t      sec;
	ops_uint_t  msec;
	ops_int_t   gmtoff;
}ops_time_t;

void ops_gmtime(time_t t, ops_tm_t *tp);

char* ops_str_time(char* buf, time_t t);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  /* OPS_TIMES_H_ */
