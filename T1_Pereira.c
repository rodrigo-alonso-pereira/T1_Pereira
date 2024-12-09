#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"
#include <time.h>

//Macro que contiene el nombre archivo .txt
#define nombre_archivo_laberinto "procesamiento_3_3.IN"

//Variables Globales
int fila = 0, columna = 0, n_proceso = 0, n_lista_procesos = 0;
//Variables que almacenan la cantidad de filas y columnas de la matriz.
int **matriz = NULL; //Matriz que almacena los datos del archivo.
proceso *lista_procesos = NULL; //Lista que almacena los procesos de forma secuencia de la matriz.
int tiempo_menor = 1000000000; //Variable que almacena el tiempo y que debe ser el menor
int contador_tiempo = 0; //Contador de tiempos menores
int LEFT_TO_RIGHT = 1;
int RIGHT_TO_LEFT = 0;

/*-----------OPERACIONES-----------*/

//TODO: Error en lectura de archivos 4x4 y +
void leerArchivo(char *nombre_archivo) {
    int i = 0;

    FILE *archivo = fopen(nombre_archivo, "r");
    if (archivo == NULL) {
        perror("Error fopen:");
        exit(EXIT_FAILURE);
    }

    //Evalua si se instancio bien el struct sb.
    struct stat sb; //Struct que almacena la informacion del archivo.
    if (stat(nombre_archivo, &sb) == -1) {
        //Si es -1, hubo un error al intentar obtener la informacion del archivo.
        perror("Error stat: "); //Error descriptivo
        exit(EXIT_FAILURE); //Termina el programa con codigo de error.
    }
    //Lectura archivo y asignacion a matriz.
    char *linea_temp = malloc(sb.st_size + 1); //Reserva memoria para la linea temporal.
    while (fscanf(archivo, "%[^\n]\n", linea_temp) != EOF) {
        //%[^\n] = Leer hasta salto de linea pero sin incluirlo. EOF = End Of File
        if (i == 0) {
            sscanf(linea_temp, "%d %d\r", &fila, &n_proceso); //Lee la primera linea y guarda los valores en fila y col.
            columna = n_proceso * 2; //Cantidad de columnas de la matriz.
            n_lista_procesos = fila * n_proceso; //Cantidad de elementos de la lista de procesos.
            lista_procesos = (proceso *) malloc(sizeof(proceso) * n_lista_procesos); //Creacion de la lista de procesos.
            matriz = (int **) malloc(sizeof(int *) * fila); //Reserva memoria para las filas.
            for (int j = 0; j < fila; j++) //Reserva memoria para las columnas. (Col * 2) porque se guardan los nro. proceso y sus tiempos.
                matriz[j] = (int *) malloc(sizeof(int) * (columna * 2));
        } else {
            char *ptr = linea_temp; // Puntero temporal para recorrer la línea
            for (int k = 0; k < columna; k++) {
                sscanf(ptr, "%d", &matriz[i - 1][k]);
                while (*ptr != ' ' && *ptr != '\0') ptr++; // Avanza al siguiente espacio o fin de línea
                if (*ptr == ' ') ptr++; // Salta el espacio
            }
            /*
            for (int k = 0, l = 0; k < columna; k++, l += 2) {
                sscanf(&linea_temp[l], "%d", &matriz[i - 1][k]);
            }
            */
        }
        i++;
    }
    imprimirMatriz(matriz, fila, columna); //Imprime la matriz.
    if (archivo != NULL) { //Si el archivo no es nulo, se cierra.
        fclose(archivo);
    }
    free(linea_temp); //Libera la memoria de la linea temporal.
}

