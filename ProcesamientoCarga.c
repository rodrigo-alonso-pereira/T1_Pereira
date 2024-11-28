#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"

//Macro que contiene el nombre archivo .txt
#define nombre_archivo_laberinto "procesamiento_3_3.IN"

//Variables Globales
//int p1 = 0, p2 = 0, p3 = 0; //Estado de ejecucion proceso 1, 2 y 3.
int fila = 0, columna = 0, n_proceso = 0; //Variables que almacenan la cantidad de filas y columnas de la matriz.
proceso* lista_procesos = NULL;
int** matriz = NULL; //Matriz que almacena los datos del archivo.
int tiempo = 0; //Variable que almacena el tiempo

// Nota: 1 == True, 0 == False.

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
            lista_procesos = (proceso*)malloc(sizeof(proceso) * n_proceso); //Reserva memoria para la lista de procesos.
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

void imprimirLista(proceso* lista, int n, int tiempo) {
    int i = 0;
    if (lista != NULL) {
        printf("---Lista de Procesos en Ejecucion---\n");
        for (i = 0; i < n; i++) {
            printf("Tiempo=%d -> [Proceso=%d, Carga=%d, Tiempo=%d] ", tiempo, lista[i].proceso, lista[i].carga, lista[i].tiempo);
        }
    } else {
        printf("[imprimirLista]Error: lista_procesos no ha sido inicializada.\n");
    }
}

int contadorTiempo(int** matriz, int filas, int columnas) {
    return 0;
}

proceso crearProceso(proceso nuevo_proceso, int proceso, int carga, int tiempo) {
    nuevo_proceso.proceso = proceso;
    nuevo_proceso.carga = carga;
    nuevo_proceso.tiempo = tiempo;
    printf("---Nuevo Proceso Creado---\n");
    printf("Proceso=%d, Carga=%d, Tiempo=%d\n", nuevo_proceso.proceso, nuevo_proceso.carga, nuevo_proceso.tiempo);
    return nuevo_proceso;
}

int evaluarExistenciaProceso(proceso* lista, int n, int proceso) {
    int i;
    for (i = 0; i < n; i++) {
        if (proceso == lista[i].proceso) {
            return 1;
        }
    }
    return 0;
}

void restarTiempoProceso(proceso* lista, int n, int proceso) {
    int i;
    for (i = 0; i < n; i++) {
        if (proceso == lista[i].proceso) {
            lista[i].tiempo -= 1;
        }
    }
}

int terminoProceso(proceso* lista, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (lista[i].tiempo != 0) {
            return 0;
        }
    }
    return 1;
}

void agregarProcesoLista(proceso* lista, int n, proceso proceso) {
    int i;
    for (i = 0; i < n; i++) {
        if (lista[i].carga == 0 && lista[i].proceso == 0 && lista[i].tiempo == 0) {
            lista[i] = proceso;
        }
    }
}

//int main(char* argv[]){
int main(){
    proceso nuevo_proceso = {};
    int condicion = 1;
    int i = 0, j = 0;
    //int** matriz = leerArchivo(argv[1]);
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.
    imprimirLista(lista_procesos, n_proceso, tiempo); //Imprime lista inicial

    //int** matrizAux = matriz;
    //while (terminoProceso(lista_procesos, n_proceso) == 1) { //Mientras no terminen los procesos.
    while (condicion) { //TODO: Eliminar luego de probar
        tiempo += 1; //Aumenta el tiempo.
        if (evaluarExistenciaProceso(lista_procesos, n_proceso, matriz[i][j]) == 0) { //Si no existe el proceso en la lista de procesos, se crea.
            agregarProcesoLista(lista_procesos, n_proceso, crearProceso(nuevo_proceso, matriz[i][j], i+1, matriz[i][j+1])); //i+1 = Carga 1, 2, 3, ..., n
        } //TODO: Evaluar si es conveniente tener los procesos iniciales antes del while y en while solo evaluar
        i++;
        condicion = 0;
        //TODO: Probar si 3) si resta el tiempo
        //TODO: Como ir avanzando entre filas de forma asincrona?
    }
    return 0;
}   