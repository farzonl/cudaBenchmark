#ifndef _MATRIXMUL_H_
#define _MATRIXMUL_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>
#define NUM_THREADS 2
#define WIDTH   1000
#define HEIGHT  WIDTH
#define KHEIGHT WIDTH // the value that row of Matrix 1 and column of Matrix 2 have to equal
typedef float Matrix[HEIGHT][WIDTH];

typedef struct
{
	int i; /* row */
	int j; /* column */
	int id;
	int process;
	int dim; /*num of threads*/
	Matrix* p;
	Matrix* a;
	Matrix* b;
}pos;

void matrixMult(Matrix* a, Matrix* b, Matrix* p);
void printArray(Matrix* twoDArr);
int compare(Matrix* a, Matrix* b);

//pthread helper function
void pthreadMultiply2(Matrix* a, Matrix* b, Matrix* p);
void *multiply(void* param);

//OpenMP
void openMPMultiply(Matrix* a, Matrix* b, Matrix* p);


#endif // _MATRIXMUL_H_

