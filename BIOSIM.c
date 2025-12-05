#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "estructuras.h"
#include "load_data.h"
#include "AlgoritmosO.h"
#include "DFS.h"
#include "Propagacion.h"
#include "hash.h"
#include "Animacion.h"
#include "Prim.h"

// gcc BIOSIM.c load_data.c AlgoritmosO.c DFS.c estructuras.c Propagacion.c hash.c Animacion.c Prim.c -o BIOSIM -lfreeglut -lopengl32 -lglu32
// linux: gcc BIOSIM.c load_data.c AlgoritmosO.c DFS.c estructuras.c Propagacion.c hash.c Animacion.c Prim.c -o BIOSIM -lGL -lGLU -lglut -lm

TablaHash bd; //Variable global para el almacenamiento O(1) para personas y cepas
void menu(sistema *sistema);
void imprimir_encabezado();


int main(int argc, char const *argv[])
{
    /*
        Se asigna malloc principalmente para que los datos sobrevivan, se reserva en el heap, se queda ahí intacta y disponible durante todo el programa hasta que se decida liberar, es para evitar entrar a una funcion que se realice dicha función y cuando se retorne se evita que el sistema desaparezca. Además para asegurar la integridad de la estructura sistema ya que sin ser reservada a memoria, y tener grandes matrices se puede llegar a desbordar la pila
    */
    sistema *sistema = malloc(sizeof(*sistema)); 
    if (sistema == NULL) {
        printf("Error: No se pudo asignar memoria para el sistema.\n");
        return 1;
    }

    // El valor fijo garantiza que la red social y los "valores aleatorios" sean iguales en cada ejecución.
    srand(1234); 
    
    inicializar_sistema(sistema);
    inicializar_hash(&bd); // Inicializa la Tabla Hash (O(1) para empezar)

    // 2. CARGA DE DATOS DESDE CSV
    leer_territorios_csv(sistema, "territorios.csv");
    leer_personas_csv(sistema, "personas.csv");
    leer_conexiones_territorios(sistema, "conexiones_t.csv");
    leer_cepas_csv(sistema, "cepas.csv");
    leer_semillas_csv(sistema, "semillas.csv"); 
    
    // 3. GENERACIÓN DE REDES Y ACCESO RÁPIDO
    generar_red_contactos(sistema); 
    
    // 4. INDEXACIÓN O(1) (SUBPROBLEMA 8: Construcción de la base de datos rápida)
    printf("\nGenerando indices Hash O(1) para consulta...\n");
    // La construcción es O(N) una sola vez, pero habilita el acceso O(1) futuro.
    // A) Indexar Personas
    for(int i = 0; i < sistema->numterritorios; i++) {
        for(int k = 0; k < sistema->territorios[i].M; k++) {
            persona *p = &sistema->territorios[i].personas[k];
            insertar_hash(&bd, p->id, (void*)p, 'P'); 
        }
    }
    // B) Indexar Cepas
    for(int i = 0; i < sistema->numcepas; i++) {
        cepa *c = &sistema->cepas[i];
        insertar_hash(&bd, c->cepa_id, (void*)c, 'C');
    }

    menu(sistema);

    free(sistema); 
    
    return 0;
}

void imprimir_encabezado() {
    printf("\n");
    printf("  ____  ___ ___  ___ ___ __  __ \n");
    printf(" | __ )|_ _/ _ \\/ __|_ _|  \\/  |\n");
    printf(" |  _ \\ | | | | \\__ \\| || |\\/| |\n");
    printf(" | |_) || | |_| |___)| || |  | |\n");
    printf(" |____/|___\\___/|___/___|_|  |_|\n");
    printf("      SISTEMA DE SIMULACION     \n");
    printf("==================================\n");
}

