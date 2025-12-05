
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_data.h"

#define MAX_line 1000 //Definimos la longitud máxima de cada línea

int leer_territorios_csv(sistema *sistema, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error al abrir %s\n",filename);
        return 0;
    }

    char line[MAX_line];
    int num_linea = 0;
    int total_personas = 0;

    // Instrucción para saltar el encabezado
    fgets(line,MAX_line,file);

    while (fgets(line,MAX_line,file))
    {
        num_linea++;

        //Creación de variables locales temporales, son la conexión o puente
        int territorio_id;
        char nombre[15];
        int M;

        if (sscanf(line, "%d,%[^,],%d", &territorio_id, nombre, &M) == 3)
        {
            if (sistema->numterritorios >= MAX_Territorios)
            {
                printf("Limite de territorios alcanzado\n");
                break;
            }
            sistema->territorios[territorio_id].territorio_id = territorio_id;
            strcpy(sistema->territorios[territorio_id].nombre, nombre);
            sistema->territorios[territorio_id].M = 0;
            sistema->numterritorios++;
        }else
        {
            printf("Error en linea %d de %s\n",num_linea,filename);
        }
        
        
    }
    fclose(file);
    printf("%d Territorios cargados\n", sistema->numterritorios);
    return 1;
}

int leer_personas_csv(sistema *sistema, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error al abrir el archivo %s",filename);
        return 0;
    }
    
    char line[MAX_line];
    int num_linea = 0;
    int total_personas = 0;

    fgets(line,MAX_line,file);

    while(fgets(line,MAX_line,file)){

        num_linea++;

        int id;
        int territorio_id;
        char nombre[50];
        int grado_inicial;
        double riesgo_inicial;
        int infectado;

        if (sscanf(line, "%d,%[^,],%d,%d,%lf,%d", &id, nombre, &territorio_id, &grado_inicial, &riesgo_inicial, &infectado)==5){
            
            if (territorio_id >= sistema->numterritorios)
            {
                printf("Número de territorio no existente\n");
                continue;
            }
            
            
            if (sistema->territorios[territorio_id].M >= MAX_Personas_por_territorio)
            {
                printf("Territorio %d lleno\n",territorio_id);
                continue;
            }
            
            int idx = sistema->territorios[territorio_id].M;

            sistema->territorios[territorio_id].personas[idx].id = id;
            strcpy(sistema->territorios[territorio_id].personas[idx].nombre, nombre);
            sistema->territorios[territorio_id].personas[idx].territorio_id = territorio_id;
            sistema->territorios[territorio_id].personas[idx].grado_inicial = grado_inicial;
            sistema->territorios[territorio_id].personas[idx].riesgo_inicial = riesgo_inicial;
            sistema->territorios[territorio_id].personas[idx].estado = SANO;
            sistema->territorios[territorio_id].personas[idx].tiempo_contagio = 0;
            sistema->territorios[territorio_id].personas[idx].cepa_id = -1;

            sistema->territorios[territorio_id].M++;
            total_personas++;
        }else{
            printf("Error en linea %d\n",num_linea);
        }

    }

    fclose(file);
    printf("%d Personas cargadas\n",total_personas);
    return 1;

}

int leer_conexiones_territorios(sistema *sistema, const char *filename){
    FILE *file = fopen(filename,"r");

    if (!file)
    {
        printf("Error al abrir el archivo %s",filename);
        return 0;
    }

    char line[MAX_line];
    int num_linea = 0;
    int total_conexiones = 0;

    fgets(line,MAX_line, file);

    while (fgets(line,MAX_line,file))
    {
        num_linea++;
        int u_territorio, v_territorio;
        float peso_proximidad;

        if (sscanf(line, "%d,%d,%f", &u_territorio, &v_territorio, &peso_proximidad) == 3)
        {
            
            if (u_territorio >= sistema->numterritorios || v_territorio >= sistema->numterritorios)
            {
                printf("\nTerritorio inválido en línea %d",num_linea);
                continue;
            }
            
            if (u_territorio == v_territorio)
            {
                printf("\nNo se puede conectar un territorio cosigo mismo linea: %d",num_linea);
                continue;
            }
            
            //Hacemos del grafo NO dirigido asignadole el peso de proximidad a ambos nodos en su grado de salida
            sistema->grafoterritorios[u_territorio][v_territorio] = peso_proximidad;
            sistema->grafoterritorios[v_territorio][u_territorio] = peso_proximidad;
            total_conexiones++;

        }else{
            printf("\nError en lineea %d",num_linea);
        }
        
    }
    fclose(file);
        printf("%d Conexiones entre territorios cargadas",total_conexiones);
        if (total_conexiones < 30)
        {
           printf("\nFaltan %d conexiones para cumplir las 30 conexiones", 30 - total_conexiones);
        }
    return 1;

}

