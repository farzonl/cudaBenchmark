#_____________GCC STUFF__________________________________________
CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -Werror -fopenmp -lpthread 
UNAME := $(shell uname)
EXE     := $(UNAME)matrixmul
#_____________GCC________________________________________________
#_____________CUDA STUFF_________________________________________
NVCC       =nvcc
CUDA	   =  /usr/local/cuda
SDK	   = /home/farzon/NVIDIA_GPU_Computing_SDK/C

INC	   = -I$(CUDA)/include -I$(SDK)/common/inc -I.
LIB	   = -L$(CUDA)/lib64   -L$(SDK)/lib

NVCCFLAGS  = -g -G -arch=sm_20 --ptxas-options=-v --use_fast_math
LIBS	   = -lcutil_x86_64D -lcudart

BINDIR     = CUDA_BIN

#EXECUTABLE	:= $(EXE)CUDA
# Cuda source files (compiled with cudacc)
#CUFILES		:= matrixmul.cu
# C/C++ source files (compiled with gcc / c++)
# CCFILES		:= \
#	matrixTest.c

#_____________CUDA STUFF_________________________________________


run : build-release
	./$(EXE)

run-cuda: build-cuda
	./$(EXE) && ./$(EXE)CUDA

run-debug : build-debug
	./$(EXE)

run-gdb : build-debug
	gdb ./$(EXE)

edit0 : 
	nano -c matrixMul.c

edit1 : 
ifeq ($(UNAME), Linux)
	kate matrixMul.c matrixMul.h matrixTest.c &
endif
ifeq ($(UNAME), Darwin)
	@echo 'N/A'
endif

run-valgrind : build-debug
	valgrind --leak-check=yes --show-reachable=yes --tool=memcheck ./EXE

build-release : CFLAGS += -O2 -O3

build-release : matrixMul.o matrixTest.c matrixMul.h
	$(CC) $(CFLAGS) matrixTest.c matrixMul.o -o $(EXE)

build-debug : CFLAGS += -g -DDEBUG
build-debug : matrixMul.o matrixTest.c matrixMul.h
	$(CC) $(CFLAGS) matrixTest.c matrixMul.o -o $(EXE)


matrixMul.o : matrixMul.c matrixMul.h
	$(CC) $(CFLAGS) -c matrixMul.c

build-cuda : matrixmul.cu matrixmul_kernel.cu  matrixMul.h
	mkdir -p $(BINDIR)
	$(NVCC) matrixmul.cu matrixmul_kernel.cu  -c $(INC) $(LIB) $(NVCCFLAGS) $(LIBS)
	$(CC)   matrixmul*.o -o $(EXE)CUDA 


#CUDA-Set :
#ifeq ($(UNAME), Linux)
#include /home/farzon/NVIDIA_GPU_Computing_SDK/C/common/common.mk
#NVCCFLAGS += --compiler-bindir=/usr/bin/gcc-4.4
#endif

#ifeq ($(UNAME), Darwin)
#include /Developer/GPU\ Computing/C/common/common.mk
#endif

clean :
	rm *.o* $(EXE)* || rm -r $(BINDIR)

