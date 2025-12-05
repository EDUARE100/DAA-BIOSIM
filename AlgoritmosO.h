#ifndef AlgoritmosO_H
#define AlgoritmosO_H

#define ALPHABET_SIZE 128

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

/* funciones: greedy*/
void minimizar_riesgo_greedy(sistema *s, int terr_id, double riesgo_target);
int comparar_por_riesgo_desc(const void *a, const void *b) ;

/* funciones: trie */
typedef struct trienode {
    struct trienode *hijos[ALPHABET_SIZE];
    int es_final;
    int cepa_id; // Guardamos el ID de la cepa
    double valor_beta; // Guardamos el valor real para referencia
} trienode;

int clustering_cepas(sistema *s);
void mostrar_agrupamiento_automatico(trienode *root, sistema *s);
trienode* construir_clustering_betas(sistema *s);
void buscar_cluster_beta(trienode *root, char *prefijo_beta, sistema *s);
void recolectar_cluster(trienode *nodo, sistema *s);
void insertar_beta(trienode *root, double beta, int cepa_id);
trienode *crear_nodo(void);

#endif