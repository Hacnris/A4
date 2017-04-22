#ifndef _REGIONS_H
#define _REGIONS_H

typedef enum { false, true } Boolean;

typedef unsigned short r_size_t;

Boolean rinit(const char *region_name, r_size_t region_size);
Boolean rchoose(const char *region_name);
const char *rchosen();
void *ralloc(r_size_t block_size);
r_size_t rsize(void *block_ptr);
Boolean rfree(void *block_ptr);
void rdestroy(const char *region_name);
void rdump();


//======Added prototypes===========

typedef struct REGION_BLOCKS table_b;
struct REGION_BLOCKS
{
	unsigned char *start;
	r_size_t size;
	unsigned char *end;
	table_b *next;
};
typedef struct TABLE_REGIONS table_r;
struct TABLE_REGIONS
{
	const char * name;
	unsigned char * start;
	unsigned char * end;
	r_size_t size;
	r_size_t remaining;
	table_r *next;
	table_b * topBlocks;
	int sizeb;
};
Boolean search(const char*region_name);
r_size_t roundup(r_size_t size);
Boolean destroyBlocks(table_b* top);
void zeroOut(r_size_t size, unsigned char * start);
#endif
