/*
 * Copyright (C) guzai.huang@gmail.com
 */

#include "ops_mysql.h"
#include "ops_times.h"

 static const char * create_app = " \
 CREATE TABLE app( \
 src_ip INT UNSIGNED,\
 src_port SMALLINT UNSIGNED,\
 dst_ip INT UNSIGNED,\
 dst_port SMALLINT UNSIGNED,\
 probe_if INT UNSIGNED,\
 app_style INT UNSIGNED,\
 app_id    INT UNSIGNED,\
 app_group_id INT UNSIGNED,\
 vlan_id    INT,\
 start_tm_sec INT UNSIGNED,\
 start_tm_usec INT UNSIGNED,\
 last_update_tm_sec INT UNSIGNED,\
 last_update_tm_usec INT UNSIGNED,\
 end_tm_sec     INT UNSIGNED,\
 end_tm_usec    INT UNSIGNED,\
 input_packets  INT UNSIGNED,\
 output_packets INT UNSIGNED,\
 input_bytes    INT UNSIGNED,\
 output_bytes   INT UNSIGNED,\
 protocol       CHAR(20),\
 data_tm        DATETIME \
 );";

/* 20 */
typedef struct ops_app{
	uint32_t  src_ip;
	uint16_t  src_port;
	uint32_t  dst_ip;
	uint16_t  dst_port;
	uint32_t  probe_if;
	uint32_t  app_style;
	uint32_t  app_id;
	uint32_t  app_group_id;
	int32_t   vlan_id;
	uint32_t  start_tm_sec;
	uint32_t  start_tm_usec;
	uint32_t  last_update_tm_sec;
	uint32_t  last_update_tm_usec;
	uint32_t  end_tm_sec;
	uint32_t  end_tm_usec;
	uint32_t  input_packets;
	uint32_t  output_packets;
	uint32_t  input_bytes;
	uint32_t  output_bytes;
	char protocol[20];
}ops_app;

static const char *create_qoe = "\
CREATE TABLE qoe(\
src_ip 						INT UNSIGNED,\
dst_ip 						INT UNSIGNED,\
probe_if 					INT UNSIGNED,\
app_style 					INT UNSIGNED,\
app_id    					INT UNSIGNED,\
app_group_id 				INT UNSIGNED,\
start_tm_sec 				INT UNSIGNED,\
start_tm_usec 				INT UNSIGNED,\
last_update_tm_sec 			INT UNSIGNED,\
last_update_tm_usec 		INT UNSIGNED,\
respon_num                  INT UNSIGNED,\
fast_num                    INT UNSIGNED,\
fast_expected_num           INT UNSIGNED,\
expected_degrated_num       INT UNSIGNED,\
degrated_service_num        INT UNSIGNED, \
service_availability_num    INT UNSIGNED,\
respon_timeout_num          INT UNSIGNED,\
respon_success_num          INT UNSIGNED,\
respon_fail_num             INT UNSIGNED,\
respon_peek_tm              INT UNSIGNED,\
app_respon_tm               INT UNSIGNED,\
cs_window                   INT UNSIGNED,\
sc_window                   INT UNSIGNED,\
cs_reset                    INT UNSIGNED,\
sc_reset                    INT UNSIGNED, \
cs_retran                   INT UNSIGNED,\
sc_retran                   INT UNSIGNED, \
user_events                 INT UNSIGNED, \
server_events               INT UNSIGNED,\
conn_setup_tm               INT UNSIGNED, \
data_transfer_tm            INT UNSIGNED,\
retrans_delay_tm            INT UNSIGNED, \
network_inbound             INT UNSIGNED,\
network_outbound            INT UNSIGNED, \
new_session                 INT UNSIGNED, \
conn_setup_peek             INT UNSIGNED,\
data_tm                     DATETIME\
);";

