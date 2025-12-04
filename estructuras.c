
#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *nombres_estados[] = {
    "Sano",
    "Infectado",
    "Recuperado",
    "Fallecido"
};

void inicializar_sistema(sistema *sistema){
    sistema->numterritorios = 0;
    sistema->numcepas = 0;
    sistema->numsemillas = 0;

    srand(1234);

    //Inicialización grafo de territorios
    for (int i = 0; i < MAX_Territorios; i++)
    {
        for (int j = 0; j < MAX_Territorios; j++)
        {
            sistema->grafoterritorios[i][j] = 0;
        }
    }
    //Inicializar cada territorio. NO ES N^3, sería O(T*N^2)
    for (int t = 0; t < MAX_Territorios; t++)
    {   
        sistema->territorios[t].territorio_id = t;
        strcpy(sistema->territorios[t].nombre, ""); //Asignamos los strings de nombre a vacíos
        sistema->territorios[t].M = 0; //Número de personas en el territorio
        //Inicializacipon del grafo de personas
        for (int p1 = 0; p1 < MAX_Personas_por_territorio; p1++)
        {
            for (int p2 = 0; p2 < MAX_Personas_por_territorio; p2++)
            {
                sistema->territorios[t].grafopersonas[p1][p2] = -1.0;
            }
        }
    }
    
}

void matriz_conexiones_territorios(sistema *sistema){

    printf("\nMatriz de territorios 20x20\n");
    
    for (int i = 0; i < MAX_Territorios; i++)
    {

        if (i < sistema->numterritorios) printf("%4d | ", i); // Etiqueta fila
        else if (i == sistema->numterritorios) printf(" ...\n"); // Separador visual
        
        // Si quieres ver toda la 20x20 aunque esté vacía, usa el bucle completo.
        // Aquí mostramos la matriz completa como pediste:
        if (i >= sistema->numterritorios) continue; // Ocultamos filas vacías para limpieza visual

        for (int j = 0; j < MAX_Territorios; j++)
        {
            if (j >= sistema->numterritorios) continue; // Ocultamos columnas vacías

            // CAMBIO AQUÍ: Si es 0.0 (sin conexión), imprime punto.
            if (sistema->grafoterritorios[i][j] != 0.0)
            {
                printf("%4.1f ", sistema->grafoterritorios[i][j]);
            }
            else
            {
                printf("  .  "); // Espaciado para alinear con %4.1f
            }
        }
        printf("\n");
    }
    printf("\n");
}

void matriz_conexiones_personas(sistema *sistema){
    
    //Por ahora limite la impresión a solo 5 territorios
    for (int t = 0; t < sistema->numterritorios; t++){
        if (sistema->territorios[t].M > 0) {
            printf("Grafo de Personas por territorio (ID: %d - %s):\n", t, sistema->territorios[t].nombre);   
            // Imprimir encabezado de columnas (índices)
            printf("   ");
            for(int k=0; k < sistema->territorios[t].M; k++) printf("%4d ", k);
            printf("\n");
            
            for (int p1 = 0; p1 < sistema->territorios[t].M; p1++)
            {
                printf("%2d ", p1); // Indice fila
                for (int p2 = 0; p2 < sistema->territorios[t].M; p2++)
                {
                    // CAMBIO VISUAL:
                    // Si es -1.0, no existe conexión (imprimimos punto)
                    // Si es >= 0.0, existe conexión (incluso si es 0.00)
                    if(sistema->territorios[t].grafopersonas[p1][p2] != -1.0)
                        printf("%.2f ", sistema->territorios[t].grafopersonas[p1][p2]);
                    else 
                        printf(" .   ");
                }
                printf("\n");
            }
            printf("\n");
            // Solo imprimimos uno de ejemplo, quitar el break si se quieren todos
            break; 
        }
    }
}

void mostrar_persona(sistema *sistema){
    if (sistema->numterritorios > MAX_Territorios)
    {
        printf("Territorio no existente");
        return;
    }
    
    for (int i = 0; i < sistema->numterritorios; i++)
    {
        printf("Territorio: %-15s [ID: %d] [Personas: %d]\n",sistema->territorios[i].nombre, sistema->territorios[i].territorio_id, sistema->territorios[i].M);

        if (sistema->territorios[i].M == 0)
        {
            printf("Sin personas registradas en el territorio\n\n");
        }else{
            for (int j = 0; j < sistema->territorios[i].M; j++)
            {
                printf("[ID: %d] %-20s[Grado: %2d] [Riesgo_init: %.3f] [Estado: %s]\n",sistema->territorios[i].personas[j].id, sistema->territorios[i].personas[j].nombre, sistema->territorios[i].personas[j].grado_inicial, sistema->territorios[i].personas[j].riesgo_inicial, nombres_estados[sistema->territorios[i].personas[j].estado]);
            }
            printf("\n");
        }
        
    }
    printf("\n");
}

