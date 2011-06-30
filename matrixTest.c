/*
 *  MatrixTest.c
 *  
 *
 *  Created by Farzon Lotfi on 7/18/11.
 *  Copyright 2011 Georgia Tech. All rights reserved.
 *
 */
#include "matrixMul.h"

int main()
{
	Matrix a; // factor 1
	Matrix b; // factor 2
	Matrix p; // product
	Matrix Ppthread, PopenMP;
	int i, j;
	for (i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			//printf("a[%i][%i]",i,j);
			a[i][j] = (rand()%9)+1;
			b[i][j] = rand()%9;
		}
	}
	clock_t start, end;
        struct timeval t0, t1;
	double runtime, elapsed, runtimePthread, elapsedPthread, runtimeOMP, elapsedOpenMP;
	
	start = clock();
        gettimeofday(&t0,NULL);
	/*unsigned int timer = 0;
	CUT_SAFE_CALL( cutCreateTimer( &timer));
	CUT_SAFE_CALL( cutStartTimer( timer));*/
	
	matrixMult(&a,&b,&p); //SEQ
	
	/*CUT_SAFE_CALL( cutStopTimer( timer));
	float seq_time =  cutGetTimerValue( timer);*/
	end = clock();
        gettimeofday(&t1,NULL);
	elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
	runtime = (double)(t1.tv_sec-t0.tv_sec) + (double)(t1.tv_usec-t0.tv_usec)/1e6;
	
	start = clock();
	gettimeofday(&t0,NULL);
	/*timer = 0;
	CUT_SAFE_CALL( cutCreateTimer( &timer));
	CUT_SAFE_CALL( cutStartTimer( timer));*/
	
	pthreadMultiply2(&a,&b,&Ppthread); //PTHREAD
	
	/*CUT_SAFE_CALL( cutStopTimer( timer));
	float pthread_time =  cutGetTimerValue( timer);*/
	
	end = clock();
	gettimeofday(&t1,NULL);
	elapsedPthread  = ((double) (end - start)) / CLOCKS_PER_SEC;
	runtimePthread  = (double)(t1.tv_sec-t0.tv_sec) + (double)(t1.tv_usec-t0.tv_usec)/1e6;
	

	start = clock();
	gettimeofday(&t0,NULL);
	/*timer = 0;
	CUT_SAFE_CALL( cutCreateTimer( &timer));
	CUT_SAFE_CALL( cutStartTimer( timer));*/
	
	openMPMultiply(&a,&b,&PopenMP);//OPM
	
	/*CUT_SAFE_CALL( cutStopTimer( timer));
	float omp_time =  cutGetTimerValue( timer);*/
	end = clock();
	gettimeofday(&t1,NULL);
	elapsedOpenMP = ((double) (end - start)) / CLOCKS_PER_SEC;
	runtimeOMP    = (double)(t1.tv_sec-t0.tv_sec) + (double)(t1.tv_usec-t0.tv_usec)/1e6;
	/*printArray(&a);
	printArray(&b);
	printArray(&p);
	printArray(&Ppthread);*/
	printf("\nSequential matrix multiply clock: %f\nPthread    matrix multiply clock: %f\nOpenMP     matrix multiply clock: %f\n",elapsed,elapsedPthread,elapsedOpenMP);
	printf("Sequential runtime = %f s\nPthread    runtime = %f s\nOpenMP     runtime = %f s\n",runtime,runtimePthread,runtimeOMP);
	
	//printf("\nSequential matrix multiply processing time: %f (ms)\nPthread    matrix multiply processing time: %f (ms)\nOpenMP     matrix multiply processing time: %f (ms)\n",seq_time,pthread_time,omp_time);
	
	printf("Pthread Speedup over SEQ: %f\n", (runtimePthread >0)  ? runtime/runtimePthread : 0);
	printf("OpenMP  Speedup over SEQ: %f\n", (runtimeOMP>0)  ? runtime/runtimeOMP : 0);

	
	int bool = compare(&p,&Ppthread);
	printf("Pthread Test %s\n", (1 == bool) ? "PASSED" : "FAILED");
	bool = compare(&p,&PopenMP);
	printf("OpenMP  Test %s\n", (1 == bool) ? "PASSED" : "FAILED");
	return 0;
}