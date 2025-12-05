
#include "estructuras.h"
#include "AlgoritmosO.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * @brief Genera e imprime un reporte de personas ordenado por 'riesgo_inicial'
 *        de forma descendente, territorio por territorio.
 *
 * NOTA: Esta función es NO DESTRUCTIVA, ya que opera sobre una copia temporal
 * de los datos de cada territorio.
 *
 * @param s     Puntero a la estructura del sistema completo.
 * @return int  Retorna 1 si el reporte se generó con éxito, -1 en caso de error
 * (ej. error de asignación de memoria).
 */
int ordenamiento_por_riesgo_desc(sistema *s)
{
    // Verificación de puntero y existencia de territorios.
    if(!s || s->numterritorios == 0) {
        return -1;
    }

    // Imprimir encabezado general del reporte.
    puts("\n======================================================");
    puts("   REPORTE DE PERSONAS ORDENADO POR RIESGO INICIAL   ");
    puts("======================================================");
    puts("Formato: Descendiente");

    // Itera sobre cada territorio en el sistema.
    for(int i = 0; i < s->numterritorios; i++) {
        
        int n = s->territorios[i].M;

        /* Si no hay personas, salta el territorio */
        if(n == 0) {
            printf("\nTerritorio: %s\n", s->territorios[i].nombre);
            printf("(Sin personas)\n");
            continue;
        }

        // 1. Asignación y verificación de memoria para la copia temporal.
        persona *copy = (persona *)malloc(n * sizeof(persona));
        if(!copy) {
            // Falla en la asignación: retorna error.
            return -1;
        }

        // 2. Copiar los datos del array original al array temporal
        memcpy(copy, s->territorios[i].personas, n * sizeof(persona));

        // 3. Ordenamiento de la copia: se aplica Merge Sort
        merge_sort(copy, 0, n - 1);

        // 4. Se imprime el reporte del territorio ordenado
        printf("\nTerritorio: %s\n", s->territorios[i].nombre);
        printf("%-20s | %-15s | %-15s\n", "Nombre", "Riesgo Inicial", "Estado Actual");
        printf("------------------------------------------------------\n");

        for(int j = 0; j < n; j++) {
            
            char riesgo_str[10];
            // Formatear el valor double de riesgo a una cadena para alineación.
            sprintf(riesgo_str, "%.3f", copy[j].riesgo_inicial);

            printf("%-20s | %-15s | %-15s\n",
                    copy[j].nombre,
                    riesgo_str,
                    // Acceder al array global de estados.
                    nombres_estados[copy[j].estado]
            );
        }

        // 5. Liberación de memoria
        free(copy);
    }

    return 1;
}

/*
 * @brief Implementa el paso de fusión del algoritmo Merge sort.
 *        Esta función toma dos sub-arrays adyacentes (left...center y center + 1...right)
 *        y los fusiona en un solo array ordenado en el segmento 'p[left...right]'.
 *        El ordenamiento se realiza de forma DESCENDENTE basado en 'riesgo_inicial'.
 *
 * @param p      Puntero al array de estructuras 'persona' que contiene los datos.
 *               Es el array que se modificará al final de la fusión.
 * @param left   Índice inicial del primer sub-array.
 * @param center Índice final del primer sub-array.
 * @param right  Índice final del segundo sub-array.
 * @return void
 */
void merge(persona *p, int left, int center, int right)
{
    // Calcula el número de elementos en el segmento a fusionar.
    int n = right - left + 1;
    // Asigna memoria dinámica para el array temporal (copia auxiliar).
    persona *temp = (persona *)malloc(n * sizeof(persona));
    // Verificación de asignación de memoria.
    if(!temp) {
        return;
    }
    
    int i = left, j = center + 1, x = 0; // i: índice izquierdo, j: índice derecho, x: índice temporal
    
    // Fase 1: Mezclar los dos sub-arrays
    // Itera mientras queden elementos en ambas mitades.
    while(i <= center && j <= right) {
        // [COMPARACIÓN CLAVE]: Ordenamiento DESCENDENTE (mayor riesgo va primero).
        if(p[i].riesgo_inicial > p[j].riesgo_inicial) {
            temp[x++] = p[i++]; // Copia el elemento de mayor riesgo (el izquierdo).
        } else {
            temp[x++] = p[j++]; // Copia el elemento de mayor riesgo (el derecho o igual).
        }     
    }

    // Fase 2: Vaciar los elementos restantes de la sub-lista izquierda
    while(i <= center) {
        temp[x++] = p[i++];
    }
    
    // Fase 3: Vaciar los elementos restantes de la sub-lista derecha
    while(j <= right) {
        temp[x++] = p[j++];
    }
    
    // Fase 4: Copiar los elementos ordenados del array temporal de vuelta al array original 'p'
    for(x = 0, i = left; i <= right; i++, x++) {
        p[i] = temp[x]; 
    }
    
    // Liberar la memoria asignada al array temporal para evitar fugas.
    free(temp);
}

/*
 * @brief Implementa el algoritmo de ordenamiento Merge Sort de forma recursiva.
 *        Divide el array de personas en mitades hasta que cada sub-array contiene
 *        un solo elemento. Luego llama a 'merge' para fusionar las mitades de forma
 *        ordenada.
 *
 * @param p     Puntero al array de estructuras 'persona' que se desea ordenar.
 * @param left  Índice de inicio del segmento del array.
 * @param right Índice de fin del segmento del array.
 * @return void
 */