int leer_cepas_csv(sistema *sistema, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error al abrir %s\n", filename);
        return 0;
    }

    char line[MAX_line];
    int num_linea = 0;
    int total_cepas = 0;

    // Saltar encabezado
    fgets(line, MAX_line, file);

    while (fgets(line, MAX_line, file))
    {
        num_linea++;
        
        // Variables temporales
        int id;
        char nombre[50];
        double beta, letalidad, gamma;

        if (sscanf(line, "%d,%[^,],%lf,%lf,%lf", &id, nombre, &beta, &letalidad, &gamma) == 5)
        {
            if (sistema->numcepas >= MAX_cepas)
            {
                printf("Límite de cepas alcanzado (MAX: %d)\n", MAX_cepas);
                break;
            }

            int idx = sistema->numcepas;
            sistema->cepas[idx].cepa_id = id;
            strcpy(sistema->cepas[idx].nombre, nombre);
            sistema->cepas[idx].beta = beta;
            sistema->cepas[idx].letalidad = letalidad;
            sistema->cepas[idx].gamma_recuperacion = gamma;

            sistema->numcepas++;
            total_cepas++;
        }
    }

    fclose(file);
    printf("\n%d Cepas cargadas exitosamente.\n", total_cepas);
    return total_cepas;
}

int leer_semillas_csv(sistema *sistema, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error al abrir %s\n", filename);
        return 0;
    }

    char line[MAX_line];
    int total_semillas = 0;

    // Saltar encabezado
    fgets(line, MAX_line, file);

    while (fgets(line, MAX_line, file)) {
        int persona_id, t0, cepa_id;

        if (sscanf(line, "%d,%d,%d", &persona_id, &t0, &cepa_id) == 3) {
            
            if (sistema->numsemillas >= MAX_semillas) {
                printf("Límite de semillas alcanzado (MAX: %d)\n", MAX_semillas);
                break;
            }

            // Validamos que la cepa exista
            int cepa_existe = 0;
            for(int c=0; c < sistema->numcepas; c++){
                if(sistema->cepas[c].cepa_id == cepa_id) {
                    cepa_existe = 1; 
                    break;
                }
            }
            if(!cepa_existe){
                printf("Error: Cepa ID %d no encontrada para semilla persona %d\n", cepa_id, persona_id);
                continue;
            }

            //Se busca a la persona en TODOS los territorios para infectarla
            int persona_encontrada = 0;
            for (int t = 0; t < sistema->numterritorios; t++) {
                for (int p = 0; p < sistema->territorios[t].M; p++) {
                    if (sistema->territorios[t].personas[p].id == persona_id) {
                        
                        // ¡PACIENTE CERO ENCONTRADO! -> LO INFECTAMOS
                        sistema->territorios[t].personas[p].estado = INFECTADO;
                        sistema->territorios[t].personas[p].tiempo_contagio = t0; 
                        sistema->territorios[t].personas[p].cepa_id = cepa_id;
                        
                        persona_encontrada = 1;
                        break; 
                    }
                }
                if (persona_encontrada) break;
            }

            if (!persona_encontrada) {
                printf("Advertencia: Persona ID %d no encontrada en ningun territorio.\n", persona_id);
                continue;
            }

            // 3. Guardar en el registro de semillas del sistema
            int idx = sistema->numsemillas;
            sistema->semillas[idx].persona_id = persona_id;
            sistema->semillas[idx].t0 = t0;
            sistema->semillas[idx].cepa_id = cepa_id;
            
            sistema->numsemillas++;
            total_semillas++;
        }
    }

    fclose(file);
    printf("%d Semillas cargadas e infectadas exitosamente.\n", total_semillas);
    return total_semillas;
}