void imprimirMatriz(int **matriz, int filas, int columnas) {
    int i, j;
    for (i = 0; i < filas; i++) {
        for (j = 0; j < columnas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

void imprimirLista(proceso *lista, int n) {
    int i = 0;
    if (lista != NULL) {
        printf("---Lista de Procesos---\n");
        printf("[");
        for (i = 0; i < n; i++) {
            (i == n - 1)
                ? printf("[c=%d_%d-%d_S=%d_ti=%d-tf=%d_i=%d]]\n",
                         lista[i].carga,
                         lista[i].proceso,
                         lista[i].tiempo,
                         lista[i].secuencia,
                         lista[i].tiempo_inicial,
                         lista[i].tiempo_final,
                         lista[i].posicion_lista)
                : printf("[c=%d_%d-%d_S=%d_ti=%d-tf=%d_i=%d]], ",
                         lista[i].carga,
                         lista[i].proceso,
                         lista[i].tiempo,
                         lista[i].secuencia,
                         lista[i].tiempo_inicial,
                         lista[i].tiempo_final,
                         lista[i].posicion_lista);
        }
    } else {
        printf("[imprimirLista]Error: lista_procesos no ha sido inicializada.\n");
    }
}

//Crea un proceso con los datos de matriz.
proceso crearProceso(proceso nuevo_proceso, int carga, int proceso, int tiempo, int secuencia, int posicion) {
    nuevo_proceso.proceso = proceso;
    nuevo_proceso.carga = carga;
    nuevo_proceso.tiempo = tiempo;
    nuevo_proceso.secuencia = secuencia;
    nuevo_proceso.tiempo_inicial = 0;
    nuevo_proceso.tiempo_final = 0;
    nuevo_proceso.posicion_lista = posicion;
    return nuevo_proceso;
}

//Crea lista de procesos
void crearListaProcesos() {
    int i, j, count = 0;
    for (i = 0; i < fila; i++) {
        int secuencia = 0;
        for (j = 0; j < columna; j += 2) {
            proceso nuevo_proceso = crearProceso(
                nuevo_proceso,
                i + 1,
                matriz[i][j],
                matriz[i][j + 1],
                secuencia,
                count + 1);
            lista_procesos[count] = nuevo_proceso;
            count++;
            secuencia++;
        }
    }
    imprimirLista(lista_procesos, n_lista_procesos);
}

//Busca y retorna la posicion del mayor número movil en el arreglo a[]
int buscarPosicionMovil(int a[], int n, int mobile) {
    for (int i = 0; i < n; i++)
        if (a[i] == mobile)
            return i + 1;
}

// Encontrar el mayor número móvil en el arreglo a[].
int obtenerMobil(int a[], int dir[], int n) {
    int mobile_prev = 0, mobile = 0;
    for (int i = 0; i < n; i++) {
        //Recorre arreglo a[] de tamaño n
        // direccion 0 representa RIGHT TO LEFT (0).
        if (dir[a[i] - 1] == RIGHT_TO_LEFT && i != 0) {
            if (a[i] > a[i - 1] && a[i] > mobile_prev) {
                mobile = a[i];
                mobile_prev = mobile;
            }
        }

        // direccion 1 representa LEFT TO RIGHT (1).
        if (dir[a[i] - 1] == LEFT_TO_RIGHT && i != n - 1) {
            if (a[i] > a[i + 1] && a[i] > mobile_prev) {
                mobile = a[i];
                mobile_prev = mobile;
            }
        }
    }

    if (mobile == 0 && mobile_prev == 0)
        return 0;
    else
        return mobile;
}

//Función para intercambiar dos enteros
void intercambiar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Imprime una permutacion y actualiza direcciones
void obtenerUnaPermutacion(int a[], int dir[], int n) {
    int mobile = obtenerMobil(a, dir, n); //Busca el mayor numero movil en el arreglo a[]
    int pos = buscarPosicionMovil(a, n, mobile); //Busca la posicion del mayor numero movil en el arreglo a[]

    //Intercamia el numero movil con elelemento en la direccion de su movimiento
    if (dir[a[pos - 1] - 1] == RIGHT_TO_LEFT) //Si la direccion del numero movil es RIGHT TO LEFT
        intercambiar(&a[pos - 1], &a[pos - 2]); //Intercambia el numero movil con el elemento a su izquierda
    else if (dir[a[pos - 1] - 1] == LEFT_TO_RIGHT) //Si la direccion del numero movil es LEFT TO RIGHT
        intercambiar(&a[pos], &a[pos - 1]); //Intercambia el numero movil con el elemento a su derecha

    //Cambiar las direcciones de los elementos mayores que el numero movil.
    for (int i = 0; i < n; i++) {
        //Recorre arreglo a[]
        if (a[i] > mobile) {
            //Si el elemento en la posicion i es mayor que el numero movil
            if (dir[a[i] - 1] == LEFT_TO_RIGHT) //Si la direccion del elemento es LEFT TO RIGHT
                dir[a[i] - 1] = RIGHT_TO_LEFT; //Cambia la direccion a RIGHT TO LEFT
            else if (dir[a[i] - 1] == RIGHT_TO_LEFT) //Si la direccion del elemento es RIGHT TO LEFT
                dir[a[i] - 1] = LEFT_TO_RIGHT; //Cambia la direccion a LEFT TO RIGHT
        }
    }
    //Imprime la permutacion actual
    /*printf("\n");
    for (int i = 0; i < n; i++)
        printf("%d", a[i]);
    printf(" -> ");*/
}

//Calcula el factorial de un número, que es el número total de permutaciones posibles.
long long int calcularFactorial(int n) {
    long long int res = 1;
    for (int i = 1; i <= n; i++)
        res = res * i;
    return res;
}

//Funcion solo llama printOnePerm() una a una de las permutaciones
void generarPermutacion(int n) {
    int count = 0; //Contador de factible
    //Arreglo para almacenar los elementos
    int a[n];

    //Array para almacenar la direccion de cada elemento
    int dir[n];

    //Llena lista a[] con numero de 0 a n.
    printf("\n");
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
    if (factibilidadProcesos(lista_procesos, n_lista_procesos)) {
        printf("\n");
        for (int l = 0; l < n_lista_procesos; l++)
            printf("%d", a[l]);
        printf(" -> ES FACTIBLE\n");
        imprimirLista(lista_procesos, n_lista_procesos);
        count++;
    }

    //Inicializa todas las direcciones con RIGHT TO LEFT = 0.
    for (int i = 0; i < n; i++)
        dir[i] = RIGHT_TO_LEFT;

    //Genera todas las permutaciones en orden
    for (long long int i = 1; i < calcularFactorial(n); i++) {
        obtenerUnaPermutacion(a, dir, n);
        ordenarListaProcesos(a, lista_procesos);
        //Evaluacion de factibilidad de lista permutada
        if (factibilidadProcesos(lista_procesos, n_lista_procesos)) {
            printf("\n");
            for (int l = 0; l < n_lista_procesos; l++)
                printf("%d", a[l]);
            printf(" -> ES FACTIBLE\n");
            imprimirLista(lista_procesos, n_lista_procesos);
            evaluacionProcesos(lista_procesos, n_lista_procesos);
            count++;
        }
    }
    printf("\n\nEn total son %lli permutaciones, de las cuales son %d factibles\n", calcularFactorial(n), count);
}

//Ordena la lista de procesos segun la permutacion
void ordenarListaProcesos(int *a, proceso *lista_procesos) {
    int i, j;
    proceso *lista_aux = (proceso *) malloc(sizeof(proceso) * n_lista_procesos);
    for (i = 0; i < n_lista_procesos; i++) {
        for (j = 0; j < n_lista_procesos; j++) {
            if (lista_procesos[j].posicion_lista == a[i])
                lista_aux[i] = lista_procesos[j];
        }
    }

    for (i = 0; i < n_lista_procesos; i++) {
        lista_procesos[i] = lista_aux[i];
    }
    free(lista_aux);
    //imprimirLista(lista_procesos, n_lista_procesos);
}

//Funcion que evalua si los procesos son iguales y con orden de procesos 1, 2, 3 ... n
int sonProcesosIguales(proceso *lista, int n, int proceso) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (lista[i].proceso != lista[j].proceso || lista[i].proceso != proceso) // Si los procesos son distintos o no son iguales al número
                return 0;
        }
    }
    return 1; //Los procesos son iguales
}