void merge_sort(persona *p, int left, int right)
{
    // Caso base: Si el segmento tiene 0 o 1 elemento, se considera ordenado.
    if(left >= right) {
        return;
    }

    // Calcular el punto central para dividir el array.
    int center = (left + right) / 2;

    // 1. Divide: Llamada recursiva a la mitad izquierda.
    merge_sort(p, left, center);

    // 2. Divide: Llamada recursiva a la mitad derecha.
    merge_sort(p, center + 1, right);

    // 3. Combina: Fusión y ordenamiento de las dos mitades.
    merge(p, left, center, right);
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


//QuickSort version Lomuto
void quicksort(persona arr[], int low, int high){
    if (low < high)
    {
        int p = partition(arr, low, high);

        quicksort(arr, low, p - 1);
        quicksort(arr, p + 1, high);
    }
}

//Recibe como parametros la direccion de memoria del grupo de personas del territorio especifico y el conjunto de personas de ese territorio osea la cantidad
void funcion_quicksort(persona arr[], int n){
    if (n>1) //Si la cantidad ed personas es mayor que 1 entonces haremos quicksort, si solo es 1 persona pues no haremos ningun ordenamiento
    {
        quicksort(arr, 0, n-1);
    }
}

void quicksort_def(sistema *sistema){
    for (int  i = 0; i < sistema->numterritorios; i++)
        {
            int n = sistema->territorios[i].M;
            persona *copia_temp = (persona*)malloc(sizeof(persona) * n);
            // Memcpy, funcion para copiar los datos de un array a otro, destino, origen, tamaño en bytes
            memcpy(copia_temp, sistema->territorios[i].personas, n * sizeof(persona));
            funcion_quicksort(copia_temp, n);

            printf("\nTerritorio: %s)\n", sistema->territorios[i].nombre);
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

void free_heap(heap *h)
{
    if(h->p) {
        free(h->p);
    }
}

void init_heap(heap *h, int capacidad)
{
    if(!h) {
        return;
    }

    h->p = (persona *) malloc(capacidad * sizeof(persona));
    if(!h->p) {
        return;
    }

    h->n = 0;
    h->capacidad = capacidad;
}

void push_min_heap(heap *h, persona dato)
{
    if(!h || h->n == h->capacidad) {
        return;
    }

    //primer paso: insertar al final
    int i = h->n;
    h->p[i] = dato;
    h->n++;
  
    //flotar
    while (i > 0 && strcmp(h->p[i].nombre, h->p[(i - 1) / 2].nombre) < 0) {
        swap(&h->p[i], &h->p[(i - 1) / 2]);
        i = (i - 1) / 2;
    }  
}

persona pop_min_heap(heap *h)
{
    persona vacia = {0};
    if (h->n <= 0) return vacia;

    //guardar el dato a retornar
    persona temp = h->p[0];
    //hacer que el ultimo elemento sea el primero
    h->p[0] = h->p[h->n - 1];
    h->n--;
  
    int i = 0;
    while (2 * i + 1 < h->n) { // Mientras tenga al menos hijo izquierdo
        
        int hijo_menor = 2 * i + 1; // Asumimos izquierdo es menor
        int hijo_derecho = 2 * i + 2;

        // Si existe hijo derecho y es AUN MENOR que el izquierdo
        if (hijo_derecho < h->n && strcmp(h->p[hijo_derecho].nombre, h->p[hijo_menor].nombre) < 0) {
            hijo_menor = hijo_derecho;
        }

        // Si el padre es mayor que el hijo menor, hundimos
        if (strcmp(h->p[i].nombre, h->p[hijo_menor].nombre) > 0) {
            swap(&h->p[i], &h->p[hijo_menor]);
            i = hijo_menor;
        } else {
            break; // Ya está en orden
        }
    }
   
  return temp;
}

int ordenamiento_por_nombre_asc(sistema *s)
{
    if(!s || s->numterritorios == 0) {
        return -1;
    }

    puts("\n======================================================");
    puts("   REPORTE DE PERSONAS POR NOMBRE (A-Z)   ");
    puts("======================================================");

    for(int i = 0; i < s->numterritorios; i++) {
        
        int M = s->territorios[i].M;

        /* Si no hay personas, salta el territorio */
        if(M == 0) {
            printf("\nTerritorio: %s\n", s->territorios[i].nombre);
            printf("(Sin personas)\n");
            continue;
        }

        heap h;
        init_heap(&h, M);

        for(int k = 0; k < M; k++) {
            push_min_heap(&h, s->territorios[i].personas[k]);
        }

        printf("\nTerritorio: %s\n", s->territorios[i].nombre);
        printf("%-20s | %-10s | %-15s\n", "Nombre", "ID", "Estado Actual");
        printf("------------------------------------------------------\n");

        while(h.n > 0) {
            persona p = pop_min_heap(&h);

            printf("%-20s | %-10d | %-15s\n",
                    p.nombre,
                    p.id,
                    // Acceder al array global de estados.
                    nombres_estados[p.estado]
            );
        }
        
        free_heap(&h);
    }
    
    return 1;
}