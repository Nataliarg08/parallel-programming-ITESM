#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <time.h>

pthread_mutex_t lock;
#define m 6
#define n 6
int i;
int j;
int p;
int pi;
int pj;
float mattemp[n][m]={{-2,-2,-2,-2,-2,-2},
                     {-2,-2,-2,-2,-2,-2},
                     {-2,-2,-2,-2,-2,-2},
                     {-2,-2,-2,-2,-2,-2},
                     {-2,-2,-2,-2,-2,-2},
                     {-2,-2,-2,-2,-2,-2}};
                
char mat[n][m]={{'F','F','F','F','F','F'},
                {'F','F','F','F','F','F'},
                {'F','F','C','C','C','F'},
                {'F','C','C','C','F','F'},
                {'F','F','C','C','C','F'},
                {'F','F','F','F','F','F'}};


void *temp(void *a){
    pthread_mutex_lock(&lock);//Bloqueamos el acceso a otros threads
    for(i=0; i<n;i++){
        for (j=0;j<m;j++){
                if(mat[i][j]=='F'){
                    mattemp[i][j]=(rand( ) % 610 ) / 10.0f ;
                }
                else if(mat[i][j]=='C'){
                    mattemp[i][j]=((rand()%410)+600)/10.0f;
            }
        }
    } 
    pthread_mutex_unlock(&lock); //debloqueamos el acceso para otros threads
}
        
    


int main(int argc, char** argv){
    srand(time(NULL));

    for(i=0; i<n;i++){
        for (j=0;j<m;j++){
            printf("%c ",mat[i][j]);
        }
        printf("\n");
    }
  
    int thread_num;
     /* Para convertir el numero dado por el usuario en el número de thread para paralelizar el proceso */
    if (argc > 1) {
        thread_num = atoi(argv[1]);
    }
    pthread_t threads[thread_num]; //Arreglo de threads

    int i2;
    for(i2=0; i2<thread_num; i2++){
        pthread_create(&threads[i], NULL, temp, NULL); //Creamos cada thread con los valores necesarios
    }


    temp(mattemp);

    for(i=0; i<n;i++){
        for (j=0;j<m;j++){
            printf(" %1.1f ",mattemp[i][j]);
        }
        printf("\n");
    }
    int max_time = 80;
    int a = 2;
    int dx = 1;
    float dt = (pow(dx, 2))/(4 * a);
    float g = (a * dt) / (dx*dx);
  
    int u[max_time][n][m];
    omp_set_num_threads(thread_num);
    #pragma omp parallel for private(k, i, j) shared (u, TemperatureMatrix)
    for(int k = 0; k < max_time; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                u[k][i][j] = mattemp[i][j];
            }
        }
    }

    //#pragma omp parallel for private(k, i, j) shared (u)
    for(int k = 0; k < max_time - 1; k++){
        for(int i = 1; i < n - 1; i+= dx){
            for(int j = 1; j < m - 1; j+= dx){
                u[k + 1][i][j] = g * (u[k][i+1][j] + u[k][i-1][j] + u[k][i][j+1] + u[k][i][j-1] - 4*u[k][i][j]) + u[k][i][j];
            }
        }
    }
  
    printf("\n ------------------------------------------------------");
    printf("\nAdministrador:");
    printf("\nEsta es su ecuacion de calor:");
    printf("\ny = %d *[ %f / (%d^2)\n]", a, dt, dx);
    printf("------------------------------------------------------");

    return 0;

}
