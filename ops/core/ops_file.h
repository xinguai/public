/*
 *	Copyright (C) guzai.huang@gmail.com
 */

#ifndef OPS_FILE_H_
#define OPS_FILE_H_

#include <stdint.h>
#include "ops_config.h"

#ifdef __cplusplus
extern "C"{
#endif  /* __cplusplus */

int read_chunk_from_file (FILE* fd,
						  u_char* chunk,
						  ops_uint_t chunk_size, 
						  ops_uint_t *read_len,
						  ops_int_t back_pos);

void handle_file_data(FILE *fd);

uint32_t handle_dir_data(const char* data_dir, const char* index_dir, int* goon, uint32_t tv_sec);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* OPS_FILE_H_ */
