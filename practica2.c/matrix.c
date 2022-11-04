/*

*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

int C[N][N];
int A[N][N];
int B[N][N];

int main(int argc, char** argv){
    int n,m,p,q;
    n=N;
    m=N;
    p=N;
    q=N;
    int i;
    int j;
    int k;
    int l;
    int w;
    int thread_num;
     /* parse command-line arg for number of threads */
    if (argc > 1) {
        thread_num = atoi(argv[1]);
    }

    #ifdef _OPENMP
        omp_set_num_threads( thread_num );
        printf("OMP defined, thread_num = %d\n", thread_num);
    #else
        printf("OMP not defined");
    #endif

    omp_set_num_threads(thread_num);
    for (i=0;i<N; i++){
        for(j=0;j<N; j++){
            A[i][j]=5;
            B[i][j]=2;
        }
    }
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for( i=0; i<m;++i){
        for( j=0; j<q;++j){
            C[i][j] = 0;
            for( k=0; k<p;++k){
                C[i][j]+= (A[i][k]) * (B[k][j]); 
            }
        } 
    }
    for(l=0; l<n; ++l) {
        for(w=0; w<q; ++w){
            printf("%d ",C[l][w]);
        }
        printf("\n");
    }
    return 0;
}