/* 36 */
typedef struct ops_qoe{
uint32_t src_ip; 					
uint32_t dst_ip; 					
uint32_t probe_if; 				
uint32_t app_style; 			
uint32_t app_id;    				
uint32_t app_group_id; 
uint32_t start_tm_sec; 			
uint32_t start_tm_usec; 
uint32_t last_update_tm_sec;
uint32_t last_update_tm_usec; 		
uint32_t respon_num;              
uint32_t fast_num;                   
uint32_t fast_expected_num;         
uint32_t expected_degrated_num;  
uint32_t degrated_service_num;    
uint32_t service_availability_num;
uint32_t respon_timeout_num;     
uint32_t respon_success_num;     
uint32_t respon_fail_num;          
uint32_t respon_peek_tm;          
uint32_t app_respon_tm;          
uint32_t cs_window;              
uint32_t sc_window;              
uint32_t cs_reset;              
uint32_t sc_reset;              
uint32_t cs_retran;              
uint32_t sc_retran;              
uint32_t user_events;            
uint32_t server_events;          
uint32_t conn_setup_tm;          
uint32_t data_transfer_tm;       
uint32_t retrans_delay_tm;       
uint32_t network_inbound;        
uint32_t network_outbound;       
uint32_t new_session;            
uint32_t conn_setup_peek;      

}ops_qoe;

/* 16 */
static const char* create_usr = "\
CREATE TABLE usr(\
probe_if     INT UNSIGNED,\
addr_src     INT UNSIGNED,\
port_src     SMALLINT UNSIGNED,\
url          CHAR(100),\
return_code  INT UNSIGNED,\
respon_time  INT,\
start_time_sec INT UNSIGNED,\
start_time_usec INT UNSIGNED,\
end_time_sec   INT UNSIGNED,\
end_time_usec  INT UNSIGNED,\
browser        CHAR(30),\
os             CHAR(30),\
page_size      INT UNSIGNED,\
network_time   INT UNSIGNED,\
server_ip      INT UNSIGNED,\
web_id         INT UNSIGNED,\
data_tm        DATETIME\
);";

typedef struct ops_usr{
	uint32_t probe_if;
	uint32_t addr_src;           
	uint16_t port_src;   
	char url[100];        
	uint32_t return_code;   
	uint32_t respon_time;      
	uint32_t start_time_sec;
	uint32_t start_time_usec;              
	uint32_t end_time_sec;
	uint32_t end_time_usec;       
	char browser[30];        
	char os[30];
	uint32_t page_size;   
	uint32_t network_time;    
	uint32_t server_ip;    
	uint32_t web_id;
}ops_usr;

static void 
finish_with_error(MYSQL *conn){
	fprintf(stderr, "%s\n", mysql_error(conn));
	mysql_close(conn);
	exit(1);
}

static void 
ops_create_tables(MYSQL *conn){
	if (mysql_query(conn, create_app)){
		//finish_with_error(conn);
	}
	if (mysql_query(conn, create_qoe)){
		//finish_with_error(conn);
	}
	if (mysql_query(conn, create_usr)){
		//finish_with_error(conn);
	}
}

MYSQL* 
ops_mysql_init(const char* host, const char* user_name, const char* passwd){
	MYSQL* conn = mysql_init(NULL);

	if (!conn){
		finish_with_error(conn);
	}

	if (mysql_real_connect(conn, host, user_name, passwd, NULL, 0, NULL, 
						   CLIENT_MULTI_STATEMENTS) == NULL){
		finish_with_error(conn);
	}

	if (mysql_query(conn, "USE DATABASE dataclone")){
		mysql_query(conn, "CREATE DATABASE dataclone");
		mysql_query(conn, "USE dataclone");
		ops_create_tables(conn);
	}

	return conn;
}

