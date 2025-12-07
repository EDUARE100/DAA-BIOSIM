
#include "estructuras.h"
#include "AlgoritmosO.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h> 

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

// Inicializar la cola con validación robusta
priorityqueue *crear_pq(int capacidad)
{
    priorityqueue *pq = (priorityqueue *)malloc(sizeof(priorityqueue));
    if(!pq) {
        return NULL;
    }

    pq->nodos = (nodopq *)malloc(capacidad * sizeof(nodopq));
    if(!pq->nodos) {
        free(pq); // Liberar la estructura si falla el array interno
        return NULL;
    }

    pq->tamano = 0;
    pq->capacidad = capacidad;
    
    return pq;
}

// Intercambiar dos nodos en el heap
void swap_nodes(nodopq *a, nodopq *b)
{
    nodopq temp = *a;
    *a = *b;
    *b = temp;
}

// Insertar en Max-Heap
void push_pq(priorityqueue *pq, int id, double prob)
{
    if (!pq || pq->tamano == pq->capacidad) {
        return;
    }

    int i = pq->tamano++;
    pq->nodos[i].nodo_id = id;
    pq->nodos[i].probabilidad = prob;

    // Flotar (Up-Heap)
    while(i > 0) {
        int padre = (i - 1) / 2;
        if(pq->nodos[i].probabilidad > pq->nodos[padre].probabilidad) {
            swap_nodes(&pq->nodos[i], &pq->nodos[padre]);
            i = padre;
        } 
        else {
            break;
        }
    }
}

// Extraer el nodo con mayor probabilidad
nodopq pop_pq(priorityqueue *pq)
{
    // Retornar un nodo inválido/vacío si la cola está vacía
    if (!pq || pq->tamano == 0) {
        nodopq vacio = {-1, -1.0};
        return vacio;
    }

    nodopq maxNode = pq->nodos[0];
    pq->nodos[0] = pq->nodos[--pq->tamano];

    // Hundir (Down-Heap)
    int i = 0;
    while(2 * i + 1 < pq->tamano) {
        int izquierdo = 2 * i + 1;
        int derecho = 2 * i + 2;
        int mayor = izquierdo;

        if(derecho < pq->tamano && pq->nodos[derecho].probabilidad > pq->nodos[izquierdo].probabilidad) {
            mayor = derecho;
        }

        if(pq->nodos[mayor].probabilidad > pq->nodos[i].probabilidad) {
            swap_nodes(&pq->nodos[i], &pq->nodos[mayor]);
            i = mayor;
        }
        else {
            break;
        }
    }

    return maxNode;
}

int es_vacia_pq(priorityqueue *pq)
{
    return (pq == NULL || pq->tamano == 0);
}

void liberar_pq(priorityqueue *pq)
{
    if (pq) {
        if (pq->nodos) free(pq->nodos);
        free(pq);
    }
}

