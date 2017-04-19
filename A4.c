#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "table.h"
/*
create and initialize a region with a given name and givn size.size>0 return false on error. call rchoose() if success
*/

typedef struct TABLE_REGIONS table_r;
struct TABLE_REGIONS
{
	void *pointer = NULL;
	const char * name = NULL;
	r_size_t size;
	table_r *next;
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
	table_b newBlock = malloc(sizeof(table_b));
	if(newBlock!=NULL)
	{
		assert(newBlock!=NULL);
		topblock = newBlock;
		assert(topblock = newBlock);
	}
	topregion = malloc(sizeof(newRegion));
	if(topregion!=NULL)
	{
		assert(topregion!=NULL);
		if(region_size>0)
		{
			topblocks = malloc(sizeof(region_size));
			topfree = topblocks;
			newBlock->start = topblocks;
			if(topblocks==NULL)
			{
				newRegion->name = region_name;
				newRegion.size = region_size;
				newRegion->end = (char*) newBlock->end +newBlock.size;
				newRegion->next = NULL;
				chooseptr = newRegion;
				Boolean confirm = rchoose(region_name);
				result = true;
			}
			else
			{
				table_r temp = NULL;
				table_r curr = topregion;
				while(curr!=NULL)
				{
					temp = curr;
					curr = curr->next;
				}
				if(curr == NULL)
				{
					temp->next = newRegion;
					newRegion->name = region_name;
					newRegion.size = region_size;
					newRegion->end = (char*) newBlock->end +newBlock.size;
					newRegion->next = NULL;
					result = true;
				}
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
	Boolean result = false;
	if(strcmp(chooseptr->name, region_name) == 0)
		result = true;
	else
	{
		result = search(region_name);
	}
	return result;
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
Boolean search(const char*region_name)
{
	Boolean result = false;
	table_r * curr = topregion;
	while(curr!= NULL && strcmp(curr->name, region_name)!= 0)
	{
		curr = curr->next;
	}
	if(curr == NULL)
		printf("Search failed! Could not find \"%s\".", region_name);
	else if(strcmp(curr->name, region_name)== 0 && curr != NULL)
	{
		assert(curr != NULL);
		assert(strcmp(curr->name, region_name)== 0);
		result = true;
	}
	return result;
}