//no-sse.c
//Papaxristos Giorgos AM P2010035
//-------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
//-------------------------------------------------------------------
#define N 512
#define M 512
//-------------------------------------------------------------------
void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int main() {
float K[3][3] = { {0.5,0.5,0.5}, {0.5,5.0,0.5}, {0.5,0.5,0.5}};
int i,j;
double ts,te,mflops;

float **imageOriginal = malloc(sizeof *imageOriginal * N);
float **imageFinal = malloc(sizeof *imageFinal * N);

if (imageOriginal)
{
  for (i = 0; i < N; i++)
  {
    imageOriginal[i] = malloc(sizeof *imageOriginal[i] * M);
  }
}
if (imageFinal)
{
  for (i = 0; i < N; i++)
  {
    imageFinal[i] = malloc(sizeof *imageFinal[i] * M);
  }
}
 
//init the Arrays 
for (i = 0; i < N; i++)
{
	for (j = 0; j < M; j++)
	{
		imageOriginal[i][j] = (float) ( rand() % 255);
		imageFinal[i][j] =  imageOriginal[i][j];
	}
}

  // get starting time (double, seconds) 
  get_walltime(&ts);
  
//-------------------------------------------------------------------

for (i = 1; i < N-1; i++)
{
	for (j = 1; j < M-1; j++)
	{
		imageFinal[i][j] = K[0][0]*imageOriginal[i-1][j-1] + 
		K[0][1]*imageOriginal[i][j-1] +
		K[0][2]*imageOriginal[i+1][j-1] +
		K[1][0]*imageOriginal[i-1][j] +
		K[1][1]*imageOriginal[i][j] +
		K[1][2]*imageOriginal[i+1][j] +
		K[2][0]*imageOriginal[i-1][j+1] +
		K[2][1]*imageOriginal[i][j+1] +
		K[2][2]*imageOriginal[i+1][j+1] ;
	}
}

//-------------------------------------------------------------------


  // get ending time
  get_walltime(&te);
  
  printf("Execution Time = %f\n",(te-ts)*1e6);
  
  // free arrays
  for(i = 0; i < N; i++)
  {
  	free(imageOriginal[i]);
  	free(imageFinal[i]);
  }

  free(imageOriginal); free(imageFinal);
  
  return 0;
}
