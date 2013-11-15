/*
 * Copyright (C) guzai.huang@gmail.com
 */
#include "ops_time.h"
void 
ops_localtime(time_t s, ops_tm_t *tm){
#if (NGX_HAVE_LOCALTIME_R)
	(void) localtime_r(&s, tm)
#else
	ops_tm_t *t;
	t = localtime(&s);
	*tm = *t;
#endif
	tm->ops_tm_mon++;
	tm->ops_tm_year += 1900;
}
