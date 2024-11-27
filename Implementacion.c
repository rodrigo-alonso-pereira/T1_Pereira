#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"

/*-----------OPERACIONES-----------*/

int** leerArchivo(char* nombre_archivo) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL){
        perror("Error fopen:");
        exit(EXIT_FAILURE);
    }

    //Evalua si se instancio bien el struct sb.
    struct stat sb;
    if (stat(nombre_archivo, &sb) == -1) {
        perror("Error stat: ");
        exit(EXIT_FAILURE);
    }

    int** matriz = (int**)malloc(5*sizeof(int*));
    for (int i = 0; i < 5; i++){
        matriz[i] = (int*)malloc(5*sizeof(int));
        for (int j = 0; j < 5; j++){
            fscanf(archivo, "%d", &matriz[i][j]);
        }
    }
    fclose(archivo);
    return matriz;
}