//sse.c
//Papaxristos Giorgos AM P2010035
//-------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <emmintrin.h>

#include <inttypes.h>
//-------------------------------------------------------------------
#define N 512
#define M 512

#define g_image_width N
#define g_image_height M
//-------------------------------------------------------------------

void get_walltime(double *wct) {
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
int main() {
	
	__m128 const_p05 = _mm_set1_ps(0.5f);
    __m128 const_p50 = _mm_set1_ps(5.0f);    
    
float *imageOriginal,*imageFinal;

int i,j,x,y;
double ts,te,mflops;

  imageOriginal = (float *)_aligned_malloc(N * M * sizeof(float),16);
  imageFinal = (float *)_aligned_malloc(N * M * sizeof(float),16);
  
  float* screen = imageFinal + g_image_width*1;
   
  for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			*(imageOriginal + i*M + j) = (float) ( rand() % 255);
			*(imageFinal + i*M + j) = *(imageOriginal + i*M + j);			
		}
	}

   // get starting time (double, seconds) 
   get_walltime(&ts);    
//-------------------------------------------------------------------        
  
  	float* image_0 = imageOriginal + g_image_width * 0;
    float* image_1 = imageOriginal + g_image_width * 1;
    float* image_2 = imageOriginal + g_image_width * 2;
    
	__m128 current_0 = _mm_set1_ps(0.0f);
	__m128 current_1 = _mm_set1_ps(0.0f);
	__m128 current_2 = _mm_set1_ps(0.0f);

for(y=1; y<g_image_height-1; ++y)
    {
        for(x=1; x<g_image_width-1; x+=4)
        {		
		
		  current_0 = _mm_loadu_ps(image_0+x-1);
		  current_1 = _mm_loadu_ps(image_1+x-1);
		  current_2 = _mm_loadu_ps(image_2+x-1);
			
		__m128 image_00 = current_0;	
		__m128 image_01 =  _mm_set1_ps(current_0[1]);	
		__m128 image_02 = _mm_set1_ps(current_0[2]);	
		__m128 image_10 = current_1;	
		__m128 image_11 = _mm_set1_ps(current_1[1]);	
		__m128 image_12 = _mm_set1_ps(current_1[2]);	
		__m128 image_20 = current_2;	
		__m128 image_21 = _mm_set1_ps(current_2[1]);	
		__m128 image_22 = _mm_set1_ps(current_2[2]);
		
		 __m128 result = _mm_add_ps( _mm_mul_ps(image_00,const_p05),
		 				_mm_add_ps( _mm_mul_ps(image_01,const_p05),
                        _mm_add_ps( _mm_mul_ps(image_02,const_p05),
                        _mm_add_ps( _mm_mul_ps(image_10,const_p05),
                        _mm_add_ps( _mm_mul_ps(image_11,const_p50),
                        _mm_add_ps( _mm_mul_ps(image_12,const_p05),
                        _mm_add_ps( _mm_mul_ps(image_20,const_p05),
                        _mm_add_ps( _mm_mul_ps(image_21,const_p05),
                        _mm_mul_ps(image_22,const_p05)))))))));
		
            _mm_storeu_ps((imageFinal+x),result);
        }
        
        image_0 += g_image_width;
        image_1 +=  g_image_width;
        image_2 +=  g_image_width;
        imageFinal +=  g_image_width;
        
	}

 //-------------------------------------------------------------------
  // get ending time
  get_walltime(&te);
  
  printf("Execution Time = %f\n",(te-ts)*1e6);

	  
  // free arrays
   _aligned_free(imageOriginal);    
  
  return 0;
}