void buscar_ruta_critica(sistema *s, int id_origen, int id_destino) {
    // 1. Validaciones básicas de punteros e índices
    if (!s) return;
    
    // Asegurar que N no exceda el límite real de memoria (MAX_Territorios)
    // Esto previene SIGSEGV si numterritorios tiene basura
    int N = s->numterritorios;
    if (N > MAX_Territorios) N = MAX_Territorios; 

    if (id_origen < 0 || id_origen >= N || id_destino < 0 || id_destino >= N) {
        printf("Error: IDs de territorio inválidos (Rango 0-%d).\n", N-1);
        return;
    }
    
    // Arrays para Dijkstra
    double *probabilidades = (double*)malloc(N * sizeof(double));
    int *padres = (int*)malloc(N * sizeof(int));
    int *visitados = (int*)calloc(N, sizeof(int)); 

    // Verificación de malloc
    if (!probabilidades || !padres || !visitados) {
        printf("Error crítico: No se pudo asignar memoria para Dijkstra.\n");
        if(probabilidades) free(probabilidades);
        if(padres) free(padres);
        if(visitados) free(visitados);
        return;
    }

    // Inicialización
    for (int i = 0; i < N; i++) {
        probabilidades[i] = -1.0; 
        padres[i] = -1;
    }

    probabilidades[id_origen] = 1.0;

    // Crear PQ
    priorityqueue *pq = crear_pq(N * N);
    if (!pq) {
        printf("Error crítico: Fallo al crear Priority Queue.\n");
        free(probabilidades); free(padres); free(visitados);
        return;
    }
    
    push_pq(pq, id_origen, 1.0);

    // Bucle principal
    while (!es_vacia_pq(pq)) {
        nodopq u_node = pop_pq(pq);
        int u = u_node.nodo_id;

        if (u == -1) break; // Seguridad extra

        if (u_node.probabilidad < probabilidades[u]) continue;
        if (u == id_destino) break;

        visitados[u] = 1;

        for (int v = 0; v < N; v++) {
            // Acceso seguro a la matriz
            double peso = s->grafoterritorios[u][v];

            // CORRECCIÓN DE DATOS:
            // Si el peso es > 1.0, asumimos que está en formato porcentual (0-100)
            // y lo normalizamos a (0.0-1.0) para que la matemática de probabilidad funcione.
            // Esto evita que la probabilidad crezca a infinito y cicle.
            if (peso > 1.0) {
                peso /= 100.0;
            }

            if (peso > 0.0 && u != v) {
                double nueva_prob = probabilidades[u] * peso;

                if (nueva_prob > probabilidades[v]) {
                    probabilidades[v] = nueva_prob;
                    padres[v] = u;
                    push_pq(pq, v, nueva_prob);
                }
            }
        }
    }

    // Reporte
    printf("\n=== RUTA CRÍTICA DE CONTAGIO (Dijkstra) ===\n");
    printf("Origen: %s -> Destino: %s\n", s->territorios[id_origen].nombre, s->territorios[id_destino].nombre);

    if (probabilidades[id_destino] == -1.0) {
        printf("Resultado: NO existe ruta de contagio posible.\n");
    } else {
        printf("Probabilidad Total: %.6f (%.2f%%)\n", 
               probabilidades[id_destino], probabilidades[id_destino] * 100.0);
        
        printf("Ruta: ");
        int *ruta = (int*)malloc(N * sizeof(int));
        if (ruta) {
            int count = 0;
            int actual = id_destino;
            // Protección contra ciclos infinitos al reconstruir (count < N)
            while (actual != -1 && count < N) { 
                ruta[count++] = actual;
                actual = padres[actual];
            }
            
            // Si el ciclo se rompió por el límite, avisar (aunque con la normalización no debería pasar)
            if (actual != -1 && count == N) {
               printf(" [Ciclo detectado/Ruta truncada] ");
            }

            for (int i = count - 1; i >= 0; i--) {
                printf("[%s]", s->territorios[ruta[i]].nombre);
                if (i > 0) printf(" -> ");
            }
            printf("\n");
            free(ruta);
        }
    }
    printf("===========================================\n");

    free(probabilidades);
    free(padres);
    free(visitados);
    liberar_pq(pq);
}

/*
 * @brief Función de interfaz para que el usuario seleccione la ruta.
 * Muestra la lista de territorios disponibles y solicita los IDs.
 */
void menu_busqueda_ruta_critica(sistema *s)
{
    if (!s || s->numterritorios < 2) {
        printf("No hay suficientes territorios cargados para calcular rutas.\n");
        return;
    }

    int origen, destino;

    printf("\n--- BUSQUEDA INTERACTIVA DE RUTA CRITICA ---\n");
    printf("Lista de Territorios Disponibles:\n");
    for (int i = 0; i < s->numterritorios && i < MAX_Territorios; i++) {
        printf("  [%d] %s\n", s->territorios[i].territorio_id, s->territorios[i].nombre);
    }
    printf("--------------------------------------------\n");

    printf("Ingrese el ID del territorio de ORIGEN (Foco infeccioso): ");
    if (scanf("%d", &origen) != 1) {
        printf("Entrada inválida.\n");
        while(getchar() != '\n');
        return;
    }

    printf("Ingrese el ID del territorio de DESTINO (Objetivo): ");
    if (scanf("%d", &destino) != 1) {
        printf("Entrada inválida.\n");
        while(getchar() != '\n');
        return;
    }

    buscar_ruta_critica(s, origen, destino);
}

