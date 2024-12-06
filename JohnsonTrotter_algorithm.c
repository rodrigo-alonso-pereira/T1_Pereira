// CPP program to print all permutations using
// Johnson and Trotter algorithm.
#include <stdio.h>
#include <stdlib.h>

int LEFT_TO_RIGHT = 1;
int RIGHT_TO_LEFT = 0;

//Busca y retorna la posicion del mayor número movil en el arreglo a[]
int searchArr(int a[], int n, int mobile)
{
    for (int i = 0; i < n; i++)
        if (a[i] == mobile)
            return i + 1;
}

// Encontrar el mayor número móvil en el arreglo a[].
int getMobile(int a[], int dir[], int n)
{
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
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Imprime una permutacion y actualiza direcciones
void printOnePerm(int a[], int dir[], int n) {
    int mobile = getMobile(a, dir, n); //Busca el mayor numero movil en el arreglo a[]
    int pos = searchArr(a, n, mobile); //Busca la posicion del mayor numero movil en el arreglo a[]

    //Intercamia el numero movil con elelemento en la direccion de su movimiento
    if (dir[a[pos - 1] - 1] == RIGHT_TO_LEFT) //Si la direccion del numero movil es RIGHT TO LEFT
        swap(&a[pos - 1], &a[pos - 2]); //Intercambia el numero movil con el elemento a su izquierda
    else if (dir[a[pos - 1] - 1] == LEFT_TO_RIGHT) //Si la direccion del numero movil es LEFT TO RIGHT
        swap(&a[pos], &a[pos - 1]); //Intercambia el numero movil con el elemento a su derecha

    //Cambiar las direcciones de los elementos mayores que el numero movil.
    for (int i = 0; i < n; i++) { //Recorre arreglo a[]
        if (a[i] > mobile) { //Si el elemento en la posicion i es mayor que el numero movil
            if (dir[a[i] - 1] == LEFT_TO_RIGHT) //Si la direccion del elemento es LEFT TO RIGHT
                dir[a[i] - 1] = RIGHT_TO_LEFT; //Cambia la direccion a RIGHT TO LEFT
            else if (dir[a[i] - 1] == RIGHT_TO_LEFT) //Si la direccion del elemento es RIGHT TO LEFT
                dir[a[i] - 1] = LEFT_TO_RIGHT; //Cambia la direccion a LEFT TO RIGHT
        }
    }
    //Imprime la permutacion actual
    for (int i = 0; i < n; i++)
        printf("%d", a[i]);
    printf(" ");
}

//Calcula el factorial de un número, que es el número total de permutaciones posibles.
int fact(int n)
{
    int res = 1;
    for (int i = 1; i <= n; i++)
        res = res * i;
    return res;
}

//Funcion solo llama printOnePerm() una a una de las permutaciones
void printPermutation(int n) {
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
    for (int i = 1; i < fact(n); i++)
        printOnePerm(a, dir, n);
    printf("\n\nEn total son %d permutaciones\n", fact(n));
}

int main()
{
    int n = 9; //Numero de elementos en la permutacion
    printPermutation(n);
    return 0;
}
