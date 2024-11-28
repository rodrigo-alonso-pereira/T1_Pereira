#include <stdio.h>
#include <stdlib.h>

/*-----------OPERACIONES-----------*/
//Struct de un proceso
typedef struct proceso {
    int proceso;
    int tiempo;
} proceso;

/*-----------OPERACIONES-----------*/

// leer_archivo(nombre_archivo) lee el archivo con nombre nombre_archivo y retorna un puntero a FILE
// orden de complejidad: O(1)

void leerArchivo(char* nombre_archivo);

void imprimirMatriz(int** matriz, int filas, int columnas);

proceso crearProceso(proceso nuevo_proceso);

int evaluarExistenciaProceso(proceso* lista_proceso, int n, int proceso);

int evaluarEstadoProceso(proceso* lista_procesos, int n);

void restarTiempoProceso(proceso* lista_procesos, int n);