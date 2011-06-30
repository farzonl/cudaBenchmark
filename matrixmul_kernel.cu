#ifndef _MATRIXMUL_KERNEL_H_
#define _MATRIXMUL_KERNEL_H_

#include <stdio.h>
#include "matrixMul.h"

////////////////////////////////////////////////////////////////////////////////
//! Simple test kernel for Matrix Multiplication functionality
//! @param M  input data
//! @param N  input data
//! @param P  output data
////////////////////////////////////////////////////////////////////////////////
// Matrix multiplication kernel thread specification
__global__ void MatrixMulKernel(Matrix* M, Matrix* N, Matrix* P)
{
	// thread index
	int ty = threadIdx.y;
	int tx = threadIdx.x;
	
	int *Mptr = (int*)((void*)M);
	int *Nptr = (int*)((void*)N);
	int *Pptr = (int*)((void*)P);
	
	// block index
	int bx = blockIdx.x;
	int by = blockIdx.y;
	
	int row = blockDim.y * by + ty;
	int col = blockDim.x * bx + tx;
	
	float Pvalue = 0;
	
	for (int k = 0; k < WIDTH; k++)
        {
            Pvalue += Mptr[row*WIDTH+k] * Nptr[k*WIDTH+col];
        }
	Pptr[row*WIDTH+col] = Pvalue;
}

#endif // #ifndef _MATRIXMUL_KERNEL_H_
