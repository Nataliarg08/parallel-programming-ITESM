/*

Referencias de códigos:
Mutex: 

Función para encontrar puntos en el circulo:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/98fae4a0a03db0c80b08706f851b831474ac3a35/labs/04/findCircle.c
Mutex:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/98fae4a0a03db0c80b08706f851b831474ac3a35/labs/04/mutex-thread.c 
Threads:
https://github.com/VictorRodriguez/parallel-programming-lecture/blob/98fae4a0a03db0c80b08706f851b831474ac3a35/labs/04/simple-thread.c 
*/

//Bibliotecas a usar
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


//Declaracion de variables globales
#define THREADS   4 //numero de threads que queremos usar
pthread_mutex_t lock; //Candado de mutex declarado

int puntos = 100000; // numero de puntos que tendremos dentro del cuadrado
double radio=1; //radio de nuestro circulo
int contador = 0; //contador de puntos dentro de nuestro circulo
int puntosT; //puntos de los threads

//Función que nos revisa si un punto esta dentro del circulo.
int punto_en_circulo(double x, double y){
    double punto = (radio*radio) -( (x*x) + (y*y)); //revisa la posicion del la coordenada del punto
    if(punto>=0){
        return 1; //si es mayor a 0 entonces significa que estan dentro del circulo por lo que regresamos un 1 al conteo
    }else{
        return 0; //si es menor a 0  significa que esta fuera del circulo por lo que regresamos 0.
    }
}

//Funcion que nos contabiliza los puntos que hay dentro del círculo.
void * conteo(void * a){
    pthread_mutex_lock(&lock);//Bloqueamos el acceso a otros threads 
    int icont; //variable para generar la cantidad de coordenadas correspondientes a nuestra cantidad de puntos en el cuadrado
    for (icont=1; icont<=puntosT; icont++){
        double x =  (rand() / (RAND_MAX + 1.0)); //generamos numero decimal de coordenada x aleatorio de entre 0.0 y 1.0;
        double y =  (rand() / (RAND_MAX + 1.0)); //generamos numero decimal de coordenada y aleatorio de entre 0.0 y 1.0;
        int puntc=punto_en_circulo(x,y); // mandamos las coordenadas a la función
        if (puntc==1) {
            contador = contador + 1; //Aumentamos el contador
        }
    }
    pthread_mutex_unlock(&lock); //debloqueamos el acceso para otros threads 
}

int main(int argc, char *argv[]){
    int nt=THREADS;
    clock_t inicio, fin; //inicializamos las varibles de tipo reloj para el conteo del tiempo
    inicio = clock();//Iniciamos el conteo
    srand(time(NULL)); //funcion para que la funcion rand nos genere otros numeros aleatorios
    pthread_t threads[THREADS]; //Arreglo de threads
    puntosT= puntos/THREADS; //Calculamos los puntos que tendriamos que tener en cada thread 
    int i,i2;
    for(i=0; i<THREADS; i++){
        pthread_create(&threads[i], NULL, conteo, NULL); //Creamos cada thread con los valores necesarios
    }
    for(i2=0; i2<THREADS; i2++){
        pthread_join(threads[i2], NULL); //Garantiza que el thread indicado haya terminado 
    }
    double pi = 4.0*(double)contador/(double)puntos; //calculamos pi
    fin = clock();//ends time
    double tiempoTotal; //creamos una variable para guardar el tiempo total.
    tiempoTotal = ((double) (fin - inicio)) / CLOCKS_PER_SEC; //calculamos el tiempo 
    printf("Pi = %f\nTiempo usado=%f",pi, tiempoTotal); //imprimimos los resultados
    pthread_mutex_destroy(&lock); //borramos el mutex
}
