#include <stdio.h>
#include <stdlib.h>

/*-----------OPERACIONES-----------*/
//Struct de un proceso
typedef struct proceso {
    int carga;
    int proceso;
    int tiempo;
    int secuencia;
    int tiempo_inicial;
    int tiempo_final;
    int posicion_lista;
} proceso;

/*-----------OPERACIONES-----------*/

// leer_archivo(nombre_archivo) lee el archivo con nombre nombre_archivo y retorna un puntero a FILE
// orden de complejidad: O(1)

void leerArchivo(char* nombre_archivo);

void imprimirMatriz(int** matriz, int filas, int columnas);

void imprimirLista(proceso* lista, int n);

proceso crearProceso(proceso nuevo_proceso, int proceso, int carga, int tiempo, int secuencia, int posicion);

void crearListaProcesos(int n);

void generarPermutacion(int n);

int calcularFactorial(int n);

void obtenerUnaPermutacion(int a[], int dir[], int n);

void intercambiar(int* a, int* b);

int obtenerMobil(int a[], int dir[], int n);

int buscarPosicionMovil(int a[], int n, int mobile);

void ordenarListaProcesos(int* a, proceso* lista_procesos);

int factibilidadProcesos(proceso* lista, int n);

int sonDistintos(proceso* lista, int n);


