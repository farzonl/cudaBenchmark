#include "matrixMul.h"

void matrixMult(Matrix* a, Matrix* b, Matrix* p)
{
	//memset(*p, 0, sizeof(Matrix));
	int sum, blockA, blockB;
	for(int i = 0; i < HEIGHT; i++) 
		for(int j = 0; j < WIDTH; j++)
		{
			sum = (*p)[i][j];
			for(int k = 0; k < KHEIGHT ; k++)
			{
				//(*p)[i][j] += (*a)[i][k] * (*b)[k][j];
				blockA = (*a)[i][k];
				blockB = (*b)[k][j]; 
				sum = sum +  blockA*blockB;
			}
			(*p)[i][j] = sum;
		}
}

void printArray(Matrix* twoDArr)
{
  printf("[");
  int i, j;
  for (i = 0; i < HEIGHT; i++)
  {
     printf("{");
    for(j = 0; j < WIDTH; j++)
    {
      printf("%f,",(*twoDArr)[i][j]);      
    }
     printf("}\n");
  }
  printf("]\n");
}

void openMPMultiply(Matrix* a, Matrix* b, Matrix* p)
{
  //int i,j,k;
  memset(*p, 0, sizeof(Matrix));
  int	tid, nthreads, i, j, k, chunk;
  /*chunk    = 100;
  nthreads =  4;*/
 //#pragma omp parallel num_threads(nthreads) shared(a,b,p,chunk) private(i,j,k)
 #pragma omp parallel shared(a,b,p,nthreads,chunk) private(tid,i,j,k)
  {
	tid = omp_get_thread_num();
	if (tid == 0)
	{
	  nthreads = omp_get_num_threads();
	}
	chunk = 10;
	//   #pragma omp parallel for private(i, j, k)
	//#pragma omp for schedule (dynamic, chunk) nowait
        #pragma omp for schedule (static, chunk)
	for(i = 0; i < HEIGHT; i++)
	{
	 // printf("Thread=%d did row=%d\n",tid,i);
		for(j = 0; j < WIDTH; j++)
		{
			//#pragma omp parallel for
			for(k = 0; k < KHEIGHT ; k++) 
				(*p)[i][j] += (*a)[i][k] * (*b)[k][j];
		}
	}
  }
}

// thread function: taking "slice" as its argument
void *multiply(void* param)
{
	/*pos *data = param; // the structure that holds our data
	int s = data->slice;   // retrive the slice info
	int from = (s * WIDTH)/NUM_THREADS; // note that this 'slicing' works fine
	int to = ((s+1) * WIDTH)/NUM_THREADS; 
	
	for(int i = from; i < to; i++) 
		for(int j = 0; j < WIDTH; j++)
			for(int k = 0; k < KHEIGHT ; k++)
				(*(data->p))[i][j] += (*(data->a))[i][k] * (*(data->b))[k][j];*/
	
	pos *data = (pos *) param;
	int             i,j,k,n;
	i = data->id;
	n = data->dim;
	while (i<n) 
	{

		for (j = 0; j < n; j++) 
		{
			for (k = 0; k < n; k++) 
			{
				(*(data->p))[i][j] += (*(data->a))[i][k] * (*(data->b))[k][j];
			}
			
		}
		i+=data->process;
	}
	return 0;
}



void pthreadMultiply2(Matrix* a, Matrix* b, Matrix* p)
{
	memset(p, 0, sizeof(Matrix));
	pthread_t* thread;  // pointer to a group of threads
	thread = (pthread_t*) malloc(NUM_THREADS*sizeof(pthread_t));
	int i;
	pos *data = (pos *) malloc(sizeof(pos)*NUM_THREADS);
	for (i = 1; i < NUM_THREADS; i++)
	{
		
		data[i].p = p;
		data[i].a = a;
		data[i].b = b;
		data[i].id = i;
		data[i].dim = HEIGHT;
		data[i].process = NUM_THREADS;
		// creates each thread working on its own slice of i
		/*if(pthread_create (&thread[i], NULL, multiply,data) != 0 )
		{
			perror("Can't create thread");
			free(thread);
		}*/
		pthread_create (&thread[i], NULL, multiply,(void *)(data+i));
	}
	/*
	pos *data = (pos *) malloc(sizeof(pos));
	data->slice = 0;
	data->p = p;
	data->a = a;
	data->b = b;
	multiply(data);*/
	
	for (i = 1; i < NUM_THREADS; i++)
		pthread_join (thread[i], NULL);
	
	free(thread);
}

int compare(Matrix* a, Matrix* b)
{
	int bool, i, j;
	
	for (i = 0; i < HEIGHT; i++)
	{
		for(j = 0; j < WIDTH; j++)
		{
			if ((*a)[i][j] == (*b)[i][j]) 
				bool = 1;
			
			else {
				bool = 0;
				break;
			}
		}
	}
	return bool;
}
