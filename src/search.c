#define _GNU_SOURCE
#include <stdio.h>

#include "type.h"
#include "api.h"


/************************************************
function name: halfSearch
description:
 
	the key word of binarySearch:
1> the sequential storage structure must be adopted;
2> must be arranged in order;
************************************************/
u32 halfSearch(u8 *input, u32 dataLen, u32 targetVal)	// binary search
{
	u16 i;
	u32 low, high, targetPos;

	low = 0;
	high = dataLen - 1;

	while(low < high) {
		targetPos = low + (high - low)/2;  // (how + high)/2 maybe beyond to array scope
		if(input[targetPos] > targetVal) {
			high = targetPos;
		} else if(input[targetPos] < targetVal) {
			low = targetPos;
		} else {
			break;
		}

		if(1 == (high - low)) // the targetVal between high and low
			break;
	}
	return targetPos;
}




