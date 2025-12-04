#ifndef AlgoritmosFase2_H
#define AlgoritmosFase2_H

#include "estructuras.h"
#include <stdio.h>

void DFS_buscar_brote(sistema *s, int t_id, int p_id, int *visitados, int *miembros_brote, int *contador) {
    
    // PROTECCIÓN: Si ya nos pasamos del total de personas, parar (evitaR crash)
    if (*contador >= s->territorios[t_id].M) return;

    // Marcar como visitado
    visitados[p_id] = 1;
    
    // Agregar a la lista
    miembros_brote[*contador] = p_id;
    (*contador)++;

    // Buscar vecinos
    int M = s->territorios[t_id].M;
    for (int vecino = 0; vecino < M; vecino++) {
        
        // Verificamos conexión
        double peso = s->territorios[t_id].grafopersonas[p_id][vecino];
        
        if (peso != -1.0) { // Si son amigos
            
            // Verificamos si el vecino está infectado
            int esta_infectado = (s->territorios[t_id].personas[vecino].estado == INFECTADO);
            
            // Verificamos si YA lo visitamos
            int ya_visitado = visitados[vecino];

            // Solo entramos si es infectado y NO ha sido visitado
            if (esta_infectado && !ya_visitado) {
                DFS_buscar_brote(s, t_id, vecino, visitados, miembros_brote, contador);
            }
        }
    }
}

void detectar_brotes(sistema *sistema) {
    printf("\n=== FASE 2: DETECCION DE BROTES (DFS) ===\n");
    int total_brotes_global = 0;

    for (int t = 0; t < sistema->numterritorios; t++) {
        int M = sistema->territorios[t].M;
        if (M == 0) continue;

        // Resetear array de visitados para este territorio
        // Calloc inicializa todo en 0
        int *visitados = (int*)calloc(M, sizeof(int));
        int *miembros_brote = (int*)malloc(M * sizeof(int)); 

        for (int p = 0; p < M; p++) {
            // Solo iniciamos DFS si la persona está infectada y no ha sido visitada
            if (sistema->territorios[t].personas[p].estado == INFECTADO && visitados[p] == 0) {
                
                int contador_miembros = 0;
                DFS_buscar_brote(sistema, t, p, visitados, miembros_brote, &contador_miembros);
                
                total_brotes_global++;
                printf("\n[Territorio %d - %s] >>> FOCO DETECTADO #%d\n", 
                       sistema->territorios[t].territorio_id, 
                       sistema->territorios[t].nombre,
                       total_brotes_global);
                
                printf("   Tamano del cluster: %d personas\n", contador_miembros);
                printf("   Involucrados (ID): ");
                for(int k=0; k<contador_miembros; k++) {
                    // Accedemos al ID real usando el índice guardado
                    int idx_persona = miembros_brote[k];
                    printf("%d ", sistema->territorios[t].personas[idx_persona].id);
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