// mst.c
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include "Prim.h"

// Función auxiliar interna (no está en el .h porque es privada de este archivo)
int minKey(int key[], bool mstSet[], int V) {
    int min = INT_MAX, min_index;
    for (int v = 0; v < V; v++)
        if (mstSet[v] == false && key[v] < min)
            min = key[v], min_index = v;
    return min_index;
}

// Función auxiliar para imprimir
void printMST(int parent[], int** grafo, int V) {
    printf("\n--- Arbol de Expansion Minima (Prim) ---\n");
    printf("Origen - Destino \tCosto\n");
    int costoTotal = 0;
    for (int i = 1; i < V; i++) {
        printf("%d \t - %d \t\t%d \n", parent[i], i, grafo[i][parent[i]]);
        costoTotal += grafo[i][parent[i]];
    }
    printf("Costo Total Minimo: %d\n", costoTotal);
}

void primMST(int** grafo, int numVertices) {
    // Arrays dinámicos porque el tamaño V varía
    int* parent = (int*)malloc(numVertices * sizeof(int));
    int* key = (int*)malloc(numVertices * sizeof(int));
    bool* mstSet = (bool*)malloc(numVertices * sizeof(bool));

    // Inicialización
    for (int i = 0; i < numVertices; i++) {
        key[i] = INT_MAX;
        mstSet[i] = false;
    }

    key[0] = 0;     
    parent[0] = -1; // Raíz

    for (int count = 0; count < numVertices - 1; count++) {
        int u = minKey(key, mstSet, numVertices);
        mstSet[u] = true;

        for (int v = 0; v < numVertices; v++) {
            // grafo[u][v] accede a la matriz dinámica
            if (grafo[u][v] && mstSet[v] == false && grafo[u][v] < key[v]) {
                parent[v] = u;
                key[v] = grafo[u][v];
            }
        }
    }

    printMST(parent, grafo, numVertices);

    // Limpieza de memoria local
    free(parent);
    free(key);
    free(mstSet);
}

// Implementación de las utilerías de memoria
int** crearGrafo(int numVertices) {
    int** grafo = (int**)malloc(numVertices * sizeof(int*));
    for (int i = 0; i < numVertices; i++) {
        grafo[i] = (int*)malloc(numVertices * sizeof(int));
        // Inicializar en 0 (sin conexión)
        for(int j=0; j < numVertices; j++) grafo[i][j] = 0; 
    }
    return grafo;
}

void liberarGrafo(int** grafo, int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        free(grafo[i]);
    }
    free(grafo);
}