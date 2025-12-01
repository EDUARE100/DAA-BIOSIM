#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "load_data.h"

#define MAX_line 1000 //Definimos la logintud máxima de cada línea

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
            sistema->territorios[territorio_id].personas[idx].tiempo_contagio = -1;

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