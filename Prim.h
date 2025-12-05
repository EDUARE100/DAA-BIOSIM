
#ifndef Prim_h
#define Prim_h

// Función principal que calcula e imprime el MST
// Recibe: la matriz de adyacencia (puntero doble) y la cantidad de nodos
void primMST(int** grafo, int numVertices);

// Función para ayudar a crear la matriz dinámicamente
int** crearGrafo(int numVertices);

// Liberación de memoria
void liberarGrafo(int** grafo, int numVertices);

#endif