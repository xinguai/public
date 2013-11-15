#ifndef OPS_MYSQL_H_
#define OPS_MYSQL_H_

/*
 * Copyright (C) guzai.huang@gmail.com
 */

#include <my_global.h>
#include <mysql.h>

MYSQL* 
ops_mysql_init(const char* host, const char* user_name, const char* passwd);

int 
ops_commit_transaction(MYSQL*conn, const char* query);

int
ops_insert_app(MYSQL* conn, u_char *data, size_t len);

int 
ops_insert_qoe(MYSQL* conn, u_char *data, size_t len);

int
ops_insert_usr(MYSQL* conn, u_char *data, size_t len);

void ops_mysql_test(MYSQL *conn);

#endif /* OPS_MYSQL_H_ */
