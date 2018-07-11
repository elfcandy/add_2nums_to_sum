CC = gcc
LD = ld
CC_FLAGS = -c -g -O0
LD_FLAGS = -T
#-lc
LD_FILE = s.lds

#successfully ops:
#1> gcc main.c -Wl,-Ts.lds
#2> gcc -c -g -O0 main.c -Wl,-Ts.lds
#3> gcc -c -g -O0 main.c
#   gcc -o ./a.out -Wl,-Ts.lds ./main.o
#            (-Wl is Optional)


    # -Wall -Werror 
    # -fPIC

all:
#	${CC} -g -O0 *.c

	${CC} ${CC_FLAGS} -I./include src/main.c   -o ./src/main.o
	${CC} ${CC_FLAGS} -I./include src/sort.c   -o ./src/sort.o
	${CC} ${CC_FLAGS} -I./include src/search.c -o ./src/search.o
	${CC} ${CC_FLAGS} -I./include src/parameter.c -o ./src/parameter.o
	${CC} -o a.out -pthread ./src/*.o -Wl,-Map=a.map 

#below is error
#	${LD} ${LD_FLAGS}s.lds ./main.o -o ./a.out

clean:
	rm -rf ./src/*.o ./*.out ./*.map ${LD_FILE}

