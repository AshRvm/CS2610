#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main(){
    struct timeval start_time;
    struct timeval end_time;
    gettimeofday(&start_time, NULL);
    printf("seconds : %ld\nmicro seconds : %ld\n",start_time.tv_sec, start_time.tv_usec);
    int** A = (int**)malloc(1024*sizeof(int*));
    int** B = (int**)malloc(1024*sizeof(int*));
    int i=0;
    int j=0;
    int k=0;
    for(i=0;i<1024;i++){
        A[i] = (int*)malloc(1024*sizeof(int));
        B[i] = (int*)malloc(1024*sizeof(int));
    }
    for(i=0;i<1024;i++){
        for(j=0;j<1024;j++){
            A[i][j] = i+j;
            B[i][j] = i-j;
        }
    }
    int temp_sum;
    int** C = (int**)malloc(1024*sizeof(int*));
    for(i=0;i<1024;i++){
        C[i] = (int*)malloc(1024*sizeof(int));
    }
    for(i=0;i<1024;i++){
        for(j=0;j<1024;j++){
            for(k=0;k<1024;k++){
                temp_sum += A[i][k] * B[k][j];
            }
            C[i][j] = temp_sum;
            temp_sum = 0;
        }
    }
    gettimeofday(&end_time, NULL);
    printf("seconds : %ld\nmicro seconds : %ld\n",end_time.tv_sec, end_time.tv_usec);
    printf("time elapsed in milli seconds :%ld\n",(end_time.tv_usec - start_time.tv_usec)/1000);
    return 0;
}