void mostrar_territorios(sistema *sistema){
    //Validamos si hay territorios existentes
    if (sistema->numterritorios == 0)
    {
        printf("No hay territorios a imprimir");
        return;
    }

    printf("----------------Territorios agregados-----------\n");
    
    if (sistema->numterritorios == 0)
    {
        printf("No hay territorios agregados");
        return;
    }
    printf("--  #  Nombre Territorio     ID      M  \n");
    for (int i = 0; i < sistema->numterritorios; i++)
    {
        printf("-- [%2d] %-16s (ID: %2d, Personas: %2d)\n",i,sistema->territorios[i].nombre,sistema->territorios[i].territorio_id ,sistema->territorios[i].M);
    }
    printf("------------------------------------------------\n");
}

void mostrar_cepas(sistema *sistema) {
    if (sistema->numcepas == 0) {
        printf("No hay cepas cargadas.\n");
        return;
    }
    printf("\n--- LISTADO DE CEPAS ---\n");
    printf("ID   | Nombre               | Beta  | Letalidad | Gamma (Recup.)\n");
    printf("-----|----------------------|-------|-----------|---------------\n");
    for (int i = 0; i < sistema->numcepas; i++) {
        printf("%-4d  %-20s  %.3f  %.3f      %.3f\n", sistema->cepas[i].cepa_id, sistema->cepas[i].nombre, sistema->cepas[i].beta, sistema->cepas[i].letalidad, sistema->cepas[i].gamma_recuperacion);
    }
    printf("\n");
}

void mostrar_semillas(sistema *sistema) {
    printf("\n--- PACIENTES CERO (SEMILLAS) ---\n");
    if (sistema->numsemillas == 0) {
        printf("No hay semillas cargadas.\n");
        return;
    }

    for (int i = 0; i < sistema->numsemillas; i++) {
        int pid = sistema->semillas[i].persona_id;
        int cid = sistema->semillas[i].cepa_id;
        
        // Buscar el nombre de la cepa para que se vea mejor
        char nombre_cepa[50] = "Desconocida";
        for(int c=0; c < sistema->numcepas; c++){
            if(sistema->cepas[c].cepa_id == cid){
                strcpy(nombre_cepa, sistema->cepas[c].nombre);
                break;
            }
        }

        printf("Semilla #%d: Persona ID %d | Cepa: %s (ID %d) | Dia 0: %d\n", 
               i+1, pid, nombre_cepa, cid, sistema->semillas[i].t0);
    }
}


void addpersonas(sistema *sistema, int territorio_id, int id, const char *nombre, int grado_inicial, double riesgo_inicial){
    //Validamos si existe el territorio
    if (territorio_id >= sistema->numterritorios || territorio_id < 0)
    {
        printf("Territorio no existente");
        return;
    }
    //Validación que no esté lleno el array basicamente, si se quieren insertar personas despues pues se sube la cantidad de personas estaticamente en el define
    //M es el numPersonas de ese territorio en especifico por eso le indicamos el indice del territorio al que pertenece por el territorio_id
    if (sistema->territorios[territorio_id].M >= MAX_Personas_por_territorio)
    {
        printf("Error. No puede insertar más personas. Capacidad máxima alcanzada (25 max)");
        return;
    }
    int indice = sistema->territorios[territorio_id].M; //Guardamos el indice de la persona a ingresar en el territorio que se especifico
    sistema->territorios[territorio_id].personas[indice].id = id;
    strcpy(sistema->territorios[territorio_id].personas[indice].nombre, nombre);
    sistema->territorios[territorio_id].personas[indice].grado_inicial = grado_inicial;
    sistema->territorios[territorio_id].personas[indice].riesgo_inicial = riesgo_inicial;
    sistema->territorios[territorio_id].personas[indice].estado = SANO;

    sistema->territorios[territorio_id].M++; //Se incrementa M osea numPersona por cada inserción que se haga en el indice del territorio específico, esto evitando que se acumulen todos las personas en un mismo contador.
}


