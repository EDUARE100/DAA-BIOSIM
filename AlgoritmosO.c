#ifndef AlgoritmosO_H
#define AlgoritmosO_H

#include "estructuras.h"

//Recibe como parametros la direccion de memoria del grupo de personas del territorio especifico y el conjunto de personas de ese territorio osea la cantidad
void funcion_quicksort(persona arr[], int n){
    if (n>1) //Si la cantidad ed personas es mayor que 1 entonces haremos quicksort, si solo es 1 persona pues no haremos ningun ordenamiento
    {
        quicksort(arr, 0, n-1);
    }
}

int condition(persona a, persona b){

    //Tiempo_infeccion de menor a mayor ASC
    int ta, tb;

    if (a.tiempo_contagio == -1)
    {
        ta = 9999999; //Si está sano, le damos un valor muy grande para que vaya hasta el final
    }else
    {
        ta = a.tiempo_contagio; //Si no, usamos el tiempo de contagio real
    }
    
    if (b.tiempo_contagio == -1) // -1 porque el tiempo de contagio
    {
        tb = 9999999;
    }else
    {
        tb = b.tiempo_contagio;
    }
    
    
    if (ta < tb) return 1;
    if (ta > tb) return 0;
    
    //if(strcmp(a.nombre, b.nombre) < 0) return 1;
    
    return 0;
}

void swap(persona *a, persona *b){
    persona temp = *a;
    *a = *b;
    *b = temp;
}

int partition(persona arr[], int low, int high){
    persona pivot = arr[high]; //Ultimo elemento del arreglo como el pivote
    int i = (low-1); //Indice del elemento mas pequeño

    for (int j = low; j <= high-1; j++)
    {
        if (condition(arr[j], pivot))
        {
           i++;
           swap(&arr[i], &arr[j]);
        }
        
    }

    swap(&arr[i+1], &arr[high]);
    return (i+1);
}


//QuickSort version Hoare
void quicksort(persona arr[], int low, int high){
    if (low < high)
    {
        int p = partition(arr, low, high);

        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

void quicksort_def(sistema sistema){
    for (int  i = 0; i < sistema.numterritorios; i++)
        {
            int n = sistema.territorios[i].M;
            persona *copia_temp = (persona*)malloc(sizeof(persona) * n);
            // Memcpy, funcion para copiar los datos de un array a otro, destino, origen, tamaño en bytes
            memcpy(copia_temp, sistema.territorios[i].personas, n * sizeof(persona));
            funcion_quicksort(copia_temp, n);

            printf("\nTerritorio: %s)\n", sistema.territorios[i].nombre);
            printf("%-20s | %-15s | %s\n", "Nombre", "Dia Contagio", "Estado Real");
            printf("---------------------------------------------------\n");
            // Aunque parezca que se engloba a la función quicksort en una función con complejidad O(N^2), sigue siendo nlogn la complejidad puesto que el segundo for no está iterando conforme a la cantidad de territorios como el for externo sino que itera conforme a el numero de personas dentro del territorio, haciendo de manera lineal ambos ciclos for puesto que actuan para distintos propositos y no moverse sobre un mismo eje. Complejidad nlogn.
            for (int k = 0; k < n; k++)
            {
                char tiempoStr[20];
                if(copia_temp[k].tiempo_contagio == -1) 
                    sprintf(tiempoStr, "-");
                else 
                    sprintf(tiempoStr, "Dia %d", copia_temp[k].tiempo_contagio);

                printf("%-20s | %-15s | %s\n", copia_temp[k].nombre, tiempoStr, nombres_estados[copia_temp[k].estado]);
            }
            free(copia_temp); //LIberamos la copia del array temporal
        }
}

#endif