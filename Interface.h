#include <stdio.h>
#include <stdlib.h>

/*-----------OPERACIONES-----------*/
//Struct de un proceso
typedef struct proceso {
    int proceso;
    int carga;
    int tiempo;
} proceso;

/*-----------OPERACIONES-----------*/

// leer_archivo(nombre_archivo) lee el archivo con nombre nombre_archivo y retorna un puntero a FILE
// orden de complejidad: O(1)

void leerArchivo(char* nombre_archivo);

void imprimirMatriz(int** matriz, int filas, int columnas);

void imprimirLista(proceso* lista, int n, int tiempo);

proceso crearProceso(proceso nuevo_proceso, int proceso, int carga, int tiempo);

int evaluarExistenciaProceso(proceso* lista, int n, int proceso);

int evaluarEstadoProceso(proceso* lista, int n);

void restarTiempoProceso(proceso* lista, int n, int proceso);

int terminoProceso(proceso* lista, int n);

void agregarProcesoLista(proceso* lista, int n, proceso proceso);

proceso* eliminarProceso(proceso* lista, int n, proceso proceso);