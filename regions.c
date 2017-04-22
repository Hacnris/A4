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
int size = 0;


Boolean rinit(const char * region_name, r_size_t region_size)
{
	Boolean result = false;
	if(region_size != 0)
	{
		assert(region_size >0);
		table_r *newRegion;
		newRegion = (table_r*)malloc(sizeof(newRegion));// create region to store info
		assert(newRegion!=NULL);//check if malloc of newRegions worked
		if(newRegion!=NULL)//Condition of malloc of Region
		{
			assert(newRegion != NULL);
			assert(region_size>0);
			if(region_size>0)//check region size >0 and round up
			{
				region_size = roundup(region_size);
				assert(region_size>=0);
				newRegion->start = malloc(region_size);//allocate the region_size and send it to the start point of newBLocks and free
				assert(newRegion->start!=NULL);//checks if the allocation works or not
				newRegion->name = region_name;//stores special region name for searching
				newRegion->size = region_size;//stores the size of the region  allocation
				newRegion->remaining = region_size;//stores memory updater
				newRegion->end = newRegion->start + region_size;
				newRegion->sizeb = 0;
				if(topregion==NULL)//if list is empty just point to the new Region Node
				{
					topregion = newRegion;
					chooseptr = topregion;//auto chooses the new region
					result = true;
				}
				else
				{
					table_r * temp = topregion;//takes the currently top region and stores it to be pointed by new Node
					newRegion->next = temp;//new Node points to the current top Node to get above it
					topregion = newRegion;//top updates to new node
					chooseptr = topregion;//auto choose the new region
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
	table_b *newBlock = malloc(sizeof(table_b));
	assert(newBlock !=NULL);
	if(block_size !=0)// check if block_size is greater does no equal zero
	{
		if(block_size>0 && block_size < chooseptr->remaining)//check if block_size is not a negative number
		{
			assert(block_size>0);
			assert(block_size<chooseptr->remaining);

			if(chooseptr->remaining == chooseptr->size)//condition if no memory used
			{
				printf("Number 1\n");
				newBlock->start = chooseptr->start;//starting buffer in new region or empty region
				zeroOut(block_size, chooseptr->start);//zeroes out memory up to block size
				newBlock->size = block_size;//saves the block size
				newBlock->end =newBlock->start + size;//end point buffer for further calc
				chooseptr->topBlocks = newBlock;//changes top to point to new block
				chooseptr->remaining -=block_size;//updates remaining memory
				chooseptr->sizeb++;
				result = newBlock->start;
			}
			else if(chooseptr->sizeb==1)//if there is only 1 Node
			{
				assert(chooseptr->sizeb ==1);
				assert(chooseptr->sizeb>=0);
				printf("Number2\n");
				table_b * curr = chooseptr->topBlocks;//temp store to be linked later
				newBlock->start = curr->end;//newBlock start == end of the first node;
				assert(newBlock->start!=NULL);
				assert(newBlock->start == curr->end);
				zeroOut(block_size,newBlock->start);//zero out memory up to the blocksize
				newBlock->size = block_size;//store block size 
				assert(newBlock->size >0);
				newBlock->end =newBlock->start + size;//store the end for further calc
				assert(newBlock->end !=NULL);
				newBlock->next = curr;// link newBlock to current top
				chooseptr->topBlocks = newBlock;//update top to point to new top
				chooseptr->remaining -=block_size;
				chooseptr->sizeb++;
				result = newBlock->start;
			}
			else
			{
				printf("Number3\n");
				table_b* curr = chooseptr->topBlocks;
				table_b* after = curr->next;
				unsigned int diff = 0;
				if(curr!=NULL && after !=NULL)
				{
					diff = curr->start-after->end;
					printf("diff1:%u\n", diff);
				}

				while((curr!=NULL && after!=NULL) && block_size>diff)
				{
					curr = curr->next;
					after = after->next;
					diff = curr->start-after->end;
					printf("NODE SIZE: %d\n", curr->size);
					printf("diff:%u\n", diff);
				}
				if((curr!=NULL && after==NULL))//if at the end of the list and remaining memory can store block
				{
					unsigned int endDiff = chooseptr->end -curr->end;
					if(block_size<endDiff)
					{
						printf("END TAIL\n");
						assert(curr!=NULL);
						table_b * temp = curr;
						newBlock->start = curr->end;
						assert(newBlock->start !=NULL);
						assert(newBlock->start == curr->end);
						zeroOut(block_size, newBlock->start);
						newBlock->size = block_size;
						assert(newBlock->size >0);
						newBlock->end =newBlock->start + size;
						assert(newBlock->end !=NULL);
						newBlock->next = temp;
						chooseptr->topBlocks = newBlock;
						chooseptr->remaining -=block_size;
					}
					
					result = newBlock->start;
				}
				else if((curr!=NULL && after!=NULL) && (block_size<=diff))//if there is gap big enough to store block do so
				{
					printf("GAP INSERT\n");

					assert(diff>0);
					assert(curr !=NULL);
					assert(after !=NULL);
					assert(block_size<diff);
					newBlock->start = curr->end;
					assert(newBlock->start!=NULL);
					assert(newBlock->start == curr->end);
					zeroOut(block_size,newBlock->start);
					newBlock->size = block_size;
					assert(newBlock->size >0);
					newBlock->end =newBlock->start + size;
					assert(newBlock->end !=NULL);
					assert(newBlock->end<=after->start);
					newBlock->next = after;
					curr->next = newBlock;
					chooseptr->remaining -=block_size;
					result = newBlock->start;

				}
				

			}
			return result;
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
	printf("RETURNING SIZE: %d", result);
	return result;
}
Boolean rfree(void *block_ptr)
{
	Boolean result = false;
	table_b * curr = chooseptr->topBlocks;//starts at the topBlocks
	table_b * prev = NULL;

	while(curr != NULL && (block_ptr!= curr->start))//traverses the linked list till the end
	{
		prev = curr;
		curr = curr->next;
	}
	if((chooseptr->topBlocks)->start == block_ptr)//if the topBlocks is aleady the block_ptr no prev so change top and free curr
	{
			chooseptr->topBlocks = curr->next;
			chooseptr->remaining+=curr->size;
			free(curr);
			result = true;
	}
	else if((curr!=NULL && prev!=NULL)&&(block_ptr== curr->start) )//when we find the buffer we want to delete
	{

			prev = curr->next;//takes previous node from block list and links around current node
			//assert(prev = curr->next);//checks to see if linked properly.
			chooseptr->remaining+=curr->size;
			free(curr);//free's the malloced memory/Node
			result = true;//successfully  freed block of memory.
		
	}

	return result;

}
void rdestroy(const char *region_name)
{
	Boolean blockList = false;
	table_r *curr = topregion;
	table_r *prev = NULL;
	while(curr!=NULL && strcmp(region_name, curr->name)!=0)
	{
		prev = curr;
		curr = curr->next;
	}
	if(topregion->name == region_name)
	{
		blockList = destroyBlocks(topregion->topBlocks);
		if(blockList)
		{
			table_r *after = topregion->next;
			free(topregion->start);
			free(topregion);
			topregion = after;
			chooseptr = NULL;
		}


	}
	else if((curr !=NULL && prev!=NULL) && strcmp(region_name, curr->name)==0)
	{
		blockList = destroyBlocks(curr->topBlocks);
		if(blockList)
		{
			free(curr->start);
			prev = curr->next;
			chooseptr = NULL;
			free(curr);
		}
		
	}

}
void rdump()
{
	table_r * curr = topregion;
	assert(curr!=NULL);
	while(curr!=NULL)
	{
		assert(strlen(curr->name)!=0);
		printf("REGION NAME: %s\n", curr->name);
		table_b * currB = curr->topBlocks;
		while(currB!=NULL)
		{
			printf("BLOCK SIZE: %d, ALLOCATED: %p\n",currB->size, currB->start);
			currB= currB->next;
		}
		printf("FREE SPACE LEFT IN REGION: %d\n", chooseptr->remaining/chooseptr->size);
		curr = curr->next;
	}

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


Boolean destroyBlocks(table_b * top)
{
	Boolean result = false;
	table_b * curr = top;
	table_b * next = curr->next;
	while(curr !=NULL)
	{
		free(curr);
		curr = next;
		next = next->next;
	}
	if(curr == NULL)
	{
		result = true;
	}
	return result;
}

void zeroOut(r_size_t size,unsigned char * start)
{
	assert(size>0);
	assert(start!=NULL);
	unsigned char *zeroPointer = start;
	for(int i =0;i<size;i++)
	{
		zeroPointer = 0;
		zeroPointer+=1;
	}
}