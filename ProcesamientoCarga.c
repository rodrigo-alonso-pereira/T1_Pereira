#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"

//Macro que contiene el nombre archivo .txt
#define nombre_archivo_laberinto "procesamiento_2_2.IN"

//Variables Globales
int fila = 0, columna = 0, n_proceso = 0, n_lista_procesos = 0;
//Variables que almacenan la cantidad de filas y columnas de la matriz.
int **matriz = NULL; //Matriz que almacena los datos del archivo.
proceso *lista_procesos = NULL; //Lista que almacena los procesos de forma secuencia de la matriz.
int tiempo = 0; //Variable que almacena el tiempo y que debe ser el menor
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
            for (int j = 0; j < fila; j++)
                matriz[j] = (int *) malloc(sizeof(int) * (columna * 2));
            //Reserva memoria para las columnas. (Col * 2) porque se guardan los nro. proceso y sus tiempos.
        } else {
            for (int k = 0, l = 0; k < columna; k++, l += 2) {
                sscanf(&linea_temp[l], "%d", &matriz[i - 1][k]);
            }
        }
        i++;
    }
    imprimirMatriz(matriz, fila, columna); //Imprime la matriz.
    if (archivo != NULL) {
        //Si el archivo no es nulo, se cierra.
        fclose(archivo);
    }
    free(linea_temp); //Libera la memoria de la linea temporal.
    //return matriz;
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
                ? printf("[c=%d_%d-%d_i=%d]]\n",
                         lista_procesos[i].carga,
                         lista_procesos[i].proceso,
                         lista_procesos[i].tiempo,
                         lista_procesos[i].posicion_lista)
                : printf("[c=%d_%d-%d_i=%d]], ",
                         lista_procesos[i].carga,
                         lista_procesos[i].proceso,
                         lista_procesos[i].tiempo,
                         lista_procesos[i].posicion_lista);
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
        for (j = 0; j < columna; j += 2) {
            proceso nuevo_proceso = crearProceso(nuevo_proceso, i + 1, matriz[i][j],
                                                 matriz[i][j + 1], i, count + 1);
            lista_procesos[count] = nuevo_proceso;
            count++;
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
int calcularFactorial(int n) {
    int res = 1;
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
    for (int i = 1; i < calcularFactorial(n); i++) {
        obtenerUnaPermutacion(a, dir, n);
        ordenarListaProcesos(a, lista_procesos);
        //Evaluacion de factibilidad de lista permutada
        if (factibilidadProcesos(lista_procesos, n_lista_procesos)) {
            printf("\n");
            for (int l = 0; l < n_lista_procesos; l++)
                printf("%d", a[l]);
            printf(" -> ES FACTIBLE\n");
            imprimirLista(lista_procesos, n_lista_procesos);
            //TODO: Aqui se deberia contar el tiempo y buscar el de menor tiempo
            evaluacionProcesos(lista_procesos, n_lista_procesos);
            count++;
        } /*else {
            printf("NO\n");
            imprimirLista(lista_procesos, n_lista_procesos);
        }*/
    }
    printf("\n\nEn total son %d permutaciones, de las cuales son %d factibles\n", calcularFactorial(n), count);
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

//Funcion que evalua si los procesos son distintos
/*int sonProcesosUno(proceso *lista, int n) {
    for (int i = 0; i < n; i++) {
        if (lista[i].proceso != 1) //Si los procesos
            return 0;
    }
    return 1;
}*/

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
    return 1; //Proceso FACTIBLE
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

proceso procesoNull(proceso proceso) {
    proceso.carga = -1;
    proceso.proceso = -1;
    proceso.tiempo = -1;
    proceso.secuencia = -1;
    proceso.tiempo_inicial = -1;
    proceso.tiempo_final = -1;
    proceso.posicion_lista = -1;
    return proceso;
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

//Funcion que evalua si la fila de una matriz esta con proceso null (-1)
int maquinaVacia(proceso **matriz, int fila) {
    if (matriz[fila][0].carga == -1 ||
        matriz[fila][0].proceso == -1 ||
        matriz[fila][0].tiempo == -1 ||
        matriz[fila][0].secuencia == -1 ||
        matriz[fila][0].tiempo_inicial == -1 ||
        matriz[fila][0].tiempo_final == -1 ||
        matriz[fila][0].posicion_lista == -1)
        return 1;
    return 0;
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


//TODO: Evaluar que retorna esta funcion.
void evaluacionProcesos(proceso *lista, int n) {
    int count = 0;
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
        for (int i = 0; i < fila; i++) { //Recorre las filas
            for (int j = 0; j < n_proceso; j++) { //Recorre las columnas
                // TODO: Condicion que no sea null.
                if (lista_proceso_aux[count].carga != -1) {
                    //Si el proceso no esta null (-1)
                    if (lista_proceso_aux[count].proceso == 1) { //Si el proceso ES el primero [C1_1-3_S0_ti-0_tf-0]
                        printf("El proceso es 1 -> [c=%d_%d-%d_S=%d_ti=%d-tf=%d]\n",
                            lista_proceso_aux[count].carga,
                            lista_proceso_aux[count].proceso,
                            lista_proceso_aux[count].tiempo,
                            lista_proceso_aux[count].secuencia,
                            lista_proceso_aux[count].tiempo_inicial,
                            lista_proceso_aux[count].tiempo_final);
                        matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                        if (j == 0) { //Si es la primera columna
                            matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Calcula el tiempo final [C1_1-3_S0_ti-0_tf-3]
                        } else {
                            matriz_aux[i][j].tiempo_inicial = matriz_aux[i][j - 1].tiempo_final; //Asigna tiempo inicial como el final anterior [C2_1-2_S0_ti-3_tf-0]
                            matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Calcula el tiempo final [C2_1-2_S0_ti-3_tf-5]
                        }
                        lista_proceso_aux[count] = procesoNull(matriz_aux[i][j]); //Asigna NULL al proceso de la lista de procesos auxiliar
                        printf("PROCESO 1\n");
                        imprimirLista(lista_proceso_aux, n_lista_procesos);
                    } else { //Si el proceso NO ES el primero [C3_2-3_S0_ti-0_tf-3]
                        //TODO: Revisar desde aqui
                        printf("El proceso NO es 1 -> [c=%d_%d-%d_S=%d_ti=%d-tf=%d]\n",
                            lista_proceso_aux[count].carga,
                            lista_proceso_aux[count].proceso,
                            lista_proceso_aux[count].tiempo,
                            lista_proceso_aux[count].secuencia,
                            lista_proceso_aux[count].tiempo_inicial,
                            lista_proceso_aux[count].tiempo_final);
                        if (maquinaVacia(matriz_aux, i)) { //Si la maquina esta vacia [C3_2-3_S0_ti-0_tf-3]
                            if (lista_proceso_aux[count].secuencia == 0) { //Si la secuencia ES la menor [C3_2-3_S0_ti-0_tf-3]
                                matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                                matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos
                                lista_proceso_aux[count] = procesoNull(matriz_aux[i][j]); //Asigna NULL al proceso evaluado de la lista de procesos auxiliar
                                printf("MAQUINA VACIA Y SECUENCIA 0\n");
                                imprimirLista(lista_proceso_aux, n_lista_procesos);
                            } else { //La secuencia NO ES la menor [C2_3-3_S1_ti-0_tf-0]
                                //Busca el proceso anterior == 0 -> no existe || != 0 -> existe y se asigna tiempo final a una variable aux
                                int t_final_sec_aux = buscarProcesoAnterior(matriz_aux, fila, n_proceso, lista_proceso_aux[count]); //tf=5
                                if (t_final_sec_aux != 0) { //El proceso anterior en secuencia existe?
                                    matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar [C2_3-3_S1_ti-0_tf-0]
                                    matriz_aux[i][j].tiempo_inicial = t_final_sec_aux; //[C2_3-3_S1_ti-5_tf-0]
                                    matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //[C2_3-3_S1_ti-5_tf-8]
                                    lista_proceso_aux[count] = procesoNull(matriz_aux[i][j]); //Asigna NULL al proceso evaluado de la lista de procesos auxiliar
                                    printf("MAQUINA VACIA Y SECUENCIA MAYOR\n");
                                    imprimirLista(lista_proceso_aux, n_lista_procesos);
                                } //Si no existe, paso al siguiente proceso
                            }
                        } else { //Si la maquina no esta vacia [C1_2-3_S1_ti-0_tf-0]
                            if (lista_proceso_aux[count].secuencia == 0) { //Si la secuencia ES la menor
                                matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar
                                matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos
                                lista_proceso_aux[count] = procesoNull(matriz_aux[i][j]); //Asigna NULL al proceso evaluado de la lista de procesos auxiliar
                                printf("MAQUINA NO VACIA Y SECUENCIA 0\n");
                                imprimirLista(lista_proceso_aux, n_lista_procesos);
                            } else { //La secuencia NO ES la menor [C1_2-3_S1_ti-0_tf-0] | [C1_3-3_S2_ti-0_tf-0]
                                //Busca el proceso anterior == 0 -> no existe || != 0 -> existe y se asigna tiempo final a una variable aux
                                int t_final_sec_aux = buscarProcesoAnterior(matriz_aux, fila, n_proceso, lista_proceso_aux[count]); //tf=3 | tf=6
                                if (t_final_sec_aux != 0) { //El proceso anterior en secuencia existe?
                                    matriz_aux[i][j] = lista_proceso_aux[count]; //Asigna el proceso a la matriz auxiliar [C1_2-3_S1_ti-0_tf-0] | [C1_3-3_S2_ti-0_tf-0]
                                    if (matriz_aux[i][j - 1].tiempo_final >= t_final_sec_aux) { //Si el tiempo final anterior es mayor que el tiempo de la secuencia anterior
                                        matriz_aux[i][j].tiempo_inicial = matriz_aux[i][j - 1].tiempo_final; //Asigna tiempo inicial el tiempo final del anterior [C1_2-3_S1_ti-3_tf-0]
                                    } else {
                                        matriz_aux[i][j].tiempo_inicial = t_final_sec_aux; //[C1_3-3_S2_ti-8_tf-0]
                                    }
                                    matriz_aux[i][j].tiempo_final = matriz_aux[i][j].tiempo_inicial + matriz_aux[i][j].tiempo; //Sumo tiempos [C1_2-3_S1_ti-3_tf-6] | [C1_3-3_S2_ti-8_tf-11]
                                    lista_proceso_aux[count] = procesoNull(matriz_aux[i][j]); //Asigna NULL al proceso evaluado de la lista de procesos auxiliar
                                    printf("MAQUINA NO VACIA Y SECUENCIA MAYOR\n");
                                    imprimirLista(lista_proceso_aux, n_lista_procesos);
                                } //Si no existe, paso al siguiente proceso
                            }
                        }
                    }
                }
                count++;
            }
        }
        printf("-------");
        imprimirLista(lista_proceso_aux, n_lista_procesos);
        printf("-------");
        imprimirMatrizProceso(matriz_aux, fila, n_proceso);
        break;
    }
    imprimirMatrizProceso(matriz_aux, fila, n_proceso);
    //TODO: Buscar tiempo mayor de la matriz y compara con tiempo.
    // Liberar memoria
    for (int i = 0; i < fila; i++) { //Liberar memoria de cada fila de matriz_aux
        free(matriz_aux[i]);
    }
    free(matriz_aux);
    free(lista_proceso_aux);
}


int main() {
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.
    crearListaProcesos(); //Pasar Matriz a Lista
    generarPermutacion(n_lista_procesos);
    //Implementar Johnson and Trotter algorithm -> Entrega lista de indices permutado

    //Por cada lista de indices, armar lista de procesos y evaluar que vengan en orden [[123] [123] [123]] -> True: Calculo tiempo, False: Vuelvo a permutar

    //Aplicar algoritmo para calcular tiempo.

    //Entregar tiempo optimo.
}
