#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "load_data.h"
#include "AlgoritmosO.h"
#include "DFS.h"
#include "Propagacion.h"
#include "hash.h"

// gcc BIOSIM.c load_data.c AlgoritmosO.c DFS.c estructuras.c Propagacion.c hash.c -o BIOSIM

TablaHash bd; //Variable global para el almacenamiento O(1) para ersonas y cepas
void menu(sistema *sistema);



int main(int argc, char const *argv[])
{
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
    printf("--- Iniciando Carga de Datos ---\n");
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
    printf("Indexado completo. Listo para consultas O(1).\n");

    menu(sistema);

    free(sistema); 
    
    return 0;
}

void menu(sistema *sistema){
    
    int opcion = -1;
    while (opcion != 0)
    {
        printf("\n----------------MENU BIOSIM-----------------\n");
        printf("1.- Mostrar Territorios\n");
        printf("2.- Mostrar Personas\n");
        printf("3.- Mostrar Conexiones Territorios\n");
        printf("4.- Mostrar Conexiones Personas\n");
        printf("5.- Mostrar Cepas\n");
        printf("6.- Mostrar Semillas\n");
        printf("7.- Insert\n");
        printf("8.- SUBPROBLEMA 1. Ordenamientos NLOGN\n");
        printf("9.- SUBPROBLEMA 2. Deteccion de Brotes (DFS)\n");
        printf("10. SUBPROBLEMA 3. Calcular la expansion Paso a Paso (>= nlogn)\n");
        printf("11.- SUBPROBLEMA 4.- Minimizacion del riesgo total O(n^2)n\n");
        printf("12.- SUBPROBLEMA 5.- Identificaciion de rutas optimas O((n+m)logn\n");
        printf("13.- SUBPROBLEMA 6.- Hallar la red minima de vacunacion o aislamiento\n");
        printf("14.- SUBPROBLEMA 7.- Agrupar variantes segun similitud, genetica o contagio\n");
        printf("15.- SUBPROBLEMA 8.- Almacenamiento y consultas eficientes O(1)\n");
        printf("0.- Salir\n");
        printf("Ingrese una opcion: ");
        scanf(" %d",&opcion);

        switch (opcion)
        {
        case 1:
            
            mostrar_territorios(sistema);
            system("pause");
            system("cls");
            break;
        case 2:
            mostrar_persona(sistema);
            system("pause");
            system("cls");
            break;
        case 3:
                matriz_conexiones_territorios(sistema);
                system("pause");
                system("cls");
            break;
        case 4:
                matriz_conexiones_personas(sistema);
                system("pause");
                system("cls");
            break;
        case 5:
                mostrar_cepas(sistema);
                system("pause");
                system("cls");
            break;
        case 6:
                mostrar_semillas(sistema);
                system("pause");
                system("cls");
            break;
        case 8:
            system("cls");
            int opcion5 = -1;
            while (opcion5 != 0)
            {
                printf("\nOrdenamientos NLOGN\n");  
                printf("1.- MergeSort\n");
                printf("2.- QuickSort\n");
                printf("3.- HeapSort\n");
                printf("0.- Salir\n");
                printf("Ingrese una opcion: ");
                scanf(" %d", &opcion5);

                switch (opcion5)
                {
                case 1:
                    if(ordenamiento_por_riesgo_desc(sistema) == -1) {
                        puts("Ordenamiento fallido.");
                    }
                    break;

                case 2:
                        quicksort_def(sistema);
                        system("pause");
                        system("cls");
                    break;
                
                default:
                        system("cls");
                    break;
                }
            }
            
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
            }else
            {
                simulacion(sistema, dias);
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
            break;
        case 13:
            break;
        case 14:
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
            break;
        }
        
    }

}