int comparar_por_riesgo_desc(const void *a, const void *b) {
    // Convertimos los punteros genéricos a punteros de struct persona
    const persona *p1 = (const persona *)a;
    const persona *p2 = (const persona *)b;

    // Criterio Voraz: Queremos que el riesgo más alto (p2) vaya primero.
    if (p1->riesgo_inicial < p2->riesgo_inicial) {
        return 1;  // p1 va DESPUÉS de p2
    } else if (p1->riesgo_inicial > p2->riesgo_inicial) {
        return -1; // p1 va ANTES de p2
    } else {
        return 0;  // Riesgos iguales
    }
}

void minimizar_riesgo_greedy(sistema *s, int terr_id, double riesgo_target) {
    
    // Verificaciones básicas
    if (terr_id < 0 || terr_id >= s->numterritorios) {
        printf("Error: ID de territorio invalido.\n");
        return;
    }

    persona *poblacion = s->territorios[terr_id].personas;
    int M = s->territorios[terr_id].M;
    double riesgo_total = 0.0; //Variable que usaremos para la cota máxima, para calcular el límite del riesgo total de cada territorio asi para que cuando se ingrese un riesto objetivo de entrada superior al riesgo total acumulado, se transforme en un target alcanzable

    printf("\n--- SUBPROBLEMA 4: MINIMIZACION DEL RIESGO (O(N log N)) ---\n");
    printf("Territorio a analizar: %d | Objetivo de Riesgo: %.2f\n", terr_id, riesgo_target);

    for(int i = 0; i < M; i++) {
        riesgo_total += poblacion[i].riesgo_inicial; // Suma de todos los riesgos
    }

    // 2. VALIDACIÓN DE ENTRADA (Asegurar que el target es alcanzable)
    if (riesgo_target > riesgo_total) {
        printf("ADVERTENCIA: El objetivo %.2f es inalcanzable.\n", riesgo_target);
        printf("Estableciendo el objetivo al maximo riesgo posible: %.2f\n", riesgo_total);
        riesgo_target = riesgo_total;
    }

    // --- FASE I: Ordenamiento (El costo O(N log N)) ---
    // Ordenamos la población in-place por riesgo_inicial de forma descendente.
    qsort(poblacion, M, sizeof(persona), comparar_por_riesgo_desc); 

    // --- FASE II: Selección Voraz (Recorrido O(M)) ---
    double riesgo_acumulado = 0.0;
    int k_aislados = 0;
    
    // Recorremos la lista ya ordenada, tomando al mas riesgoso en cada paso.
    for (int i = 0; i < M; i++) {
        
        // El algoritmo voraz se detiene en cuanto alcanza la meta
        if (riesgo_acumulado >= riesgo_target) {
            break;
        }
        
        // Decisión Voraz: Sumar el riesgo del individuo actual (el más alto restante)
        riesgo_acumulado += poblacion[i].riesgo_inicial;
        k_aislados++;
        
        printf("Aislando ID %d (Riesgo %.4f) -> Acumulado: %.4f\n", poblacion[i].id, poblacion[i].riesgo_inicial, riesgo_acumulado);
    }

    printf("Se aislaron %d personas (el subconjunto minimo) para contener un riesgo total de %.2f.\n", k_aislados, riesgo_acumulado);
}

trienode *crear_nodo(void)
{
    trienode *nodo = (trienode *)calloc(1, sizeof(trienode));
    if(!nodo) {
        return NULL;
    }

    nodo->cepa_id = -1;

    return nodo;
}

/* * Convierte el double beta a string y lo inserta.
 * Beta típica: 0.000 a 1.000. 
 * L = longitud de la cadena (ej. "0.850" -> L=5)
 */