int 
ops_commit_transaction(MYSQL* conn, const char* query){
	int ret;

	ret = mysql_real_query(conn, "SET AUTOCOMMIT =0", strlen("SET AUTOCOMMIT =0"));
	if (ret){
		printf("%s\n", "set auto commit failed");
		return ret;
	}

	ret = mysql_real_query(conn, "BEGIN ;", strlen("BEGIN ;"));
	ret = mysql_real_query(conn, query, strlen(query));
	if (ret){
		printf("%s:query failed\n", query);
		//return ret;
	}

	if (ret){
		ret = mysql_real_query(conn, "COMMIT ;", strlen("COMMIT ;"));
	}else {
		ret = mysql_real_query(conn, "ROLLBACK ;", strlen("ROLLBACK ;"));
	}

	ret = mysql_real_query(conn, "SET AUTOCOMMIT =1", strlen("SET AUTOCOMMIT =1"));

	return ret;

}

int
ops_insert_app(MYSQL* conn, u_char *data, size_t len){
	size_t length = len;
	size_t elt_size = sizeof(ops_app);
	size_t elt_num = 0;
	char query[1024*1024];
	char row[512];
	ops_app *app;
    int ret;
    char date[128];
    time_t tval;
    struct tm* local;

	tval = time(NULL);
	local = localtime(&tval);
	strftime(date, 64, "%Y-%m-%d %H:%M:%S", local);
	strcpy(query, "INSERT INTO app VALUES ");

	while(length >= elt_size){
		app = (ops_app*)(data + elt_num*elt_size);
	   // ops_str_time(date, (time_t)(tval.tv_sec));
		sprintf(row, "(%u,%u,%u,%u,%u,%u,%u,%u,%d,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u, \'%s\', \'%s\')", 
				app->src_ip,
				app->src_port,
				app->dst_ip,
				app->dst_port,
				app->probe_if,
				app->app_style,
				app->app_id,
				app->app_group_id,
				app->vlan_id,
				app->start_tm_sec,
				app->start_tm_usec,
				app->last_update_tm_sec,
				app->last_update_tm_usec,
				app->end_tm_sec,
				app->end_tm_usec,
				app->input_packets,
				app->output_packets,
				app->input_bytes,
				app->output_bytes,
				app->protocol,
				date
				);
		elt_num++;
		length -= elt_size*elt_num;
		strcat(query, row);

		if ((elt_num %2048 == 0) || (length == 0)){
			strcat(query, ";");
			ret = mysql_real_query(conn, query, strlen(query));
			if (ret){
				return ret;
			}
			strcpy(query, "INSERT INTO app VALUES ");

		}else{
			strcat(query, ",");
		}
	}

	/*if ((elt_num % 2048) != 0){
		strcat(query, ";");
		ret = mysql_real_query(conn, query, strlen(query));
	}*/

	return ret;
}

