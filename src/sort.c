#define _GNU_SOURCE
#include <stdio.h>

#include "type.h"
#include "api.h"


static void prvSwapData(u8 *input, u32 a, u32 b)
{
	u8 t_val;

	t_val = input[a];
	input[a] = input[b];
	input[b] = t_val;
}

/**************************************************************************
Sort Method: bubbleSort
description: 
	the key word of "bubble sort":
1> loop cnt is dataLen
2> compare cnt is dataLen-1

Algorithm complexity: O(n*n)
**************************************************************************/
static u32 prvBubbleSort(u8 *input, u32 dataLen)
{
	u32 i, j;
	bool_t b_isCompleted = FALSE;

	for (i = 0; i < dataLen; i++) { //sorting count
		if (b_isCompleted == FALSE)
			b_isCompleted = TRUE;
		else
			break;

		for (j = 0; j < (dataLen - 1); j++) { //compare two numbers, should sub 1
			if(input[j] > input[j + 1]) {
				prvSwapData(input, j, j+1);
				b_isCompleted = FALSE;
			} else if(input[j] == input[j + 1]) {
				dataLen -= 1;
				input[j + 1] = input[dataLen];
			}
		}
	}
	return dataLen;
}


/**************************************************************************
Sort Method: insertSort
description: 
	the basic principle of "insert sort":
sorting array[n], for a data[i]:
1> data[0] to data[i-1] has been ordered;
2> data[i+1] to data[n-1] is disorder series, and waiting to be order;
3> If from small to large, data[i-1](the end of ordered series) is smaller than data[i], directly break;

Algorithm complexity: O(n*n)
**************************************************************************/
static u32 prvInsertSort(u8 *input, u32 dataLen)
{
	u32 i, cur_pos;

	for (cur_pos = 1; cur_pos < dataLen; cur_pos ++) {
		i = cur_pos;
		while (i > 0) { //insert data from the second data	
			if (input[i] > input[i-1]) { //because 0..cur_pos is ordered series, can directly break
				break;
			} else if(input[i] < input[i-1]) {
				prvSwapData(input, i, i-1);
			} else {
				while (i < cur_pos) {    //move the ordered series foward
					input[i] = input[i+1];
					i++;
				}
				dataLen -= 1;
				input[cur_pos] = input[dataLen];  //move the last data to the end of ordered series
				break;
			}
			i--;
		}
	}
	return dataLen;
}

/**************************************************************************
Sort Method: heapSort
description:
A>	the concept of big heap:
		The value of each node is greater than or equal to the value of its child nodes;
		EXP:
		   _______     _______________
		  |   |   |   |               |
		  |  \|/ \|/  |              \|/
		--|---|---|---|---------------|--
		| 3 | 2 | 4 | 7 | 1 | 5 | 9 | 6 |
		------|---|---|---|---|---|------
			  |   |  /|\ /|\ /|\ /|\
			  |___|___|___|   |   |
				  |___________|___|

B>	the key word of "heap sort":
	1> structure a big top heap or small top heap;
	    i> the big-top heap:   data[i] >= data[2i+1] && data[i] >= data[2i+2]
	   ii> the small-top heap: data[i] <= data[2i+1] && data[i] <= data[2i+2]
	2> move the data of heap top to array tail, data[0]->data[i-1], dataLen becomes i-1;
	3> for array[0..i-2], loop above 1 and 2 step to sort; 

Algorithm complexity: O(n*logn)
**************************************************************************/


/*************************************************************************
Function Name: prvAdjustCurNode()
description: 
	adjust current node input[i] to the mininum value node(maybe is leaf or non-leaf);
*************************************************************************/
static void prvAdjustCurNode(u8 *input, u32 offset, u32 dataLen)
{
	s32 k;
	u8  t_data = input[offset];  // t_data is non-leaf node

	for (k = (2*offset + 1); k < dataLen; k = (2*k + 1)) {			// the left leaf node of input[i] begin
		if ( ((k + 1) < dataLen) && (input[k] < input[k + 1]) ) {	// compare the value of left leaf node and right leaf node
			k++;
		}
		if (input[k] > t_data) { // adjust the leaf node to its root node
			input[offset] = input[k];
			offset = k;
		} else {
			break;				 // from the last non-leaf adjust, so if leaf node less than non-leaf node, directly exit 
		}
	}
	input[offset] = t_data;
}

static u32 prvHeapSort(u8 *input, u32 dataLen)
{
	s32 i, t_len;

	// generate big top heap
	for (i = dataLen/2-1; i>=0; i--) { // (dataLen/2-1) for get the last non-leaf node, the node before this node are non-leaf
		prvAdjustCurNode(input, i, dataLen);
 	}

	// adjust big top heap and exchange top node and tail node
    for (t_len = dataLen - 1; t_len>0; t_len--) {
		prvSwapData(input, 0, t_len);
        prvAdjustCurNode(input, 0, t_len);
    }

	return dataLen;
}


typedef u32 (*tdSortFunction)(u8 *input, u32 dataLen);
static tdSortFunction tdSortFunc[] = { prvBubbleSort,
									 prvInsertSort,
									 prvHeapSort,
									 NULL
								   };

u32 sortData(u8 *input, u32 dataLen, enumSortType eSortMode)
{
	u32 i;

	dataLen = tdSortFunc[eSortMode](input, dataLen);
	printf("Sorted input data result is:");
	for (i = 0; i < dataLen; i++, input++) {
		printf("%hhu ", *input);
	}
	printf("\n");

	return dataLen;	
}






//FILE *fp = NULL;
//fprintf(fp, "Sorted input data result is:");
//for (i = 0; i < dataLen; i ++) {
//	fprintf(fp, "%hhu ", *input);
//	input ++;
//}
//fprintf(fp, "\n");
