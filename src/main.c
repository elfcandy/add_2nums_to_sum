#define _GNU_SOURCE
#include <stdio.h>

#include "type.h"
#include "api.h"

u32 g_dataCnt;
u32 g_goalSum;

u8  g_aData[BUFFER_SIZE + 1];  //add g_goalSum/2
u8  g_aResultData[2][RESULT_BUF_SIZE];

static void prvGetTwoNum(u32 index, u32 halfVal)
{
	bool_t t_bExistCombination = 0;

	if ( (g_goalSum < g_aData[0]) || (halfVal > g_aData[g_dataCnt-1]) || (index >= g_dataCnt) ) {
		printf("Can't find required g_aData\n");
		assert(0);
//	} else if(2 == g_g_aDataCnt) {
//		if( g_goalSum == (g_aData[0] + g_aData[1]) ) {
//			printf("The combination of g_aData that conforms to the Goal sum(%u) is: %u and %u\n", g_goalSum, g_aData[0], g_aData[1]);
//		} else {
//			printf("Can't find required g_aData\n");
//		}
	} else {
		u16 i, j, k;
		k = 0;
		for(i = 0; i < index; i++) {
			for(j = index; j < g_dataCnt; j++) {
				if( g_goalSum == (g_aData[i] + g_aData[j]) ) {
					g_aResultData[k][0] = g_aData[i]; 
					g_aResultData[k][1] = g_aData[j];
					printf("The combination of g_aData that conforms to the Goal sum(%u) is: %u and %u\n", g_goalSum, g_aData[i], g_aData[j]);
					k++;
					t_bExistCombination = 1;
				} else if( g_goalSum < (g_aData[i] + g_aData[j]) ) {
					break;
				}
			}
		}
	}

	if (0 == t_bExistCombination) {
		printf("Can't find required g_aData\n");
	}
}

int main(int argc, char **argv)
{
	u32 t_idx;
	u32 t_halfVal;

	//printf("short is %d; int is %d; long is %d\n", sizeof(short), sizeof(int), sizeof(long));
	parseParameters(argc, argv);

	TIME_DEFINE_VAR
    INTERNAL_TIME_START(&start_tv);

	g_dataCnt = sortData(g_aData, g_dataCnt, g_eSortType);

    INTERNAL_TIME_STOP(&stop_tv);
    INTERNAL_TIME_REPORT("sort g_aData", start_tv, stop_tv);


	t_halfVal = g_goalSum/2;
	t_idx = halfSearch(g_aData, g_dataCnt, t_halfVal);

	prvGetTwoNum(t_idx, t_halfVal);
}

