#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main(){
  struct timeval start;
  struct timeval end;
  gettimeofday(&start,NULL);
  int** A = (int**)malloc(1024*sizeof(int*));
  int** B = (int**)malloc(1024*sizeof(int*));
  int** C = (int**)malloc(1024*sizeof(int*));
  int i=0,j=0,k=0;
  for(i=0;i<1024;i++){
    A[i] = (int*)malloc(1024*sizeof(int));
    B[i] = (int*)malloc(1024*sizeof(int));
    C[i] = (int*)malloc(1024*sizeof(int));
  }
  for(i=0;i<1024;i++){
    for(j=0;j<1024;j++){
      A[i][j] = 1;
      B[i][j] = 2;
      C[i][j] = 0;
    }
  }
  for(i=0;i<1024;i++){
    for(j=0;j<1024;j++){
      for(k=0;k<1024;k++){
        C[i][j] += A[i][k]*B[k][j];
      }
    }
  }
  gettimeofday(&end,NULL);
  long time = (end.tv_sec-start.tv_sec)*1000;
  time += (end.tv_usec-start.tv_usec)/1000;
  printf("Time taken by the program in milliseconds :%ld\n",time);
  return 0;
}