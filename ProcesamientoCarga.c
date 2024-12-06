#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "Interface.h"

//Macro que contiene el nombre archivo .txt
#define nombre_archivo_laberinto "procesamiento_3_3.IN"

//Variables Globales
int fila = 0, columna = 0, n_proceso = 0, n_lista_procesos = 0; //Variables que almacenan la cantidad de filas y columnas de la matriz.
int** matriz = NULL; //Matriz que almacena los datos del archivo.
proceso* lista_procesos = NULL; //Lista que almacena los procesos de forma secuencia de la matriz.
int tiempo = 0; //Variable que almacena el tiempo
int LEFT_TO_RIGHT = 1;
int RIGHT_TO_LEFT = 0;

/*-----------OPERACIONES-----------*/

void leerArchivo(char* nombre_archivo) {
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
            sscanf(linea_temp, "%d %d\r", &fila, &n_proceso); //Lee la primera linea y guarda los valores en fila y col.
            columna = n_proceso * 2; //Proceso tiene numero y tiempo.
            n_lista_procesos = fila * n_proceso; //Cantidad de elementos de la lista de procesos.
            lista_procesos = (proceso*)malloc(sizeof(proceso) * n_lista_procesos); //Creacion de la lista de procesos.
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
        printf("---Lista de Procesos---\n");
        printf("[");
        for (i = 0; i < n; i++) {
            (i == n-1) ? printf("[%d-%d]]\n", lista_procesos[i].proceso, lista_procesos[i].tiempo) : printf("[%d-%d], ", lista_procesos[i].proceso, lista_procesos[i].tiempo);
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
void crearListaProcesos(int n) {
    int i, j, count = 0;
    for (i = 0; i < fila ; i++) {
        for (j = 0; j < columna; j+=2) {
            proceso nuevo_proceso = crearProceso(nuevo_proceso, i+1, matriz[i][j], matriz[i][j+1], i, count);
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
    for (int i = 0; i < n; i++) { //Recorre arreglo a[] de tamaño n
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
    for (int i = 0; i < n; i++) { //Recorre arreglo a[]
        if (a[i] > mobile) { //Si el elemento en la posicion i es mayor que el numero movil
            if (dir[a[i] - 1] == LEFT_TO_RIGHT) //Si la direccion del elemento es LEFT TO RIGHT
                dir[a[i] - 1] = RIGHT_TO_LEFT; //Cambia la direccion a RIGHT TO LEFT
            else if (dir[a[i] - 1] == RIGHT_TO_LEFT) //Si la direccion del elemento es RIGHT TO LEFT
                dir[a[i] - 1] = LEFT_TO_RIGHT; //Cambia la direccion a LEFT TO RIGHT
        }
    }
    //TODO: Aqui deberia retornar la lista de indices permutadas
    //Imprime la permutacion actual
    for (int i = 0; i < n; i++)
        printf("%d", a[i]);
    printf(" ");
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
    //Arreglo para almacenar los elementos
    int a[n];

    //Array para almacenar la direccion de cada elemento
    int dir[n];

    //Llena lista a[] con numero de 0 a n.
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
        printf("%d", a[i]);
    }
    printf("\n");

    //Inicializa todas las direcciones con RIGHT TO LEFT = 0.
    for (int i = 0; i < n; i++)
        dir[i] = RIGHT_TO_LEFT;

    //Genera todas las permutaciones en orden
    for (int i = 1; i < calcularFactorial(n); i++)
        obtenerUnaPermutacion(a, dir, n);
        //TODO: Aqui debo evaluar si la permutacion es factible
    printf("\n\nEn total son %d permutaciones\n", calcularFactorial(n));
}

int main() {
    leerArchivo(nombre_archivo_laberinto); //Lee el archivo y asigna los valores a la matriz.

    //Pasar Matriz a Lista
    crearListaProcesos(n_lista_procesos);

    //Implementar Johnson and Trotter algorithm -> Entrega lista de indices permutado
    generarPermutacion(n_lista_procesos);

    //Por cada lista de indices, armar lista de procesos y evaluar que vengan en orden [[123] [123] [123]] -> True: Calculo tiempo, False: Vuelvo a permutar

    //Aplicar algoritmo para calcular tiempo.

}