int
ops_insert_qoe(MYSQL* conn, u_char *data, size_t len){
	size_t length = len;
	size_t elt_size = sizeof(ops_qoe);
	size_t elt_num = 0;
	char query[1024*1024];
	char row[512];
	ops_qoe *qoe;
    int ret;
	char date[128];
    struct tm* local;
    time_t tval;

	tval = time(NULL);
	local = localtime(&tval);
	strftime(date, 64, "%Y-%m-%d %H:%M:%S", local);
	strcpy(query, "INSERT INTO qoe VALUES ");

	while(length >= elt_size){
		qoe = (ops_qoe*)(data + elt_num *elt_size);
		//ops_str_time(date, (time_t)(tval.tv_sec));
		sprintf(row, "(%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u, %u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u, \'%s\')",
				        qoe->src_ip,					
						qoe->dst_ip, 					
						qoe->probe_if,				
						qoe->app_style,			
						qoe->app_id,
						qoe->app_group_id, 
						qoe->start_tm_sec,
						qoe->start_tm_usec, 
						qoe->last_update_tm_sec,
						qoe->last_update_tm_usec, 		
						qoe->respon_num,              
						qoe->fast_num,                   
						qoe->fast_expected_num,         
						qoe->expected_degrated_num,
						qoe->degrated_service_num,    
						qoe->service_availability_num,
						qoe->respon_timeout_num,
						qoe->respon_success_num,    
						qoe->respon_fail_num,          
						qoe->respon_peek_tm,          
						qoe->app_respon_tm,          
						qoe->cs_window,              
						qoe->sc_window,              
						qoe->cs_reset,              
						qoe->sc_reset,              
						qoe->cs_retran,
						qoe->sc_retran,
						qoe->user_events,           
						qoe->server_events,          
						qoe->conn_setup_tm,         
						qoe->data_transfer_tm,       
						qoe->retrans_delay_tm,       
						qoe->network_inbound,        
						qoe->network_outbound,       
						qoe->new_session,            
						qoe->conn_setup_peek,
						date
				);
		elt_num++;
		length -= elt_size*elt_num;
		strcat(query, row);
		if ((elt_num %2048 == 0) || length == 0){
			strcat(query, ";");
			ret = mysql_real_query(conn, query, strlen(query));
			if (ret){
				return ret;
			}
			strcpy(query, "INSERT INTO app VALUES ");

		}else{
			strcat(query, ",");
		}
	}

	/*if ((elt_num % 2048) != 0){
		strcat(query, ";");
		ret = mysql_real_query(conn, query, strlen(query));
	}*/

	return ret;
}

int
ops_insert_usr(MYSQL* conn, u_char *data, size_t len){
	size_t length = len;
	size_t elt_size = sizeof(ops_usr);
	size_t elt_num = 0;
	char query[1024*1024];
	char row[512];
	ops_usr *usr;
	time_t  tval;
    int ret;
    char date[128];
    struct tm* local;

	tval = time(NULL);
	local = localtime(&tval);
	strftime(date, 64, "%Y-%m-%d %H:%M:%S", local);
	strcpy(query, "INSERT INTO usr VALUES ");

	while(length >= elt_size){
		usr = (ops_usr*)(data + elt_size * elt_num);
		//ops_str_time(date, (time_t)(tval.tv_sec));
		sprintf(row, "(%u,%u,%u,\'%s\',%u,%u,%u,%u,%u,%u,\'%s\',\'%s\',%u,%u,%u,%u, \'%s\')",
					  usr->probe_if,
					  usr->addr_src,           
      				  usr->port_src,   
      				  usr->url,        
      				  usr->return_code, 
      				  usr->respon_time,      
      				  usr->start_time_sec,
      				  usr->start_time_usec,              
      				  usr->end_time_sec,
      				  usr->end_time_usec,     
      				  usr->browser,        
      				  usr->os,
      				  usr->page_size,   
      				  usr->network_time,    
      				  usr->server_ip,    
      				  usr->web_id,
      				  date
			    );
		elt_num++;
		length -= elt_size*elt_num;
		strcat(query, row);
		if ((elt_num %2048 == 0) || (length == 0)){
			strcat(query, ";");
			ret = mysql_real_query(conn, query, strlen(query));
			if (ret){
				return ret;
			}
			strcpy(query, "INSERT INTO app VALUES ");

		}else{
			strcat(query, ",");
	        }
	}

	/*if ((elt_num % 2048) != 0){
		strcat(query, ";");
		ret = mysql_real_query(conn, query, strlen(query));
	}*/

	return ret;
}


void ops_mysql_test(MYSQL *conn){
	ops_app app;
	ops_qoe qoe;
	ops_usr usr;

	memset(&app, 0,sizeof(app));
	memset(&qoe, 0, sizeof(qoe));
	memset(&usr, 0, sizeof(usr));
	ops_insert_app(conn, (u_char*)&app, sizeof(app));
	ops_insert_qoe(conn, (u_char*)&qoe, sizeof(qoe));
	ops_insert_usr(conn, (u_char*)&usr, sizeof(usr));
}