int addterritorio(sistema *sistema, const char *nombre){
    if (sistema->numterritorios >= MAX_Territorios)
    {
        printf("No se pueden agregar más territorios al máximo permitido MAX: %d\n", MAX_Territorios);
        return -1;
    }

    if (nombre == NULL || strlen(nombre) == 0)
    {
        printf("El nombre del territorio no puede estar vacío");
        return -1; //Retornarmos -1 en todos los casos para evitar poner 0 y pensar que se añadió algún dato no requerido
    }
    
    if (strlen(nombre) >= 15)
    {
        printf("Nombre muy largo para este campo (Máximo de 14 caracteres)");
        return -1;
    }
    

    int indice = sistema->numterritorios; //Hacemos que nuestro indice del array sea lo mismo que el valor de num de territorios

    sistema->territorios[indice].territorio_id = indice; //Asignamos el valor del indice actual de nuestro array al indice que tuvimos al declarar la variable indice
    strcpy(sistema->territorios[indice].nombre, nombre); //Copiamos el nombre que le pasariamos por parametro desde el main y se lo asignamos al nombre de la posicion osea del indice actual
    sistema->territorios[indice].M = 0; //Se empieza sin personas desde un inicio, estó se irá actualizando

    sistema->numterritorios++; //Esto es lo que hará que se actualice el indice y no se nos sobreescriba en la misma posición una y otra vez

    return indice; //Retornamos el índice actual
    
}

void conexiones_territorios(sistema *sistema, int u_territorio, int v_territorio, float distancia_peso){
    if (u_territorio <0 || v_territorio >= MAX_Territorios || v_territorio < 0 || u_territorio >= MAX_Territorios)
    {
        printf("Error : ID's fueras de los limites de territorios");
        return;
    }

    
    if (u_territorio == v_territorio)
    {
        printf("Error: Un territorio no puede conectarse consigo mismo");
        return;
    }

    if (distancia_peso < 0.0 )
        printf("Distancia no puede ser menor a 0");
    

    //Hacemos de un grafo dirigido osea que ambas conexiones de A - B sean iguales, se dice conexion de territorios tomado como Kilometros
    sistema->grafoterritorios[u_territorio][v_territorio] = distancia_peso;
    sistema->grafoterritorios[v_territorio][u_territorio] = distancia_peso;
      
}

int obtener_id(sistema *sistema, int territorio_id, int id_buscado){
    if (territorio_id < 0 || territorio_id >= sistema->numterritorios) return -1;
    for (int i = 0; i < sistema->territorios[territorio_id].M; i++)
    {
        if (sistema->territorios[territorio_id].personas[i].id == id_buscado) return 1;
    }
    return -1;
}

// Funcion aux para conectar a dos personas (indices internos)
void conectar_personas_xindice(sistema *sistema, int territorio_id, int id1, int id2, double prob_contagio){
    if (id1 == id2) return;
    
    sistema->territorios[territorio_id].grafopersonas[id1][id2] = prob_contagio;
    sistema->territorios[territorio_id].grafopersonas[id2][id1] = prob_contagio;
    
}

// Verifica cuantas conexiones tiene actualmente una persona
int contador_conexiones(sistema *sistema, int territorio_id, int id_persona){
    int contador = 0;
    for (int i = 0; i < sistema->territorios[territorio_id].M; i++)
    {
        if (sistema->territorios[territorio_id].grafopersonas[id_persona][i] > 0)
        {
            contador++;
        }
        
    }
    return contador;
}

/*Matematicamente */

void generar_red_contactos(sistema *sistema){
    for (int i = 0; i < sistema->numterritorios; i++)
    {
        int num_personas = sistema->territorios[i].M;
        if (num_personas < 2) continue;

        for (int j = 0; j < num_personas; j++)
        {
            int grado_objetivo = sistema->territorios[i].personas[j].grado_inicial;
            int intentos = 0;

            //Contamos conexiones antes de entrar al bucle de intentos de conexion por grado inicial
            int conexiones_actuales = contador_conexiones(sistema,i,j);
            //Se puso como condicion intentos < 1000 como medida de seguridad para que no se quede en un bucle infinito
            while (conexiones_actuales < grado_objetivo && intentos < 1000)
            {
                int candidato = rand() % num_personas;

                if (candidato != j && sistema->territorios[i].grafopersonas[j][candidato] == -1.0)
                {
                    //Se genera una probabilidad aleatoria entre 0.0 y 1.0 para la prob_contagio de cada conexion
                    double prob_contagio = (double)rand() / RAND_MAX;
                    conectar_personas_xindice(sistema,i,j,candidato,prob_contagio);
                    conexiones_actuales++;
                }
                intentos++;
            }
        }
        
    }
    printf("Red de contactos generada correctamente\n");
}