#ifndef AlgoritmosFase2_H
#define AlgoritmosFase2_H

#include "estructuras.h"

void DFS(sistema *sistema, int territorio_id, int persona_id, int *visitados, int *miembros_brote, int *contador);
void detectar_brotes(sistema *sistema);

#endif