//Funcion que evalua si los procesos son distintos
int sonCargasDistintas(proceso *lista, int n) { //n
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (lista[i].carga == lista[j].carga) //Si las cargas son iguales
                return 0;
        }
    }
    return 1; //Las cargas son distintas
}

//Funcion que evalua si los procesos son factibles
int factibilidadProcesos(proceso *lista, int n) {
    int i, count = 1;
    for (i = 0; i < n; i = i + n_proceso) { //i avanza segun numero de proceso
        if (!sonCargasDistintas(&lista[i], n_proceso) || !sonProcesosIguales(&lista[i], n_proceso, count)) //Se envia puntero desde posicion i y evaluo
            return 0; //NO ES factible
        count++;
    }
    if (lista[0].secuencia == 0)
        return 1; //Proceso FACTIBLE

    return 0;
}

int listaProcesosNull(proceso *lista, int n) {
    for (int i = 0; i < n; i++) {
        if (lista[i].carga != -1 ||
            lista[i].proceso != -1 ||
            lista[i].tiempo != -1 ||
            lista[i].secuencia != -1 ||
            lista[i].tiempo_inicial != -1 ||
            lista[i].tiempo_final != -1 ||
            lista[i].posicion_lista != -1) {
            return 0; // La lista no es completamente NULL
        }
    }
    return 1; // Toda la lista es NULL
}

