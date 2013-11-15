/*
 * Copyright (C) guzai.huang@gmail.com
 */

#include "ops_file.h"

#define BUFF_SIZE (10<<20)

struct  timestamp{
	uint32_t timestamp_s;
	uint32_t timestamp_us;
}__attribute__((packed));

typedef struct timestamp timestamp;

struct pcap_header{
	timestamp ts;
	uint32_t capture_len;
	uint32_t len;
}__attribute__((packed));

typedef struct pcap_header pcap_header;

u_char buffer[BUFF_SIZE];

#define DATA_DIR "/data/datastor/dataspace/"
#define INDEX_DIR "/data/datastor/dataindex/"

int 
read_chunk_from_file (FILE* fd,
					  u_char* chunk,
					  ops_uint_t chunk_size,  
					  ops_uint_t *read_len,
					  ops_int_t back_pos){
	ops_int_t len;
	fseek(fd, back_pos, SEEK_CUR);
	len = fread(chunk, 1, chunk_size, fd);
	*read_len = len;

	if (ferror(fd) != 0){
		return -1;
	}
	if (feof(fd) != 0){
		return -2;
	}

	return 0;
}

void 
handle_file_data(FILE *fd){
	//const char* path = "144555";
	//FILE *fd = fopen(path, "rb");
	ops_int_t  read_size = 0;
	int ret = 0;
	pcap_header *pcap;
	ops_int_t  reading_size = 0;
	ops_int_t back_off = 0;

	do{
		ret = read_chunk_from_file(fd, buffer, BUFF_SIZE, &read_size, back_off);
  		
  		while((read_size - reading_size) >= 16){
  			pcap = (pcap_header*)(buffer + reading_size);
  			//printf("%u:%u:%u\n", pcap->ts.timestamp_s, pcap->ts.timestamp_us, pcap->capture_len);  	

  			if ((read_size - reading_size) < (pcap->capture_len + 16){
  				break;
  			}
  			reading_size += pcap->capture_len + 16;
  		}  	

  		back_off = reading_size - read_size;
        printf("finish one chunk %d:%d:%d:%d\n", reading_size, read_size, back_off, ret);
		reading_size = read_size = 0;

        if ((ret == -2) && (back_off != 0)){
            continue;
        }
  		if (ret != 0){
  			fclose(fd);
  			break;
  		}  	

	}while(1);
    
    printf("finish one file\n");
	
}

static inline const char* 
dir2pathname(const char* dir, char* pathname, uint32_t tv_now){
	char tv_str[20];

	strcpy(pathname, dir);
	sprintf(tv_str, "%u", tv_now);
	strcat(pathname, tv_str);

	return pathname;
}

uint32_t 
handle_dir_data(const char* data_dir, const char* index_dir, int *goon, uint32_t tv_sec){
	char dir_str[1024];
	FILE* fd = NULL;
	struct timeval tv;
	//gettimeofday(&tv, NULL);
	uint32_t tv_now = tv_sec;

	dir2pathname(index_dir, dir_str, tv_now);
	fd = fopen(dir_str, "rb");

	while((*goon) > 0 && (!fd) ){
		if (!fd){
			//printf("%s[open error]\n", dir_str);
			usleep(10000);
		}
		gettimeofday(&tv, NULL);
		tv_now = (tv.tv_sec/60)*60 - 60;
        if (tv_now < tv_sec){
            continue;
        }
		dir2pathname(index_dir, dir_str, tv_now);
		fd = fopen(dir_str, "rb");
        //printf("*****\n");
	}
    fclose(fd); 
	dir2pathname(data_dir, dir_str, tv_now);
	fd = fopen(dir_str, "rb");

	if (fd){
        printf("%s\n", dir_str);
		handle_file_data(fd);
	}

    return tv_now;
}
