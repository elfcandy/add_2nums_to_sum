#define _GNU_SOURCE
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "type.h"
#include "api.h"

#define MAX_PATH 100

enumSortType  g_eSortType = -1;
enumInputType g_eInputType = -1;
char g_cCommand[512] = {0};
bool_t g_bIsfileInput = FALSE;

static void prvPrintVersion(void)
{
	s8 *program_version = "1.0.0.1";
	printf("[MSG]: This program Version: [%s]\n", program_version);
}


static void prvPrintUsage(void)
{
	printf("This program input parameter info:\n");
	printf("=======================================================================\n");
	printf("--sortType/-s:  select sort method;       0/1/2 --> bubble/insert/heap\n");
	printf("--inputType/-i: select sort data source;  0/1   --> UI/file\n");
	printf("--conf/-1:      the conf file for using sort data source file;\n");
	printf("--version/-v:   program version;\n");
	printf("--help/-h:      program input parameters format info;\n");
	printf("                                                                       \n");
	printf("Input parameters has two methods:\n");
	printf("1> -sX -i0\n");
	printf("2> -sX -i1 --conf xxx.ini\n");
	printf("=======================================================================\n");
}


static u32 prvGetInputDataFromUI(void)
{
	u32 i;

	printf("Please enter the number of data to be calculated:");
	scanf("%u", &g_dataCnt);
	printf("[Remind]: Please confirm enter number is: %u\n", g_dataCnt);
	if ( (g_dataCnt < 2) || (g_dataCnt > BUFFER_SIZE) ) {
		printf("[Remind]: The g_dataCnt value is error\n");
		assert(0);
	}

	for (i = 0; i < g_dataCnt; i ++) {
		printf("Please enter data[%d]:", i);
		scanf("%hhu", &g_aData[i]);
	}
	printf("[Remind]: Please confirm enter data:");
	for (i = 0; i < g_dataCnt; i ++) {
		printf("%hhu ", g_aData[i]);
	}
	printf("\n");
		
	printf("[Remind]: Please enter the data of goal_sum:");
	scanf("%u", &g_goalSum);
}


static void prvParseConfigFile(s8 *configFile)
{
	FILE *t_fp = NULL;
	size_t t_len = 0;
	s8 *t_buf = NULL;
	s8 *t_str = NULL;
	s8 *t_savePtr = NULL;

	if ((t_fp = fopen(configFile, "r")) == NULL) {
		printf("[ERR]: [no file]: cannot open config file %s", configFile);
		exit(-1);
	}
	while (getline(&t_buf, &t_len, t_fp) != -1) {
		printf("The content of current line is %s", t_buf); // t_buf include "\n"
		t_str = strtok_r(t_buf, " =", &t_savePtr);
		if (strcmp(t_str, "InputDataNum") == 0) {
			t_str = strtok_r(NULL, " ;", &t_savePtr);
			g_dataCnt = atoi(t_str);
		} else if (strcmp(t_str, "InputGoalSum") == 0) {
			t_str = strtok_r(NULL, " ;", &t_savePtr);
			g_goalSum = atoi(t_str);
		}
	}
}


static void prvGenerateDataFromFile(void)
{
	int t_ret = 0;
	s8 t_pythonPath[MAX_PATH];
	const s8 t_pythonFile[] = {"script/genRandom.py"};

	getcwd(t_pythonPath, MAX_PATH);
	strcat(t_pythonPath, "/");
	strcat(t_pythonPath, t_pythonFile);
	sprintf(g_cCommand, "python %s -n%d", t_pythonPath, g_dataCnt);
	t_ret = system(g_cCommand);
	t_ret = WEXITSTATUS(t_ret);
	if (t_ret != 0) {
		printf("Generate random data file %s failed\n", t_pythonPath);
 		exit(-1);
	}
}


