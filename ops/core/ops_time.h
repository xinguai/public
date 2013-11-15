/*
 * Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_TIME_H_
#define OPS_TIME_H_

#include "ops_config.h"

#ifdef __cplusplus
extern "C"{
#endif /*__cplusplus*/

typedef struct tm ops_tm_t;

#define ops_tm_sec     tm_sec
#define ops_tm_min     tm_min
#define ops_tm_hour    tm_hour
#define ops_tm_mday    tm_mday
#define ops_tm_mon     tm_mon
#define ops_tm_year    tm_year
#define ops_tm_wday    tm_wday
#define ops_tm_isdst   tm_isdst

#define ops_tm_sec_t   int
#define ops_tm_min_t   int
#define ops_tm_hour_t  int
#define ops_tm_mday_t  int
#define ops_tm_mon_t   int
#define ops_tm_year_t  int
#define ops_tm_wday_t    int
#define ops_tm_isdst_t   int

void ops_localtime(time_t s, ops_tm_t *tm);

#define ops_gettimeofday(tp)  (void) gettimeofday(tp, NULL)
#define ops_usleep(us)        (void) usleep(us)
#define ops_msleep(ms)        (void) usleep(ms * 1000)
#define ops_sleep(s)          (void) sleep(s)

#ifdef __cplusplus
}
#endif

#endif  /* OPS_TIME_H_ */
