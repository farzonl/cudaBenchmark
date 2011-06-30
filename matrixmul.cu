
// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include <cuda_runtime_api.h>
#include <cuda.h>


// includes, project
#include <cutil.h>

// includes, kernels
#include <matrixmul_kernel.cu>

////////////////////////////////////////////////////////////////////////////////
//! Run a simple test for CUDA
////////////////////////////////////////////////////////////////////////////////
void MatrixMulOnDevice(Matrix* M, Matrix* N, Matrix* P)
{
	//Interface host call to the device kernel code and invoke the kernel
	 Matrix *Md, *Nd, *Pd;
	 
	//Step 1 allocate memory using cuda malloc
	cudaMalloc( (void**) &Md, sizeof(Matrix) );
	cudaMalloc( (void**) &Nd, sizeof(Matrix) );
	cudaMalloc( (void**) &Pd, sizeof(Matrix) );
	
	//Step 2 memcpy to GPU from host using cudamemcpy
	 cudaMemcpy(Md, M, sizeof(Matrix), cudaMemcpyHostToDevice);
	 cudaMemcpy(Nd, N, sizeof(Matrix), cudaMemcpyHostToDevice);
	 cudaMemcpy(Pd, P, sizeof(Matrix), cudaMemcpyHostToDevice);

	
	// step 3: setup the threads should be based on some constant	
	
	int width, height;
	
	//this seems wrong
	width = height = KHEIGHT;
	
	//http://www.cs.nyu.edu/manycores/cuda_many_cores.pdf see page 17
	//Dimensions of the block in threads
	dim3 bDim(width,height);
	
	//Dimensions of the grid in blocks
	dim3 gDim(width/bDim.x,height/bDim.y);
	
	//Step 4 call the kernel
	 MatrixMulKernel<<<gDim,bDim >>>(Md,Nd,Pd);
	
	//Step 5 memcopy computed value back from device to host
	 cudaMemcpy(P, Pd, sizeof(Matrix), cudaMemcpyDeviceToHost);
	 
	 //Step 6 free the malloced memory of step 1
	cudaFree(Md);
	cudaFree(Nd);
	cudaFree(Pd);
	
	
	
}

int main()
{
  return 0;
}