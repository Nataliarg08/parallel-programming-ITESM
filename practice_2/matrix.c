/*
Programa que realiza la multiplicación de matrices de forma paralela por open MP
*/
//Librerías que se usarán
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>

#define N 100

//Matrices para multiplicar (A y B) y la resultante (C)
int C[N][N];
int A[N][N];
int B[N][N];

int main(int argc, char** argv){
    //Declaracion de variables para los for
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
     /* Para convertir el numero dado por el usuario en el número de thread para paralelizar el proceso */
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

    //Se llenan las matrices con datos fijos
    for (i=0;i<N; i++){
        for(j=0;j<N; j++){
            A[i][j]=5;
            B[i][j]=2;
        }
    }

    //A partir de aquí se paraleliza el proceso.
    #pragma omp parallel for private(i,j,k) shared(A,B,C)
    for( i=0; i<m;++i){
        for( j=0; j<q;++j){
            C[i][j] = 0;
            for( k=0; k<p;++k){
                C[i][j]+= (A[i][k]) * (B[k][j]); 
            }
        } 
    }

    //for para imprimir la matriz resultante
    for(l=0; l<n; ++l) {
        for(w=0; w<q; ++w){
            printf("%d ",C[l][w]);
        }
        printf("\n");
    }
    
    return 0;
}
