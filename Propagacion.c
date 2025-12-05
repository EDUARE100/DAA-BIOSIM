#ifndef Prop_H
#define Prop_H

#include "estructuras.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


//Complejidad O(N) debido a que iteramos sobre numcepas aunque numcepas ya está limitado a 50 desde un principio podria ser O(1) ya que pues no se indica que se puedan agregar más o menos cepas al código

/* Por que es O(n) y no de complejidad mayor?, */

int indice_cepa(sistema *sistema, int cepa_id){
    for (int i = 0; i < sistema->numcepas; i++)
    {
        if(sistema->cepas[i].cepa_id == cepa_id) return 1;
    }
    return -1;
}

void escribir_reporte(FILE *fp, int id_territorio, int *inf, int n_inf, int *rec, int n_rec, int *fal, int n_fal) {
    
    // Imprimimos encabezado del territorio en Pantalla y Archivo
    printf(" [Territorio %d]:\n", id_territorio);
    fprintf(fp, " [Territorio %d]:\n", id_territorio);

    // 1. Reportar Nuevos Infectados
    if (n_inf > 0) {
        printf("   (+) Infectados: ");
        fprintf(fp, "   (+) Infectados: ");
        for(int x = 0; x < n_inf; x++) {
            printf("%d ", inf[x]);
            fprintf(fp, "%d ", inf[x]);
        }
        printf("\n"); fprintf(fp, "\n");
    }

    // 2. Reportar Recuperados
    if (n_rec > 0) {
        printf("   (R) Recuperados: ");
        fprintf(fp, "   (R) Recuperados: ");
        for(int x = 0; x < n_rec; x++) {
            printf("%d ", rec[x]);
            fprintf(fp, "%d ", rec[x]);
        }
        printf("\n"); fprintf(fp, "\n");
    }

    // 3. Reportar Fallecidos
    if (n_fal > 0) {
        printf("   (X) Fallecidos: ");
        fprintf(fp, "   (X) Fallecidos: ");
        for(int x = 0; x < n_fal; x++) {
            printf("%d ", fal[x]);
            fprintf(fp, "%d ", fal[x]);
        }
        printf("\n"); fprintf(fp, "\n");
    }
}