void insertar_beta(trienode *root, double beta, int cepa_id)
{
    char buffer[20];
    // Formateamos a 3 decimales para estandarizar la agrupación
    sprintf(buffer, "%.3f", beta); 
    
    trienode *crawl = root;
    int len = strlen(buffer);

    for(int i = 0; i < len; i++) {
        int index = (int)buffer[i];
        if(!crawl->hijos[index]) {
            crawl->hijos[index] = crear_nodo();
        }
        crawl = crawl->hijos[index];
    }
    
    crawl->es_final = 1;
    crawl->cepa_id = cepa_id;
    crawl->valor_beta = beta;
}

/* Función recursiva para recolectar cepas en el subárbol */
void recolectar_cluster(trienode *nodo, sistema *s)
{
    if (nodo->es_final) {
        // Recuperamos la info completa usando el ID guardado
        // Nota: Esto asume que el ID coincide con el índice
        // Si cepa_id es el índice en el array:
        if(nodo->cepa_id >= 0 && nodo->cepa_id < s->numcepas) {
            cepa c = s->cepas[nodo->cepa_id]; 
            printf("   -> [Beta: %.3f] %s (ID: %d)\n", 
                   c.beta, c.nombre, c.cepa_id);
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i++) {
        if(nodo->hijos[i]) {
            recolectar_cluster(nodo->hijos[i], s);
        }
    }
}

/*
 * Busca cepas por rango de propagación (prefijo de beta).
 * Ej: buscar_cluster_beta(root, "0.9", s) -> Cluster de alta propagación
 */
void buscar_cluster_beta(trienode *root, char *prefijo_beta, sistema *s)
{
    trienode *crawl = root;
    int len = strlen(prefijo_beta);

    // 1. Navegar hasta el nodo que representa el prefijo (ej. "0.8")
    for(int i = 0; i < len; i++) {
        int index = (int)prefijo_beta[i];
        if(!crawl->hijos[index]) {
            printf("No se encontraron cepas en el rango de propagación '%s'.\n", prefijo_beta);
            return;
        }
        crawl = crawl->hijos[index];
    }

    // 2. Mostrar todas las cepas que cuelgan de este rango
    printf("\n=== Cluster de Propagación: %s... ===\n", prefijo_beta);
    printf("Variantes con tasa de contagio similar:\n");
    recolectar_cluster(crawl, s);
    printf("======================================\n");
}

/* Función wrapper para construir todo el Trie desde el sistema */
trienode* construir_clustering_betas(sistema *s)
{
    trienode *root = crear_nodo();
    for(int i = 0; i < s->numcepas; i++) {
        // Insertamos usando el índice 'i' como referencia rápida
        insertar_beta(root, s->cepas[i].beta, i);
    }
    return root;
}

void mostrar_agrupamiento_automatico(trienode *root, sistema *s)
{
    printf("\n=== CLUSTERING AUTOMÁTICO POR NIVELES DE PROPAGACIÓN ===\n");

    // 1. Acceder a la rama "0" -> "."
    // Esto asume que los betas son 0.XXX
    if(root->hijos['0'] && root->hijos['0']->hijos['.']) {
        trienode *nodo_decimal = root->hijos['0']->hijos['.'];

        // 2. Recorrer dígitos 0-9 para encontrar clusters activos
        for(char digito = '0'; digito <= '9'; digito++) {
            if(nodo_decimal->hijos[(int)digito]) {
                printf("\n[+] Grupo de contagio rango: 0.%c...\n", digito);
                recolectar_cluster(nodo_decimal->hijos[(int)digito], s);
            }
        }
    }
    
    // 3. Caso especial Beta = 1.0 (o cepas que empiezan con 1...)
    if(root->hijos['1']) {
        printf("\n[+] Grupo de contagio TOTAL (1.0 o mayor)\n");
        recolectar_cluster(root->hijos['1'], s);
    }
    printf("\n========================================================\n");
}

int clustering_cepas(sistema *s)
{ 
    if(!s) {
        return -1;
    }

    trienode *raiz_betas = construir_clustering_betas(s);

    mostrar_agrupamiento_automatico(raiz_betas, s);

    return 1;
}