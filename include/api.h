#ifndef API
#define API
#include <assert.h>
#include <sys/time.h>

#define BUFFER_SIZE     1024
#define RESULT_BUF_SIZE BUFFER_SIZE/2

extern u32 g_dataCnt;
extern u32 g_goalSum;
extern u8  g_aData[BUFFER_SIZE + 1];  //add g_goalSum/2

typedef enum {
	BUBBLE_SORT = 0,
	INSERT_SORT,
	HEAP_SORT,
}enumSortType;
extern enumSortType g_eSortType;

typedef enum {
	FROM_CMD_LINE = 0,
	FROM_FILE,
}enumInputType;
extern enumInputType g_eInputType;

void parse_parameters(int argc, char **argv);
u32 halfSearch(u8 *input, u32 dataLen, u32 targetVal);
u32 sortData(u8 *input, u32 dataLen, enumSortType sortMode);


#define TIME_DEFINE_VAR struct timeval start_tv, stop_tv;
static inline void INTERNAL_TIME_START(struct timeval *tv)
{
    gettimeofday(tv, NULL);
}

static inline void INTERNAL_TIME_STOP(struct timeval *tv)
{
    gettimeofday(tv, NULL);
}

static inline void INTERNAL_TIME_REPORT(char *event, struct timeval start_tv, struct timeval stop_tv)
{
    printf("Event: %s time spend %dus\n", event, \
           (int)(((stop_tv.tv_sec - start_tv.tv_sec) * 1000000) + (stop_tv.tv_usec - start_tv.tv_usec)));
}

#endif
