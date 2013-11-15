/*
 * Copyright (C) guzai.huang@gmail.com
 */

#include "ops_times.h"

void
ops_gmtime(time_t t, ops_tm_t *tp){
	ops_int_t  yday;
	ops_uint_t  n, sec, min, hour, mday, mon, year, wday, days, leap;

	/* the calculation is valid for positive time_t only */
	n = (ops_uint_t)t;
	days = n / 86400;

	/* January 1, 1970 was Thursday*/
	wday = (4 + days) % 7;
	n %= 86400;
	hour = n / 3600;
	n %= 3600;
	min = n / 60;
	sec = n % 60;

	/*
     * the algorithm based on Gauss' formula,
     * see src/http/ngx_http_parse_time.c
     */

    /* days since March 1, 1 BC */
    days = days - (31 + 28) + 719527;

    /*
     * The "days" should be adjusted to 1 only, however, some March 1st's go
     * to previous year, so we adjust them to 2.  This causes also shift of the
     * last February days to next year, but we catch the case when "yday"
     * becomes negative.
     */

    year = (days + 2) * 400 / (365 * 400 + 100 - 4 + 1);

    yday = days - (365 * year + year / 4 - year / 100 + year / 400);

    if (yday < 0) {
        leap = (year % 4 == 0) && (year % 100 || (year % 400 == 0));
        yday = 365 + leap + yday;
        year--;
    }

    /*
     * The empirical formula that maps "yday" to month.
     * There are at least 10 variants, some of them are:
     *     mon = (yday + 31) * 15 / 459
     *     mon = (yday + 31) * 17 / 520
     *     mon = (yday + 31) * 20 / 612
     */

    mon = (yday + 31) * 10 / 306;

    /* the Gauss' formula that evaluates days before the month */

    mday = yday - (367 * mon / 12 - 30) + 1;

    if (yday >= 306) {

        year++;
        mon -= 10;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday -= 306;
         */

    } else {

        mon += 2;

        /*
         * there is no "yday" in Win32 SYSTEMTIME
         *
         * yday += 31 + 28 + leap;
         */
    }

    tp->ops_tm_sec = (ops_tm_sec_t) sec;
    tp->ops_tm_min = (ops_tm_min_t) min;
    tp->ops_tm_hour = (ops_tm_hour_t) hour;
    tp->ops_tm_mday = (ops_tm_mday_t) mday;
    tp->ops_tm_mon = (ops_tm_mon_t) mon;
    tp->ops_tm_year = (ops_tm_year_t) year;
    tp->ops_tm_wday = (ops_tm_wday_t) wday;
}

char* ops_str_time(char* buf, time_t t){
    ops_tm_t tm;

    ops_gmtime(t, &tm);

    return sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d",
                        tm.ops_tm_year,
                        tm.ops_tm_mon,
                        tm.ops_tm_mday,
                        tm.ops_tm_hour,
                        tm.ops_tm_min,
                        tm.ops_tm_sec);
}
