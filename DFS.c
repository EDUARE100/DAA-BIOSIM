#ifndef AlgoritmosFase2_H
#define AlgoritmosFase2_H

#include "estructuras.h"
#include <stdio.h>

void DFS(sistema *sistema, int territorio_id, int persona_id, int *visitados, int *miembros_brote, int *contador) {
    
    // PROTECCIÓN: Si ya nos pasamos del total de personas, paramos (evitarcrash)
    if (*contador >= sistema->territorios[territorio_id].M) return;

    // Marcar como visitado
    visitados[persona_id] = 1;
    
    // Agregar a la lista
    miembros_brote[*contador] = persona_id;
    (*contador)++;

    // Buscar vecinos
    int M = sistema->territorios[territorio_id].M;
    for (int vecino = 0; vecino < M; vecino++) {
        
        // Verificamos conexión
        double peso = sistema->territorios[territorio_id].grafopersonas[persona_id][vecino];
        
        if (peso != -1.0) { // Si son amigos
            
            // Verificamos si el vecino está infectado
            int esta_infectado = (sistema->territorios[territorio_id].personas[vecino].estado == INFECTADO);
            
            // Verificamos si YA lo visitamos
            int ya_visitado = visitados[vecino];

            // Solo entramos si es infectado y NO ha sido visitado
            if (esta_infectado && !ya_visitado) {
                DFS(sistema, territorio_id, vecino, visitados, miembros_brote, contador);
            }
        }
    }
}

void detectar_brotes(sistema *sistema) {
    int total_brotes_global = 0;

    for (int i = 0; i < sistema->numterritorios; i++) {
        int M = sistema->territorios[i].M;
        if (M == 0) continue;

        // Resetear array de visitados para este territorio
        // Calloc inicializa todo en 0
        int *visitados = (int*)calloc(M, sizeof(int));
        int *miembros_brote = (int*)malloc(M * sizeof(int)); 

        for (int j = 0; j < M; j++) {
            // Solo iniciamos DFS si la persona está infectada y no ha sido visitada
            if (sistema->territorios[i].personas[j].estado == INFECTADO && visitados[j] == 0) {
                
                int contador_miembros = 0;
                DFS(sistema, i, j, visitados, miembros_brote, &contador_miembros);
                
                total_brotes_global++;
                printf("\n[Territorio %d - %s] > FOCO DETECTADO #%d\n", sistema->territorios[i].territorio_id, sistema->territorios[i].nombre,total_brotes_global);
                
                printf("Tamano del cluster: %d personas\n", contador_miembros);
                printf("Involucrados (ID): ");
                for(int k=0; k<contador_miembros; k++) {
                    // Accedemos al ID real usando el índice guardado
                    int id_persona = miembros_brote[k];
                    printf("%d ", sistema->territorios[i].personas[id_persona].id);
                }
                printf("\n");
            }
        }

        free(visitados);
        free(miembros_brote);
    }
    
    if (total_brotes_global == 0) printf("\nNo hay infectados activos.\n");
}

#endif