void menu(sistema *sistema){
    int opcion = -1;

    while (opcion != 0)
    {
        // Limpiamos pantalla en cada iteración para que el menú se vea estático
        system("cls"); 
        imprimir_encabezado();

        printf("\n [ MENU PRINCIPAL ]\n");
        printf("  1. Mostrar Territorios\n");
        printf("  2. Mostrar Personas\n");
        printf("  3. Mostrar Conexiones Territorios\n");
        printf("  4. Mostrar Conexiones Personas\n");
        printf("  5. Mostrar Cepas\n");
        printf("  6. Mostrar Semillas\n");
        printf("  7. Insertar nuevo dato\n");
        printf("  8. SUBPROBLEMAS Y ANALISIS (Entrar al submenu)\n"); // Entrada al switch anidado
        
        printf("\n  0. Salir\n");
        printf("\n >> Ingrese una opcion: ");
        scanf(" %d",&opcion);

        switch (opcion)
        {
        case 1:
            mostrar_territorios(sistema);
            system("pause");
            break;
        case 2:
            mostrar_persona(sistema);
            system("pause");
            break;
        case 3:
            matriz_conexiones_territorios(sistema);
            system("pause");
            break;
        case 4:
            matriz_conexiones_personas(sistema);
            system("pause");
            break;
        case 5:
            mostrar_cepas(sistema);
            system("pause");
            break;
        case 6:
            mostrar_semillas(sistema);
            system("pause");
            break;
        case 7:
            printf("Funcion insertar (Placeholder)...\n");
            system("pause");
            break;
            
        // --- AQUI ESTA EL SWITCH ANIDADO ---
        case 8:
            {
                int subOpcion = -1;
                while(subOpcion != 0) {
                    system("cls");
                    imprimir_encabezado();
                    printf("\n [ SUBPROBLEMAS Y ALGORITMOS ]\n");
                    printf("  1. SP 1: Ordenamientos NLOGN\n");
                    printf("  2. SP 2: Deteccion de Brotes (DFS)\n");
                    printf("  3. SP 3: Calcular Expansion Paso a Paso\n");
                    printf("  4. SP 4: Minimizacion del riesgo (Greedy)\n");
                    printf("  5. SP 5: Rutas Optimas (Dijkstra/Prim)\n");
                    printf("  6. SP 6: Red Minima de Vacunacion\n");
                    printf("  7. SP 7: Clustering de Variantes\n");
                    printf("  8. SP 8: Consultas Eficientes Hash O(1)\n");
                    printf("  0. Volver al Menu Principal\n");
                    printf("\n >> Seleccione subproblema: ");
                    scanf(" %d", &subOpcion);

                    switch(subOpcion) {
                        case 1: // SP 1: Ordenamientos
                            system("cls");
                            int optSort = -1;
                            while (optSort != 0) {
                                printf("\n--- ORDENAMIENTOS NLOGN ---\n");  
                                printf("1. MergeSort (Riesgo Desc)\n");
                                printf("2. QuickSort (Defecto)\n");
                                printf("3. HeapSort (Nombre Asc)\n");
                                printf("0. Volver\n");
                                printf("Opcion: ");
                                scanf(" %d", &optSort);
                                switch (optSort) {
                                    case 1:
                                        if(ordenamiento_por_riesgo_desc(sistema) == -1) puts("Fallido.");
                                        else puts("Ordenado por MergeSort.");
                                        system("pause"); break;
                                    case 2:
                                        quicksort_def(sistema);
                                        puts("Ordenado por QuickSort.");
                                        system("pause"); break;
                                    case 3:
                                        if(ordenamiento_por_nombre_asc(sistema) == -1) puts("Fallido.");
                                        else puts("Ordenado por HeapSort.");
                                        system("pause"); break;
                                }
                                if(optSort != 0) system("cls");
                            }
                            break;

                        case 2: // SP 2: DFS
                            detectar_brotes(sistema);
                            system("pause");
                            break;

                        case 3: // SP 3: Simulacion
                            {
                                system("cls");
                                int dias = 0;
                                printf("\n--- SIMULACION DE EXPANSION ---\n");
                                printf("Ingrese los dias de simulacion: ");
                                scanf(" %d", &dias);

                                if(dias <= 0){
                                    printf("Error: Dias deben ser > 0\n");
                                    system("pause");
                                    break;
                                }
                                int modo = 0;
                                printf("1. Reporte Texto (Rapido)\n");
                                printf("2. Animacion Grafica (OpenGL)\n");
                                printf("Opcion: ");
                                scanf("%d", &modo);

                                if (modo == 1) simulacion(sistema, dias);
                                else if (modo == 2) lanzar_visualizacion(sistema, dias);
                                else printf("Opcion invalida.\n");
                                system("pause");
                            }
                            break;

                        case 4: // SP 4: Greedy
                            {
                                system("cls");
                                int terr_id;
                                double riesgo_target;
                                printf("\n--- MINIMIZACION DEL RIESGO (GREEDY) ---\n");
                                printf("ID Territorio: ");
                                scanf(" %d", &terr_id);
                                printf("Riesgo Objetivo (0.0 - 10.0): ");
                                scanf(" %lf", &riesgo_target);
                                minimizar_riesgo_greedy(sistema, terr_id, riesgo_target);
                                system("pause");
                            }
                            break;

                        case 5: // SP 5: Rutas
                            printf("Funcionalidad Rutas Optimas pendiente...\n");
                            system("pause");
                            break;

                        case 6: // SP 6: Red Minima
                            system("cls");
                            printf("\n--- RED DE DISTRIBUCION DE VACUNAS (PRIM) ---\n");
                            
                            if (sistema->numterritorios > 0) {
                            
                            // La matriz es double, pero Prim usa int. Creamos una copia temporal.
                                
                                // 1. Reservar memoria para la matriz temporal de enteros
                                int **grafo_int = malloc(sistema->numterritorios * sizeof(int*));
                                if (grafo_int == NULL) {
                                    printf("Error de memoria.\n");
                                    break;
                                }

                                for (int i = 0; i < sistema->numterritorios; i++) {
                                    grafo_int[i] = malloc(sistema->numterritorios * sizeof(int));
                                    
                                    // 2. Copiar y convertir cada valor (Casting)
                                    for(int j = 0; j < sistema->numterritorios; j++){
                                        // Convertimos el double a int truncando decimales
                                        grafo_int[i][j] = (int)sistema->grafoterritorios[i][j];
                                    }
                                }

                                // 3. Llamar al algoritmo con la matriz compatible
                                primMST(grafo_int, sistema->numterritorios);

                                // 4. Limpieza: Liberar la matriz temporal para no dejar basura en RAM
                                for (int i = 0; i < sistema->numterritorios; i++) {
                                    free(grafo_int[i]);
                                }
                                free(grafo_int);

                            } else {
                                printf("Error: No hay territorios cargados en el sistema.\n");
                            }
                            system("pause");
                            break;
                            break;

                        case 7: // SP 7: Clustering
                            if(clustering_cepas(sistema) == -1) puts("Fallido.");
                            system("pause");
                            break;

                        case 8: // SP 8: Hash
                            system("cls");
                            printf("\n--- CONSULTAS O(1) ---\n");
                            consultar_hash(&bd); 
                            system("pause");
                            break;

                        case 0:
                            // Rompe el while interno y vuelve al menú principal
                            break;
                        
                        default:
                            printf("Opcion invalida en Submenu.\n");
                            system("pause");
                            break;
                    }
                }
            }
            break; // Fin del Case 8 (Submenu)

        case 0:
            printf("Saliendo del sistema...\n");
            break;

        case 9:
            detectar_brotes(sistema);
            system("pause");
            system("cls");
            break;

        case 10:
            system("cls");
            int dias = 0;
            printf("\nSimulacion de Expansion\n");
            printf("Ingrese los dias de simulacion: ");
            scanf(" %d", &dias);

            if(dias <= 0){
                printf("Error. La cantidad de dias debe ser mayor a 0\n");
                continue;
            }
            int opcionx = 0;
            printf("1. Reporte de Texto (Rapido - Genera Historial TXT)\n");
            printf("2. Animacion Grafica (Visual - OpenGL)\n");
            printf("Opcion: ");
            scanf("%d", &opcionx);

            if (opcionx == 1) {
                // Modo Texto: Calcula todo rápido y guarda en TXT
                simulacion(sistema, dias);
            } 
            else if (opcionx == 2) {
                // Modo Gráfico: Abre la ventana y simula paso a paso
                printf("Abriendo ventana grafica...\n");
                lanzar_visualizacion(sistema, dias);
            } 
            else {
                printf("Opcion invalida. Cancelando simulacion.\n");
            }
            
            system("pause");
            system("cls");
            break;
        case 11:
            system("cls");
            int terr_id;
            double riesgo_target;

            printf("\n--- SUBPROBLEMA 4: MINIMIZACION DEL RIESGO ---\n");
            printf("Ingrese ID del Territorio a analizar: ");
            scanf(" %d", &terr_id);
            
            printf("Ingrese Riesgo Objetivo a contener (0.0-10.0 /Riesgos normalizados): ");
            scanf(" %lf", &riesgo_target);

            minimizar_riesgo_greedy(sistema, terr_id, riesgo_target);
            
            system("pause");
            system("cls");
            break;
        case 12:
            menu_busqueda_ruta_critica(sistema);
            break;
        case 13:
            break;
        case 14:
            if(clustering_cepas(sistema) == -1) {
                puts("Ordenamiento fallido.");
            }
            break;
        case 15:
            system("cls");
            // La función espera la dirección de la Tabla Hash
            consultar_hash(&bd); 
            system("pause");
            system("cls");
            break;
            break;
        default:
            printf("Opcion no reconocida.\n");
            system("pause");
            break;
        }
    }
}