void simulacion(sistema *sistema, int dias){

    FILE *fp = fopen("historial_simulacion.txt", "w");
    if (fp == NULL) {
        printf("Error fatal: No se pudo crear el archivo de historial.\n");
        return;
    }
    fprintf(fp,"Historial BIOSIM\n");
    fprintf(fp,"Duracion: %d dias\n", dias);

    for (int i = 1; i <= dias; i++)
    {
        fprintf(fp,"\nDIA %d\n",i);
        int nuevos_infectados = 0;
        int nuevos_recuperados = 0;
        int nuevos_fallecidos = 0;

        int cambios = 0;

        //Iteramos por numero de territorios
        for (int j = 0; j < sistema->numterritorios; j++)
        {
            int M = sistema->territorios[j].M;
            if(M==0)continue;

            //Guardamos en copias temporales los cambios del dia sin afectar la iteración actual
            estado *estados_futuros = (estado*)malloc(M * sizeof(estado));
            int *cepas_futuras = (int*)malloc(M * sizeof(int));
            
            //Necesitamos guardar los ID's de los afectados de cada Día
            int *list_nuevos_infectados = (int*)malloc(M*sizeof(int));
            int cant_inf = 0;

            int *list_nuevos_recuperados = (int*)malloc(M*sizeof(int));
            int cant_rec = 0;

            int *list_nuevos_fallecidos = (int*)malloc(M*sizeof(int));
            int cant_fal = 0;

            //Inicializamos futuro con presente. M está definido como 30 asi que es un O(1) este for
            for (int k = 0; k < M; k++)
            {
                estados_futuros[k] = sistema->territorios[j].personas[k].estado;
                cepas_futuras[k] = sistema->territorios[j].personas[k].cepa_id;
            }
            //Calcular contagios y evolución
            for (int k = 0; k < M; k++)
            {
                persona *individuo = &sistema->territorios[j].personas[k];

                //Si solamente el individuo está infectado pueden contagiar o cambiar de estado
                if (individuo->estado == INFECTADO)
                {
                    int id_cepa = indice_cepa(sistema,individuo->cepa_id);
                    if(id_cepa == -1) continue; //Medida de seguridad para


                    double beta = sistema->cepas[id_cepa].beta; //Tasa de contagio
                    double letalidad = sistema->cepas[id_cepa].letalidad;
                    double gamma = sistema->cepas[id_cepa].gamma_recuperacion;

                    for (int vecino = 0; vecino < M; vecino++)
                    {
                        /*
                            Condiciones a contagiar 
                            1.- Que exista la conexión entre las personas osea que != -1
                            2.- Que el vecino esté SANO
                            3.- Que el vecino siga SANO en el futuro, para no contagiarlo 2 veces el mismo dia
                        
                        */

                        double prob_contagio = sistema->territorios[j].grafopersonas[k][vecino];

                        if (prob_contagio != -1.0 && sistema->territorios[j].personas[vecino].estado == SANO && estados_futuros[vecino] == SANO)
                        {
                            
                            double riesgo_vecino = sistema->territorios[j].personas[vecino].riesgo_inicial;
                            
                            //Formula de contagio
                            double prob_final = beta * prob_contagio * riesgo_vecino;
                            /* 
                                Se rige de la probabilidad final de contagio siendo el producto de la probabilidad beta de la cepa por la probabilidad de contagio que es el peso de la conexión entre cada persona del gráfico estático por el riesgo inicial 
                            */
                            //Asignamos el tope de contagio normalizado para que no rebase el 100%
                            if(prob_final > 1.0) prob_final = 1.0;

                            double prob_aleatoria_contagio = (double)rand() / RAND_MAX;
                            
                            //Si la prob_final que es la formula de contagio se dice que es mayor que la probabilidad aleatoria de contagio entonces se infecta el individuo
                            if (prob_aleatoria_contagio < prob_final)
                            {
                                estados_futuros[vecino] = INFECTADO;
                                cepas_futuras[vecino] = individuo->cepa_id; //Hereda la cepa
                                nuevos_infectados++;
                            }
                            
                            
                        }
                        
                    }
                    
                        individuo->tiempo_contagio++;

                        double prob_aleatoria_recuperacion = (double)rand() / RAND_MAX;

                        if (prob_aleatoria_recuperacion < gamma)
                        {
                            double prob_aleatoria_muerte = (double)rand() / RAND_MAX;

                            if (prob_aleatoria_muerte < letalidad)
                            {
                                estados_futuros[k] = FALLECIDO;
                                nuevos_fallecidos++;
                            }
                            else
                            {
                                estados_futuros[k] = RECUPERADO;
                                nuevos_recuperados++;
                            }
                    }
                }

            } //termino del bucle k
            // Actualización de los cambios al array del sistema real
            for (int q = 0; q < M; q++)
            {
                // Si el estado cambió, guardamos el ID en la lista correspondiente
                if (sistema->territorios[j].personas[q].estado != estados_futuros[q])
                {
                    int id_real = sistema->territorios[j].personas[q].id; // El ID del CSV

                    if (estados_futuros[q] == INFECTADO) {
                        list_nuevos_infectados[cant_inf++] = id_real;
                        sistema->territorios[j].personas[q].tiempo_contagio = 0; // Reset timer
                    }
                    else if (estados_futuros[q] == RECUPERADO) {
                        list_nuevos_recuperados[cant_rec++] = id_real;
                    }
                    else if (estados_futuros[q] == FALLECIDO) {
                        list_nuevos_fallecidos[cant_fal++] = id_real;
                    }

                    // Aplicar cambio
                    sistema->territorios[j].personas[q].estado = estados_futuros[q];
                    sistema->territorios[j].personas[q].cepa_id = cepas_futuras[q];
                }
                
            }

            //Impresión del reporte por territorio
            if (cant_inf > 0 || cant_rec > 0 || cant_fal > 0) {
                cambios = 1;
                // Pasamos el puntero del archivo 'fp' y los datos recolectados
                escribir_reporte(fp, sistema->territorios[j].territorio_id,list_nuevos_infectados, cant_inf,list_nuevos_recuperados, cant_rec,list_nuevos_fallecidos, cant_fal);
            }

            free(estados_futuros);
            free(cepas_futuras);
            free(list_nuevos_infectados);
            free(list_nuevos_recuperados);
            free(list_nuevos_fallecidos);
        }
        
        if (!cambios) {
            fprintf(fp, " Sin cambios registrados.\n");
        }
         
    }

    printf("Reporte completo guardado en: historial_simulacion.txt\n");
    fprintf(fp, "\n=== FIN DEL REPORTE ===\n");
    
    // CERRAR EL ARCHIVO (Esto guarda los cambios en disco)
    fclose(fp);
}

/* Como haremos la funcion de historial con complejidad O(1), ya que tenemos la limitante de que en la lectura de datos con un numero no especificado de tamaño, necesitamos si o si recorrer hasta llegar al final, pero no sabemos hasta cuanto recorrer lo que lo transforma a O(n), siendo asi el caso, una opción que se encontró es ir imprimiendo en un txt, siendo la función de la escritura individual una función O(1) al ser una salida de output que mantiene un puntero al final de cada linea como funcion append hace que el tiempo sea cosntante.

Para la consulta de algun dato en especifico que se quiera consultar ocupamos una tabla hash que almacene los datos requeridos, haciendo su tiempo de complejidad en busqueda de O(1).

*/



#endif