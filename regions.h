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

typedef struct FREE_REGIONS table_f;
struct FREE_REGIONS
{
	void *start;
	int diff;
	table_f *next;
};
typedef struct REGION_BLOCKS table_b;
struct REGION_BLOCKS
{
	void *start;
	r_size_t size;
	void *end;// how to calculate it?
	table_b *next;
};
typedef struct TABLE_REGIONS table_r;
struct TABLE_REGIONS
{
	const char * name;
	r_size_t size;
	table_r *next;
	table_b * topBlocks;
	table_f * topFree;
};
Boolean search(const char*region_name);
r_size_t roundup(r_size_t size);
Boolean addFree(table_b *curr);
#endif
