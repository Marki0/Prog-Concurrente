#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Estructura para pasar parámetros a los hilos
typedef struct {
    int *array;
    int low;
    int high;
} parametros;

// Función para fusionar dos mitades ordenadas
void merge(int *array, int low, int mid, int high) {
    int i, j, k;
    int n1 = mid - low + 1;
    int n2 = high - mid;

    int *left = malloc(n1 * sizeof(int));
    int *right = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        left[i] = array[low + i];
    for (j = 0; j < n2; j++)
        right[j] = array[mid + 1 + j];

    i = 0; j = 0; k = low;

    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            array[k++] = left[i++];
        else
            array[k++] = right[j++];
    }

    while (i < n1)
        array[k++] = left[i++];

    while (j < n2)
        array[k++] = right[j++];

    free(left);
    free(right);
}

// Función que se ejecutará en cada hilo
void *merge_sort(void *arg) {
    parametros *p = (parametros *)arg;
    int low = p->low;
    int high = p->high;
    int *array = p->array;

    if (low < high) {
        int mid = (low + high) / 2;

        pthread_t t1, t2;
        parametros *left = malloc(sizeof(parametros));
        parametros *right = malloc(sizeof(parametros));

        left->array = array;
        left->low = low;
        left->high = mid;

        right->array = array;
        right->low = mid + 1;
        right->high = high;

        // Crear hilos para cada mitad
        pthread_create(&t1, NULL, merge_sort, left);
        pthread_create(&t2, NULL, merge_sort, right);

        // Esperar a que ambos hilos terminen
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);

        // Fusionar las mitades
        merge(array, low, mid, high);

        free(left);
        free(right);
    }

    free(p);
    return NULL;
}

// Función principal de prueba
int main() {
    int i;
    int array[] = {12, 11, 13, 5, 6, 7, 18, 1};
    int n = sizeof(array) / sizeof(array[0]);

    printf("Arreglo original:\n");
    for (i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");

    parametros *datos = malloc(sizeof(parametros));
    datos->array = array;
    datos->low = 0;
    datos->high = n - 1;

    merge_sort(datos);

    printf("Arreglo ordenado:\n");
    for (i = 0; i < n; i++)
        printf("%d ", array[i]);
    printf("\n");

    return 0;
}
