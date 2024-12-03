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

void imprimirLista(proceso* lista, int n) {
    int i = 0;
    if (lista != NULL) {
        printf("---Lista de Procesos en Ejecucion---\n");
        printf("Tiempo=%d -> ", tiempo);
        for (i = 0; i < n; i++) {
            printf("[[Carga=%d] => Proceso=%d, Tiempo=%d, Secuencia=%d] ", lista[i].carga, lista[i].proceso, lista[i].tiempo, lista[i].secuencia);
        }
        printf("\n");
    } else {
        printf("[imprimirLista]Error: lista_procesos no ha sido inicializada.\n");
    }
}

//Crea un proceso con los datos de matriz.
proceso crearProceso(proceso nuevo_proceso, int proceso, int carga, int tiempo, int secuencia) {
    nuevo_proceso.proceso = proceso;
    nuevo_proceso.carga = carga;
    nuevo_proceso.tiempo = tiempo;
    nuevo_proceso.secuencia = secuencia;
    printf("*****Nuevo Proceso Creado*****\n");
    printf("[Carga=%d] -> Proceso=%d, Tiempo=%d, Secuencia=%d\n", nuevo_proceso.carga, nuevo_proceso.proceso, nuevo_proceso.tiempo, nuevo_proceso.secuencia);
    return nuevo_proceso;
}

// Evalua si proceso esta en ejecucion o si hay otro de igual proceso en ejecucion
int evaluarEjecucionProcesoCarga(proceso* lista, int n, int proceso, int carga) {
    int i;
    for (i = 0; i < n; i++) {
        if (lista[i].proceso == proceso && lista[i].carga == carga) {
            return 1;
        }
    }
    return 0;
}

void liberarProceso(proceso* lista, int posicion) {
    proceso proceso_liberado = {};
    lista[posicion]= proceso_liberado;
}

void restarTiempoProceso(proceso* lista, int n, int proceso, int carga) {
    int i;
    for (i = 0; i < n; i++) {
        if (proceso == lista[i].proceso && carga == lista[i].carga) {
            lista[i].tiempo -= 1;
            break;
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

void agregarProcesoLista(proceso* lista, int n, proceso proceso, int posicion) {
    // Evaluo que el proceso no exista en la lista
    if (lista[posicion].carga == 0 && lista[posicion].proceso == 0 && lista[posicion].tiempo == 0) {
        lista[posicion] = proceso; //Agrego el proceso a la lista
    }
}

int buscarProceso(proceso* lista, int n, int proceso) {
    int i;
    for (i = 0; i < n; i++) {
        if (proceso == lista[i].proceso) {
            return 1;
        }
    }
    return 0;
}

int buscarProcesoTiempo(proceso* lista, int n, int proceso, int tiempo) {
    int i;
    for (i = 0; i < n; i++) {
        if (lista[i].proceso == proceso && lista[i].tiempo <= tiempo) {
            return 1;
        }
    }
    return 0;
}

//int main(char* argv[]){
int main(){
    proceso nuevo_proceso = {};
    int condicion = 1;
    int i = 0, j = 0, k = 0;
    //int** matriz = leerArchivo(argv[1]);
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.

    //Ingresa los procesos diferentes a la lista de procesos por primera vez.
    for (k = 0; k < n_proceso; k++) {
        lista_procesos[k] = crearProceso(nuevo_proceso, matriz[k][j], k + 1, matriz[k][j+1], j); //i+1 = Carga 1, 2, 3, ..., n
    }
    imprimirLista(lista_procesos, n_proceso, tiempo); //Imprime lista inicial

    //int** matrizAux = matriz;
    while (condicion) { //TODO: Eliminar luego de testear
    //while (terminoProceso(lista_procesos, n_proceso) != 1) { //Mientras no terminen los procesos.
        tiempo += 1; //Aumenta el tiempo.
        for (i = 0; i < n_proceso; i++) { // i avanza en filas de matriz
            int secuencia_proceso = lista_procesos[i].secuencia; //indica secuencia(columna) del proceso a evaluar
            if (evaluarEjecucionProcesoCarga(lista_procesos, n_proceso, matriz[i][secuencia_proceso], i+1) == 1) { //Si existe el proceso de una carga especifica
                if (lista_procesos[i].tiempo == 1) { //Si el tiempo del proceso es 1
                    proceso nuevo_proceso = {};
                    //lista_procesos[i] = nuevo_proceso; //Se agrega proceso vacio
                    matriz[i][secuencia_proceso + 1] = 0;
                    if (buscarProcesoTiempo(lista_procesos, n_proceso, matriz[i][secuencia_proceso+2], 1)) { //
                        nuevo_proceso = crearProceso(nuevo_proceso, matriz[i][secuencia_proceso+2], i+1, matriz[i][secuencia_proceso+3], secuencia_proceso+2); //Se crea un nuevo proceso
                        lista_procesos[i] = nuevo_proceso; //se agrega nuevo_proceso en posicion de proceso anterior (logrando el avance en secuencia)
                         //Marcar tiempo de proceso anterior como 0 en matriz
                    }
                } else { //Si el tiempo del proceso es mayor a 1
                    restarTiempoProceso(lista_procesos, n_proceso, matriz[i][secuencia_proceso], i+1); //Se resta 1 al tiempo del proceso
                }
            } else {
                if (!buscarProceso(lista_procesos, n_proceso, matriz[i][secuencia_proceso])) { //Si no existe el proceso en la lista de procesos, se crea.
                    nuevo_proceso = crearProceso(nuevo_proceso, matriz[i][secuencia_proceso], i+1, matriz[i][secuencia_proceso+1], secuencia_proceso);
                    agregarProcesoLista(lista_procesos, n_proceso, nuevo_proceso, i);
                }
            }
        }
        imprimirLista(lista_procesos, n_proceso, tiempo);

        if (tiempo == 6) {
            condicion = 0;
        }
    }
    imprimirMatriz(matriz, fila, columna);
    return tiempo;
}   