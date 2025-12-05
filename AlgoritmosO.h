#ifndef AlgoritmosO_H
#define AlgoritmosO_H

#include "estructuras.h"

/* funciones: merge sort */
void merge(persona *p, int left, int center, int right);
void merge_sort(persona *p, int left, int right);
int ordenamiento_por_riesgo_desc(sistema *s);

/* funciones: quick sort */
void quicksort(persona arr[], int low, int high);
void quicksort_def(sistema *sistema);
void funcion_quicksort(persona arr[], int n);
int partition(persona arr[], int low, int high);
int condition(persona a, persona b);
void swap(persona *a, persona *b);

/* funciones: heap sort */
typedef struct heap {
    persona *p;
    int n;
    int capacidad;
} heap;

void free_heap(heap *h);
void init_heap(heap *h, int capacidad);
void push_min_heap(heap *h, persona dato);
persona pop_min_heap(heap *h);
int ordenamiento_por_nombre_asc(sistema *s);

#endif