proceso procesoNull() {
    proceso nuevo;
    nuevo.carga = -1;
    nuevo.proceso = -1;
    nuevo.tiempo = -1;
    nuevo.secuencia = -1;
    nuevo.tiempo_inicial = -1;
    nuevo.tiempo_final = -1;
    nuevo.posicion_lista = -1;
    return nuevo;
}

void imprimirMatrizProceso(proceso **matriz, int fila, int columna) {
    int i, j;
    printf("\n");
    for (i = 0; i < fila; i++) {
        for (j = 0; j < columna; j++) {
            printf("[c=%d_%d-%d_S=%d_ti=%d-tf=%d] ",
                   matriz[i][j].carga,
                   matriz[i][j].proceso,
                   matriz[i][j].tiempo,
                   matriz[i][j].secuencia,
                   matriz[i][j].tiempo_inicial,
                   matriz[i][j].tiempo_final);
        }
        printf("\n");
    }
}

void imprimirProceso(proceso proceso) {
    printf("[c=%d_%d-%d_S=%d_ti=%d-tf=%d_i=%d]",
                   proceso.carga,
                   proceso.proceso,
                   proceso.tiempo,
                   proceso.secuencia,
                   proceso.tiempo_inicial,
                   proceso.tiempo_final,
                   proceso.posicion_lista);
}

//Funcion que evalua si la fila de una matriz esta con proceso null (-1)
int maquinaVacia(proceso **matriz, int fila) {
    for (int i = 0; i < n_proceso; i++) {
        if (matriz[fila][i].carga != -1 ||
            matriz[fila][i].proceso != -1 ||
            matriz[fila][i].tiempo != -1 ||
            matriz[fila][i].secuencia != -1 ||
            matriz[fila][i].tiempo_inicial != -1 ||
            matriz[fila][i].tiempo_final != -1 ||
            matriz[fila][i].posicion_lista != -1)
            return 0;
    }
    return 1;
}

//Funcion que busca un proceso anterior y retorna su tiempo_final, si no existe retorna 0;
int buscarProcesoAnterior(proceso **matriz, int fila, int columna, proceso proceso) {
    for (int i = 0; i < fila; i++) {
        for (int j = 0; j < columna; j++) {
            if ((matriz[i][j].carga == proceso.carga) && (matriz[i][j].secuencia == proceso.secuencia - 1))
                return matriz[i][j].tiempo_final;
        }
    }
    return 0;
}

//Funcion que busca al mayor tiempo final entre todos los procesos.
int obtenerMayorTiempo(proceso** matriz, int fila, int columna) {
    int tiempo = 0;
    for (int i = 0; i < fila; i++) {
        for (int j = 0; j < columna; j++) {
            if (matriz[i][j].tiempo_final > tiempo)
                tiempo = matriz[i][j].tiempo_final;
        }
    }
    return tiempo;
}

int buscarTiempoAnteriorFila(proceso** matriz, int fila, int columna) {
    int tiempo_final = 0;
    for (int i = 0; i < columna; i++) {
        if (matriz[fila][i].tiempo_final != -1 && matriz[fila][i].tiempo_final > tiempo_final)
            tiempo_final = matriz[fila][i].tiempo_final;
    }
    return tiempo_final;
}

