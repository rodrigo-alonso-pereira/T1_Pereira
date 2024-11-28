#include <stdio.h>
#include <stdlib.h>

/*-----------OPERACIONES-----------*/

// leer_archivo(nombre_archivo) lee el archivo con nombre nombre_archivo y retorna un puntero a FILE
// orden de complejidad: O(1)

void leerArchivo(char* nombre_archivo);

void imprimirMatriz(int** matriz, int filas, int columnas);