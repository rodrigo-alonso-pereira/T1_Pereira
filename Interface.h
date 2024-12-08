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
    int posicion_lista; //Posicion en la lista de procesos inicial
} proceso;

/*-----------OPERACIONES-----------*/

// leer_archivo(nombre_archivo) lee el archivo con nombre nombre_archivo y retorna un puntero a FILE
// orden de complejidad: O(1)

void leerArchivo(char* nombre_archivo);

void imprimirMatriz(int** matriz, int filas, int columnas);

void imprimirLista(proceso* lista, int n);

proceso crearProceso(proceso nuevo_proceso, int proceso, int carga, int tiempo, int secuencia, int posicion);

void crearListaProcesos();

void generarPermutacion(int n);

int calcularFactorial(int n);

void obtenerUnaPermutacion(int a[], int dir[], int n);

void intercambiar(int* a, int* b);

int obtenerMobil(int a[], int dir[], int n);

int buscarPosicionMovil(int a[], int n, int mobile);

void ordenarListaProcesos(int* a, proceso* lista_procesos);

int factibilidadProcesos(proceso* lista, int n);

int sonCargasDistintas(proceso* lista, int n);

int sonProcesosIguales(proceso* lista, int n, int proceso);

void evaluacionProcesos(proceso* lista, int n);

int listaProcesosNull(proceso* lista, int n);

proceso procesoNull();

proceso** crearProcesoMatriz(proceso *lista, proceso **matriz, int fila, int columna);

void imprimirMatrizProceso(proceso** matriz, int fila, int columna);

int maquinaVacia(proceso** matriz, int fila);

int buscarTiempoSecuenciaAnterior(proceso** matriz, int fila, int columna);

int buscarProcesoAnterior(proceso** matriz, int fila, int columna, proceso proceso);

void imprimirProceso(proceso proceso);

int obtenerMayorTiempo(proceso** matriz, int fila, int columna);

void imprimirSecuenciaListaProcesos(proceso* lista, int n);