void evaluacionProcesos(proceso *lista, int n) {
    proceso **matriz_aux = malloc(sizeof(proceso *) * fila); //Reserva memoria para las filas de matriz aux.
    proceso *lista_proceso_aux = malloc(sizeof(proceso) * n); //Reserva memoria para la lista de procesos aux.
    for (int i = 0; i < n; i++)
        lista_proceso_aux[i] = lista[i]; //Copia la lista de procesos a la lista de procesos aux.

    proceso nuevo_proceso = {};
    for (int i = 0; i < fila; i++) {
        matriz_aux[i] = (proceso *) malloc(sizeof(proceso) * n_proceso); //Reserva memoria para las columnas de matriz aux.
        for (int j = 0; j < n_proceso; j++) {
            matriz_aux[i][j] = procesoNull(nuevo_proceso); //Asigna procesos NULL a la matriz auxiliar
        }
    }
    //Mientras la lista de procesos no sea completamente NULL
    while (!listaProcesosNull(lista_proceso_aux, n)) {
        int count = 0;
        for (int i = 0; i < fila; i++) { //Recorre las filas
            for (int j = 0; j < n_proceso; j++) { //Recorre las columnas
                //printf("\nPROCESO A EVALUAR -> "); //BORRAR
                //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                //printf("\n"); //BORRAR
                if (lista_proceso_aux[count].carga != -1) { //Si el proceso no esta null (-1)
                    if (maquinaVacia(matriz_aux, lista_proceso_aux[count].proceso - 1)) { //Si la maquina esta vacia
                        if (lista_proceso_aux[count].secuencia == 0) { //Si la secuencia ES la menor
                            //printf("MAQUINA VACIA Y SECUENCIA MENOR -> "); //BORRAR
                            //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                            //printf("\n"); //BORRAR
                            matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                            matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos
                            lista_proceso_aux[count] = procesoNull(); // Asigna NULL directamente al proceso de la lista de procesos auxiliar
                            //imprimirMatrizProceso(matriz_aux, fila, n_proceso); //BORRAR
                            //printf("\n"); //BORRAR
                            //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                            //printf("\n"); //BORRAR
                        } else { //La secuencia NO ES la menor
                            //Busca el proceso anterior == 0 -> no existe || != 0 -> existe y se asigna tiempo final a una variable aux
                            int t_final_sec_aux = buscarProcesoAnterior(matriz_aux, fila, n_proceso, lista_proceso_aux[count]); //tiempo final es proceso secuencia anterior
                            if (t_final_sec_aux != 0) { //El proceso anterior termino?
                                //printf("MAQUINA VACIA Y SECUENCIA MAYOR -> "); //BORRAR
                                //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                                //printf("\n"); //BORRAR
                                matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                                matriz_aux[i][j].tiempo_inicial = t_final_sec_aux; //Tiempo inicial es tiempo secuencia anterior
                                matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo;
                                lista_proceso_aux[count] = procesoNull(); // Asigna NULL directamente al proceso de la lista de procesos auxiliar
                                //imprimirMatrizProceso(matriz_aux, fila, n_proceso); //BORRAR
                                //printf("\n"); //BORRAR
                                //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                                //printf("\n"); //BORRAR
                            } else { //Si el proceso anterior NO termino, PASO.
                                //printf("MAQUINA VACIA Y SECUENCIA MAYOR -> PASO -> "); //BORRAR
                                //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                                //printf("\n"); //BORRAR
                                //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                                //printf("\n"); //BORRAR
                            }
                        }
                    } else { //La maquina no esta vacia
                        if (lista_proceso_aux[count].secuencia == 0) { //Si la secuencia ES la menor
                            //printf("MAQUINA NO VACIA Y SECUENCIA MENOR -> "); //BORRAR
                            //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                            //printf("\n"); //BORRAR
                            matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                            int tiempo_final_ant = buscarTiempoAnteriorFila(matriz_aux, lista_proceso_aux[count].proceso - 1, n_proceso);
                            matriz_aux[i][j].tiempo_inicial = tiempo_final_ant; //Tiempo inicial es tiempo de proceso anterior
                            //matriz_aux[i][j].tiempo_inicial = matriz_aux[i][j-1].tiempo_final; //Tiempo inicial es tiempo de proceso anterior
                            matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos
                            lista_proceso_aux[count] = procesoNull(); //Asigna NULL directamente al proceso de la lista de procesos auxiliar
                            //imprimirMatrizProceso(matriz_aux, fila, n_proceso); //BORRAR
                            //printf("\n"); //BORRAR
                            //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                            //printf("\n"); //BORRAR
                        } else { //La secuencia NO ES la menor
                            //Busca el proceso anterior == 0 -> no existe || != 0 -> existe y se asigna tiempo final a una variable aux
                            int t_final_sec_aux = buscarProcesoAnterior(matriz_aux, fila, n_proceso, lista_proceso_aux[count]); //tiempo final es proceso secuencia anterior
                            if (t_final_sec_aux != 0) { //El proceso anterior termino?
                                //printf("MAQUINA NO VACIA Y SECUENCIA MAYOR -> "); //BORRAR
                                //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                                //printf("\n"); //BORRAR
                                matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                                int tiempo_final_ant = buscarTiempoAnteriorFila(matriz_aux, lista_proceso_aux[count].proceso - 1, n_proceso);
                                if (tiempo_final_ant >= t_final_sec_aux) { //Si el tiempo final anterior es mayor que el tiempo final de la secuencia anterior
                                    matriz_aux[i][j].tiempo_inicial = tiempo_final_ant; //Asigna tiempo inicial el tiempo final del anterior [C1_2-3_S1_ti-3_tf-0]
                                } else { //Tiempo final secuencia anterios es mayor
                                    matriz_aux[i][j].tiempo_inicial = t_final_sec_aux; //Se asigna tiempo secuencia anterior como inicial
                                }
                                matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos k
                                lista_proceso_aux[count] = procesoNull(); // Asigna NULL directamente al proceso de la lista de procesos auxiliar
                                //imprimirMatrizProceso(matriz_aux, fila, n_proceso); //BORRAR
                                //printf("\n"); //BORRAR
                                //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                                //printf("\n"); //BORRAR
                            } else {
                                //Si el proceso anterior NO termino, PASO.
                                //printf("MAQUINA NO VACIA Y SECUENCIA MAYOR -> PASO -> "); //BORRAR
                                //imprimirProceso(lista_proceso_aux[count]); //BORRAR
                                //printf("\n"); //BORRAR
                                //imprimirLista(lista_proceso_aux, n_lista_procesos); //BORRAR
                                //printf("\n"); //BORRAR
                            }
                        }
                    }
                }
                count++;
            }
        }
    }
    //printf("\nMatriz Final");
    //imprimirMatrizProceso(matriz_aux, fila, n_proceso);
    int tiempo_parcial = obtenerMayorTiempo(matriz_aux, fila, n_proceso);
    printf("Tiempo de esta permutacion es de: %d\n", tiempo_parcial);

    if (tiempo_menor > tiempo_parcial) {
        printf("***Nuevo tiempo menor encontrado %d***\n", tiempo_parcial);
        tiempo_menor = tiempo_parcial;
        contador_tiempo = 1;
    } else if (tiempo_menor == tiempo_parcial)
        contador_tiempo++;

    // Liberar memoria
    for (int i = 0; i < fila; i++) { //Liberar memoria de cada fila de matriz_aux
        free(matriz_aux[i]);
    }
    free(matriz_aux);
    free(lista_proceso_aux);
}

int main() {
    clock_t clock_ini, clock_fin;
    float tiempo_algoritmo = 0;
    clock_ini = clock();
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.
    crearListaProcesos(); //Pasar Matriz a Lista
    generarPermutacion(n_lista_procesos); //Johnson and Trotter algorithm

    printf("\n");
    printf("--------------------------------------------------------\n");
    printf("*** El tiempo optimo para esta matriz %dx%d es de %d ***\n", fila, n_proceso, tiempo_menor);
    printf("--------------------------------------------------------\n");
    printf("\n");
    printf("-> Existen %d formas de ordenar la carga para obtener el tiempo de %d\n", contador_tiempo, tiempo_menor);
    clock_fin = clock();
	tiempo_algoritmo = (float)((clock_fin - clock_ini) / CLOCKS_PER_SEC);
	printf("\nTiempo del algoritmo en segundos: %.2f  \n", tiempo_algoritmo);
}
