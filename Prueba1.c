#include <stdio.h>
#include <stdlib.h>
#include "estructuras.h"
#include "load_data.h"

void conexiones_territorios(sistema *sistema, int u_territorio, int v_territorio, float distancia_peso);

void menu(sistema sistema);


int main(int argc, char const *argv[])
{
    sistema sistema;

    inicializar_sistema(&sistema);
    int territorios = leer_territorios_csv(&sistema,"territorios.csv");
    int personas = leer_personas_csv(&sistema,"personas.csv");
    menu(sistema);

    //Nota, imprimir matrices se limito a 5 a propósito pero nada más se cambia en estructuras.c y ya
    //imprimir_matrices(sistema);
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
        printf("5.- SUBPROBLEMA 1. Ordenamientos NLOGN\n");
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
            break;
        case 4:
            break;
        case 5:
            system("cls");
            int opcion5 = -1;
            while (opcion5 != 0)
            {
                printf("Ordenamientos NLOGN\n");  
                printf("1.- MergeSort\n");
                printf("2.- QuickSort\n");
                printf("3.- HeapSort\n");
                printf("4.- En desarrollo\n");
                printf("0.- Salir\n");
                printf("Ingrese una opcion: ");
                scanf(" %d", &opcion5);

                switch (opcion5)
                {
                case 1:

                    break;

                case 2:
                        
                        system("pause");
                        system("cls");
                    break;
                
                default:
                        system("cls");
                    break;
                }
            }
            
            break;
        default:
            break;
        }
        
    }

}




void conexiones_territorios(sistema *sistema, int u_territorio, int v_territorio, float distancia_peso){
    if (u_territorio <0 || v_territorio >= MAX_Territorios || v_territorio < 0 || u_territorio >= MAX_Territorios)
    {
        printf("Error: ID's fueras de los limites de territorios");
        return;
    }

    
    if (u_territorio == v_territorio)
    {
        printf("Error: Un territorio no puede conectarse consigo mismo");
        return;
    }

    //Hacemos de un grafo dirigido osea que ambas conexiones de A - B sean iguales, se dice conexion de territorios tomado como Kilometros
    sistema->grafoterritorios[u_territorio][v_territorio] = distancia_peso;
    sistema->grafoterritorios[v_territorio][u_territorio] = distancia_peso;
      
}