#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
/*
create and initialize a region with a given name and givn size.size>0 return false on error. call rchoose() if success
*/

typedef struct TABLE_REGIONS table_r;
struct TABLE_REGIONS
{
	void *pointer = NULL;
	const char * name = NULL;
	r_size_t size;
};
typedef struct FREE_REGIONS table_f;
struct FREE_REGIONS
{
	void *pointer = NULL;
	int diff = 0;
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
table_r *topregion = NULL;
table_r *chooseptr = NULL;
table_f *topfree=NULL;
table_b *topblocks = NULL;


Boolean rinit(const char * region_name, r_size_t region_size)
{
	Boolean result = false;
	table_r newRegion;
	table_b newBlock;
	topregion = malloc(sizeof(newRegion));
	if(topregion!=NULL)
	{
		if(region_size>0)
		{
			newBlock.pointer = malloc(region_size);
			if(newBlock.pointer!=NULL)
			{
				newBlock.name = region_name;
				newBlock.size = region_size;
				newBlock.end = (char*) newBlock->end +newBlock.size;
				newBlock.next = NULL;
				result = true;
			}
		}
	}
	
	return result;
}
/*
choose a previously-ini mem region for subsequent, ralloc, rsize, and rfree calls. return false on error.
*/
Boolean rchoose(const char*region_name)
{
	
}
void ralloc(r_size_t block_size)
{
	
}
Boolean rfree(void *block_ptr)
{
	
}
void rdestroy(const char *region_name)
{
	
}
void rdump()
{
	
}
