/*
 * Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_CONFIG_H_
#define OPS_CONFIG_H_

#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>
//#include <stddef.h>             /* offsetof() */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <glob.h>
#include <sys/vfs.h>            /* statfs() */

#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sched.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>        /* TCP_NODELAY, TCP_CORK */
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>

#include <time.h>               /* tzset() */
#include <malloc.h>             /* memalign() */
#include <limits.h>             /* IOV_MAX */
#include <sys/ioctl.h>
#include <crypt.h>
#include <sys/utsname.h>        /* uname() */

 

typedef  intptr_t   ops_int_t;
typedef  uintptr_t  ops_uint_t;
typedef  intptr_t   ops_flag_t;

#define ops_align(d, a) (((d) + (a -1)) & ~(a - 1))
#define ops_align_ptr(p, a)  \
	(u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

/* TODO: auto_conf: ops_inline   inline __inline __inline__ */
#ifndef ops_inline
#define ops_inline      inline
#endif

#if ((__GNU__ == 2) && (__GNUC_MINOR__ < 8))
#define OPS_MAX_UINT32_VALUE  (uint32_t) 0xffffffffLL
#else
#define OPS_MAX_UINT32_VALUE  (uint32_t) 0xffffffff
#endif

#define OPS_MAX_INT32_VALUE   (uint32_t) 0x7fffffff

#endif /* OPS_CONFIG_H_ */
