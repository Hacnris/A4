#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "regions.h"
/*
create and initialize a region with a given name and givn size.size>0 return false on error. call rchoose() if success
*/

typedef struct TABLE_REGIONS table_r;
struct TABLE_REGIONS
{
	const char * name;
	r_size_t size;
	table_r *next;
};
typedef struct FREE_REGIONS table_f;
struct FREE_REGIONS
{
	void *pointer;
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
table_r *topregion = NULL;
table_r *chooseptr = NULL;
table_b *topfree=NULL;
table_b *topblocks = NULL;
int size = 0;


Boolean rinit(const char * region_name, r_size_t region_size)
{
	Boolean result = false;
	table_r *newRegion = NULL;
	table_b *newBlock = (table_b*)malloc(sizeof(table_b));
	if(newBlock!=NULL)
	{
		assert(newBlock!=NULL);
		topblocks = newBlock;
		assert(topblocks = newBlock);
	}
	newRegion = (table_r*)malloc(sizeof(newRegion));
	topregion = newRegion;
	if(newRegion!=NULL)
	{
		assert(topregion!=NULL);
		if(region_size>0)
		{
			topblocks = malloc(sizeof(region_size));
			topfree = (table_b*)topblocks;
			newBlock->start = topblocks;
			newRegion->name = region_name;
			newRegion->size = region_size;
			newRegion->next = NULL;
			if(topregion==NULL)
			{
				table_r * curr = topregion;
				topregion = newRegion;
				chooseptr = topregion;
				topregion->next = curr;
				result = true;
				size++;
			}
			else
			{
				topregion = newRegion;
				chooseptr = topregion;
				result = true;
				size++;
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
	printf("size:%d\n", size);
	table_r * curr = topregion;
	while(curr!=NULL && (strcmp(curr->name,region_name)!=0))
	{
		printf("%s\n", curr->name);
		curr = curr->next;
	}
	printf("after:%s\n", curr->name);
	if(curr!=NULL && (strcmp(curr->name,region_name)==0))
	{
		chooseptr = curr;
		result = true;
	}
	else if(curr == NULL&&(strcmp(curr->name,region_name)!=0))
	{
		printf("SOMETHING WENT WRONG TRAVERSING\n");
	}
	return result;
}
const char * rchosen()
{
	return chooseptr->name;
}
void *ralloc(r_size_t block_size)
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
		chooseptr = curr;
		result = true;
	}
	return result;
}