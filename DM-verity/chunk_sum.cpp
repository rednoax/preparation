#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))
const char sparse_header_str[] = "Sparse Image Header";
//const char chunk_header_str[] = "Chunk Header";
const char total_chunks_str[] = "total_chunks: ";

const char chunk_type_str[] = "chunk_type: ";
const char chunk_data_sz_str[] = "chunk_data_sz: ";
const char total_size_str[] = "total_size: ";
const char* chunk_str[] = {chunk_type_str, chunk_data_sz_str, total_size_str};

struct chunk_header {
#if 0	
	unsigned short chunk_type;
	unsigned int chunk_sz;//unit:4KB
	unsigned int total_sz;
#else
	unsigned int entry[ARRAY_SIZE(chunk_str)];
	unsigned long long base;
	unsigned long long size;
#endif
};

struct sparse_image {
	unsigned int total_chunks;
	struct chunk_header *headers;
};

#define CHUNK_TYPE_RAW		0xCAC1
#define CHUNK_TYPE_FILL		0xCAC2
#define CHUNK_TYPE_DONT_CARE	0xCAC3
#define CHUNK_TYPE_CRC		0xCAC4

struct sparse_image images[16];
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t line_len = 0;
	ssize_t len;
	unsigned int i, j, k, index, cur;
	unsigned int blk_sz = 4096;
	const char *p;
	struct sparse_image *img;	
	struct chunk_header *head;
	unsigned int total_blocks, type;
	FILE *fp = fopen(argv[1], "r");

	if (argc < 2) {
		printf("usage: %s filename_to_parse\n", argv[0]);
		return 0;
	}
	if (!fp) {
		printf("open %s err\n", argv[1]);
		return 0;
	}
	index = 0;
	while ((len = getline(&line, &line_len, fp)) != -1) {
		if (strstr(line, sparse_header_str)) {
			cur = index++;
		}  else if ((p = strstr(line, total_chunks_str))) {
			total_blocks = 0;
			img = &images[cur];
			p += strlen(total_chunks_str);
			img->total_chunks = atoi(p);
			img->headers = (struct chunk_header*)calloc(img->total_chunks, sizeof(struct chunk_header));
			for (i = 0; i < img->total_chunks; i++) {
				/*
				[33760] total_chunks: 3622
				[33760] === Chunk Header ===
				[33760] chunk_type: 0xcac1
				[33770] chunk_data_sz: 0x1
				[33770] total_size: 0x100c
				[33770] === Chunk Header ===
				*/
				head = &img->headers[i];
				getline(&line, &line_len, fp);//
				for (j = 0; j < ARRAY_SIZE(chunk_str); j++) {
					getline(&line, &line_len, fp);
					p = strstr(line, chunk_str[j]);
					head->entry[j] = strtol(p + strlen(chunk_str[j]), NULL, 16);
				}
				type = head->entry[0];
				switch(type)
				{
				case CHUNK_TYPE_RAW:
				case CHUNK_TYPE_DONT_CARE:
					head->base = total_blocks;
					total_blocks += head->entry[1];
					break;
				default:
					printf("error type %x\n", type);
					exit(1);
				}
			}
		}
	}
	for (i = 0; i < index; i++) {
		img = images + i;
		printf("[%d] has %d chunks\n", i, img->total_chunks);
		for (j = 0; j < img->total_chunks; j++) {
			head = img->headers + j;
			printf("[%d]: ", j);
			type = head->entry[0];
			for (k = 0; k < ARRAY_SIZE(head->entry); k++) {
				printf("%x ", head->entry[k]);
			}
			printf("\t\t %lld %lld", head->base, head->base * blk_sz);
			if (type == CHUNK_TYPE_DONT_CARE)
				printf(")<--------------");
			printf("\n");
		}
		if (type == CHUNK_TYPE_RAW) {
			unsigned long long last = head->base + head->entry[1] - 1;
			printf("%lld %lld] END\n", last, last * blk_sz);
		}
	}
	free(line);
	fclose(fp);
	return 0;
}
