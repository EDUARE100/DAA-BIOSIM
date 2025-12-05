#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "load_data.h"
#include "AlgoritmosO.h"
#include "DFS.h"
#include "Propagacion.h"

// gcc BIOSIM.c load_data.c AlgoritmosO.c DFS.c estructuras.c Propagacion.c -o BIOSIM

void menu(sistema sistema);


int main(int argc, char const *argv[])
{
    sistema sistema;

    inicializar_sistema(&sistema);
    leer_territorios_csv(&sistema,"territorios.csv");
    leer_personas_csv(&sistema,"personas.csv");
    leer_conexiones_territorios(&sistema, "conexiones_t.csv");
    leer_cepas_csv(&sistema,"cepas.csv");
    leer_semillas_csv(&sistema,"semillas.csv");
    generar_red_contactos(&sistema);
    menu(sistema);

    return 0;
}

void menu(sistema sistema){
    
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
        printf("0.- Salir\n");
        printf("Ingrese una opcion: ");
        scanf(" %d",&opcion);

        switch (opcion)
        {
        case 1:
            
            mostrar_territorios(&sistema);
            system("pause");
            system("cls");
            break;
        case 2:
            mostrar_persona(&sistema);
            system("pause");
            system("cls");
            break;
        case 3:
                matriz_conexiones_territorios(&sistema);
                system("pause");
                system("cls");
            break;
        case 4:
                matriz_conexiones_personas(&sistema);
                system("pause");
                system("cls");
            break;
        case 5:
                mostrar_cepas(&sistema);
                system("pause");
                system("cls");
            break;
        case 6:
                mostrar_semillas(&sistema);
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
                    if(ordenamiento_por_riesgo_desc(&sistema) == -1) {
                        puts("Ordenamiento fallido.");
                    }
                    break;

                case 2:
                        quicksort_def(&sistema);
                        system("pause");
                        system("cls");
                    break;
                case 3:
                    if(ordenamiento_por_nombre_asc(&sistema) == -1) {
                        puts("Ordenamiento fallido.");
                    }
                    break;
                
                default:
                        system("cls");
                    break;
                }
            }
            
            break;

        case 9:
            detectar_brotes(&sistema);
            system("pause");
            system("cls");
            break;

        case 10:
            system("cls");
            int dias = 0;
            printf("\nSimulación de Expansion\n");
            printf("Ingrese los dias de simulacion: ");
            scanf(" %d", &dias);

            if(dias <= 0){
                printf("Error. La cantidad de dias debe ser mayor a 0\n");
                return;
            }else
            {
                simulacion(&sistema, dias);
            }
            system("pause");
            system("cls");
            break;
        default:
            break;
        }
        
    }

}