static void prvReadDataFromFile(void)
{
	u32 i = 0;
	FILE *t_fp = NULL;
	size_t t_len = 0;
	s8 *t_buf = NULL;
	s8 t_dataFilePath[MAX_PATH];
	const s8 t_dataFile[] = {"script/random.txt"};

	getcwd(t_dataFilePath, MAX_PATH);
	strcat(t_dataFilePath, "/");
	strcat(t_dataFilePath, t_dataFile);
	if ((t_fp = fopen(t_dataFilePath, "r")) == NULL) {
		printf("[ERR]: [no file]: cannot open random data file %s\n", t_dataFilePath);
		exit(-1);
	}

	while (getline(&t_buf, &t_len, t_fp) != -1) {
		g_aData[i] = atoi(t_buf);
		i++;
	}
}

#define NO_ARGUMENT        0
#define REQUIRED_ARGUMENT  1
#define OPTIONAL_ARGUMENT  2
void parseParameters(int argc, char **argv)
{
	int t_opt, t_optIndex;
	struct option long_options[] = {
        {"sortType",  REQUIRED_ARGUMENT, 0, 's'},
        {"inputType", REQUIRED_ARGUMENT, 0, 'i'},
        {"conf",      REQUIRED_ARGUMENT, 0,  1 },
        {"version",   NO_ARGUMENT,       0, 'v'},
        {"help",      NO_ARGUMENT,       0, 'h'},
        {0, 0, 0, 0}
    };

	s8 *t_configFile = NULL;
	/*
		int getopt_long_only (int ___argc, char *const *___argv,    
							  const char *__shortopts,    
							  const struct option *__longopts, int *__longind);
		1> argc, argv:  pass from the parse_parameters() argument;
		2> __shortopts: EXP: n:v; don't need "-"; "n:" mean "n" must be followed by parameters, EXP -n0;
		3> __longopts:  struct option array;
			struct option  
			{
				const char *name; // long opt name
				  int has_arg;    // 0/1/2: N/M/O
				  int *flag;      // if flag is NULL, specify a unique "Val" value for the long option, using "short opt" mapping "long opt"
				  int val;        // If flag isn't NULL, the "val" is stored in the flag's storage space, indicating the long opt has occurred.
			};
		4> __longind: return the index value of the long option in the __longopts structure array, for debug, usually is NULL;
		5> return -1 indicating parsing finish;
		6> parameters is stored "optarg", Exp: -i0, optarg = 0; 
	*/
    while( (t_opt = getopt_long_only(argc, argv, "s:i:vh", long_options, &t_optIndex))!= -1 ) {
        switch (t_opt) {
            case 's':
				 g_eSortType = atoi(optarg);
				 switch(g_eSortType) {
					case BUBBLE_SORT:
						 printf("[MSG]: Selected sort method is \"bubble_sort\"\n");
						 break;
					case INSERT_SORT:
						 printf("[MSG]: Selected sort method is \"insert_sort\"\n");
						 break;
					case HEAP_SORT:
						 printf("[MSG]: Selected sort method is \"heap_sort\"\n");
						 break;
				 }
				 break;
            case 'i':
				 g_eInputType = atoi(optarg);
				 if ( g_eInputType == FROM_CMD_LINE ) {
					printf("[MSG]: Selected input data type is \"from UI\"\n");
					prvGetInputDataFromUI();
				 } else if ( g_eInputType == FROM_FILE ) {
					printf("[MSG]: Selected input data type is \"from File\"\n");
					g_bIsfileInput = TRUE;
				 }
				 break;
			case 1:
				 if(g_bIsfileInput) {
				 	t_configFile = strdup(optarg);
				 	prvParseConfigFile(t_configFile);
					prvGenerateDataFromFile();
					prvReadDataFromFile();
				 }
				 break;
            case 'v':
				 prvPrintVersion();
				 exit(0);
				 break;
            case 'h':
				 prvPrintUsage();
				 exit(0);
				 break;
			default:
				 printf("[ERR]: Input parameters is invalid\n");
				 exit(0);
				 break;	
		}
	}
	if ((g_eSortType == -1) && (g_eInputType == -1)) {
		printf("[ERR]: Input parameters is null\n");
		exit(0);
	}
}

