#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"

//Macro que contiene el nombre archivo .txt
#define nombre_archivo_laberinto "procesamiento_3_3.IN"

//Variables Globales
int fila = 0, columna = 0, n_proceso = 0; //Variables que almacenan la cantidad de filas y columnas de la matriz.
int** matriz = NULL; //Matriz que almacena los datos del archivo.
int tiempo = 0; //Variable que almacena el tiempo

/*-----------OPERACIONES-----------*/

void leerArchivo(char* nombre_archivo) {
    //int** matriz = NULL;
    int i = 0;

    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL){
        perror("Error fopen:");
        exit(EXIT_FAILURE);
    }

    //Evalua si se instancio bien el struct sb.
    struct stat sb; //Struct que almacena la informacion del archivo.
    if (stat(nombre_archivo, &sb) == -1) { //Si es -1, hubo un error al intentar obtener la informacion del archivo.
        perror("Error stat: "); //Error descriptivo
        exit(EXIT_FAILURE); //Termina el programa con codigo de error.
    }
    //Lectura archivo y asignacion a matriz.
    char* linea_temp = malloc(sb.st_size + 1); //Reserva memoria para la linea temporal.
    matriz = (int**) malloc(sizeof(int*) * fila); //Reserva memoria para las filas.
    while (fscanf(archivo, "%[^\n]\n", linea_temp) != EOF) { //%[^\n] = Leer hasta salto de linea pero sin incluirlo. EOF = End Of File
        if (i == 0){
            sscanf(linea_temp, "%d %d\r",&fila, &n_proceso); //Lee la primera linea y guarda los valores en fila y col.
            columna = n_proceso * 2; //Proceso tiene numero y tiempo.
            matriz = (int**) malloc(sizeof(int*) * fila); //Reserva memoria para las filas.
            for (int j = 0; j < fila; j++)
                matriz[j] = (int*) malloc(sizeof(int) * (columna * 2)); //Reserva memoria para las columnas. (Col * 2) porque se guardan los nro. proceso y sus tiempos.
        } else {
            for (int k = 0, l = 0; k < columna; k++, l += 2) {
                sscanf(&linea_temp[l], "%d", &matriz[i-1][k]);
            }
        }
        i++;
    }
    imprimirMatriz(matriz, fila, columna); //Imprime la matriz.
    if (archivo != NULL) { //Si el archivo no es nulo, se cierra.
        fclose(archivo);
    }
    free(linea_temp); //Libera la memoria de la linea temporal.
    //return matriz;
}

void imprimirMatriz(int** matriz, int filas, int columnas) {
    int i, j;
    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main() {
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.
}
