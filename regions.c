#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "regions.h"
/*
create and initialize a region with a given name and givn size.size>0 return false on error. call rchoose() if success
*/



table_r *topregion = NULL;
table_r *chooseptr = NULL;
table_b * workBlock = NULL;
int size = 0;


Boolean rinit(const char * region_name, r_size_t region_size)
{
	Boolean result = false;
	if(region_size != 0)
	{
		assert(region_size >0);
		table_r *newRegion;
		table_b *newBlock = (table_b*)malloc(sizeof(table_b));//create a new block node to store first free pointer
		table_f *newFree = (table_f*)malloc(sizeof(table_f));//create a new free node to store starting pointer
		assert(newBlock != NULL);//check if malloc worked.
		assert(newFree != NULL);
		newRegion = (table_r*)malloc(sizeof(newRegion));// create region to store info

		assert(newRegion!=NULL);//check if malloc of newRegions worked
		if(newBlock != NULL)// add newBlock to the block list inside region struct
		{
			assert(newBlock!=NULL);
			(newRegion->topBlocks) = newBlock;//linked top pointer to newBlock
			workBlock = newBlock;// keeps a pointer to current block that still needs to fill in info.
			assert((newRegion->topBlocks) !=NULL);
			assert((newRegion->topBlocks) == newBlock);

		}
		if(newFree != NULL)//check if newFree properly allocated and store the free starting pointer.
		{
			assert(newFree != NULL);
			(newRegion->topFree) = newFree;
			assert((newRegion->topFree)!=NULL);
			assert((newRegion->topFree) == newFree);
		}

		if(newRegion!=NULL)
		{
			assert(newRegion != NULL);
			assert(region_size>0);
			if(region_size>0)//check region size >0 and round up
			{
				r_size_t remainder = region_size % 8;
				assert(remainder>=0);
				if(remainder>0)
				{
					region_size = roundup(region_size);
					assert(region_size>=0);
				}
				newBlock->start = malloc(region_size);//allocate the region_size and send it to the start point of newBLocks and free
				assert(newBlock->start!=NULL);
				newFree->start = (newBlock->start);
				newRegion->name = region_name;
				newRegion->size = region_size;
				if(topregion==NULL)
				{
					topregion = newRegion;
					chooseptr = topregion;
					result = true;
					size++;
				}
				else
				{
					table_r * temp = topregion;
					newRegion->next = temp;
					topregion = newRegion;
					chooseptr = topregion;
					result = true;
					size++;
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
	if(strcmp(region_name,rchosen())== 0)
	{
		result = true;
	}
	else
	{
		table_r * curr = topregion;
		while(curr!=NULL && strcmp(curr->name, region_name)!=0)
		{
			curr = curr->next;
		}
		if(curr!=NULL && strcmp(curr->name, region_name)==0)
		{
			assert(curr!= NULL);
			assert(strcmp(curr->name, region_name)==0);
			result = true;
		}
		else if(curr == NULL && strcmp(curr->name, region_name)==0)
		{
			printf("Something went wrong with searching!\n");
		}

	}
	return result;
}
const char * rchosen()
{
	assert(chooseptr!=NULL);
	return chooseptr->name;
}

void *ralloc(r_size_t block_size)
{
	assert(block_size !=0);//check block size is not 0
	void * result= NULL;//defaul result if we do not allocate correctly
	char * iterateptr = workBlock->start;//casted pointer to zero out the memory in the region.
	if(block_size !=0)// check if block_size is greater does no equal zero
	{
		if(block_size>0 && block_size < chooseptr->size)//check if block_size is not a negative number
		{
			block_size = roundup(block_size);// rounds up the block_size to the nearest multiple of 8
			assert(block_size >0);
			for(int i = 0; i<block_size; i++)
			{
				iterateptr = 0;//zeroes the memory 
				iterateptr+=1;// increases the pointer to the size of the block
			}
			//NEEDS TO BE CHANGED IN ORDER TO USE THE FREE LINKED LIST!!!!!!!!!!!!!!!!!!!!!
			/*


			*/
			workBlock -> end = iterateptr;
			workBlock -> size = block_size;
			chooseptr->size = (chooseptr->size)-(block_size);
			assert(chooseptr->size>=0);
			result = workBlock->start;
		}
	}
	return result;
}
r_size_t rsize(void *block_ptr)
{
	table_b * curr = chooseptr->topBlocks;//takes temp pointer to interate through list
	assert(curr != NULL);
	r_size_t result = 0;//default return size if block_ptr does not match any node.
	while(curr != NULL && (block_ptr!= curr->start))//while loop to iterate through the linked list and find same pointer.
	{
		curr = curr->next;
	}
	if(curr != NULL &&(block_ptr== curr->start))//conditional statement to check if Node exists and memory of the buffers is the same;
	{
		result = curr->size;
		assert(result>=0);
		assert(result >0);
	}
	assert(result >=0);//check if returning actual size and no overrun number
	return result;
}
Boolean rfree(void *block_ptr)
{
	Boolean result = false;
	table_b * curr = chooseptr->topBlocks;
	table_b * prev = NULL;

	while(curr != NULL && (block_ptr!= curr->start))
	{
		prev = curr;
		curr = curr->next;
	}
	if(curr!=NULL&&(block_ptr== curr->start) )//when we find the buffer we want to delete
	{
		addFree(curr);
		prev = curr->next;//takes previous node from block list and links around current node
		assert(prev = curr->next);//checks to see if linked properly.
		free(curr);//free's the malloced memory/Node
		result = true;//successfully  freed block of memory.
	}
	return result;

}
void rdestroy(const char *region_name)
{
	// table_r *curr = topregion;
	// assert(topregion!=NULL);

}
void rdump()
{
	
}
r_size_t roundup(r_size_t size)
{
	assert(size>0);
	if(size!= 0)
	{
		r_size_t remainder = size%8;
		assert(remainder>=0);
		if(remainder > 0)
		{
			r_size_t diff = 8-remainder;
			size += diff;
			assert(size>0);
		}
	}
	return size;
}
Boolean addFree(table_b *curr)
{
	Boolean result = false;
	table_f* newFree = (table_f*) malloc( sizeof(table_f));//creates index of location for next free region;
	assert(newFree != NULL);// checks if malloc allocated properly
	assert(curr->size>0);
	newFree->diff = curr->size;//stores the size of freed memory into newFree
	assert(newFree->diff == curr->size);
	if(chooseptr->topFree == NULL)//if free index list is empty
	{
		assert(newFree !=NULL);
		chooseptr->topFree = newFree; //have topFree point to newFree.
		assert(chooseptr->topFree == newFree);
		result = true;
	}
	else//when has something in it add it to the top of the list.
	{
		assert(newFree!=NULL);
		assert(chooseptr->topFree!=NULL);
		table_f *temp = chooseptr->topFree;//takes the current top Node and stores in temp
		newFree->next = temp;//takes thew newFree and links it above current Free Node.
		chooseptr->topFree = newFree;//takes the topFree pointer and points it to newFree
		assert(chooseptr->topFree == newFree);
		result = true;
	}
	return result;
}