#include <GL/freeglut.h>
#include <math.h>
#include <stdio.h>
#include "estructuras.h"
#include "Propagacion.h" // Necesitamos acceder a la logica de contagio
#include "Animacion.h"

// --- CONFIGURACIÓN ---
#define ANCHO_VENTANA 1000
#define ALTO_VENTANA 700
#define TAMANO_BOLITA 4.0f

// Variables globales para la animación
sistema *sys_viz;
int animacion_activa = 0;
int dia_actual_viz = 0;
int limite_dias_viz = 0; // Variable meta o limite de dias asignada en el main de BIOSIM

// =============================================================
// 1. INICIALIZAR POSICIONES (Solo se hace una vez)
// =============================================================
void asignar_posiciones_graficas(sistema *s) {
    // Calculamos una rejilla para los territorios (ej. 3 columnas)
    int columnas = 4; // Ajusta según cuantos territorios tengas
    int filas = (s->numterritorios / columnas) + 1;
    
    float ancho_terr = (float)ANCHO_VENTANA / columnas;
    float alto_terr = (float)ALTO_VENTANA / filas;

    for (int t = 0; t < s->numterritorios; t++) {
        // Coordenadas del cuadro del territorio
        int col = t % columnas;
        int row = t / columnas;
        
        float x_base = col * ancho_terr;
        float y_base = row * alto_terr;

        // Asignar posición aleatoria a cada persona DENTRO de su cuadro
        for (int k = 0; k < s->territorios[t].M; k++) {
            // Margen de 20 pixeles para que no queden pegados a la orilla
            float offset_x = 20 + rand() % (int)(ancho_terr - 40);
            float offset_y = 20 + rand() % (int)(alto_terr - 40);
            
            s->territorios[t].personas[k].x = x_base + offset_x;
            // Invertimos Y porque en OpenGL el 0 está abajo (opcional)
            s->territorios[t].personas[k].y = ALTO_VENTANA - (y_base + offset_y);
        }
    }
}

// =============================================================
// 2. DIBUJAR EN PANTALLA
// =============================================================
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Limpiar fondo (Negro)

    // A) DIBUJAR TERRITORIOS (Cuadros Blancos)
    int columnas = 4;
    float ancho_terr = (float)ANCHO_VENTANA / columnas;
    float filas = (sys_viz->numterritorios / columnas) + 1;
    float alto_terr = (float)ALTO_VENTANA / filas;

    glColor3f(0.3, 0.3, 0.3); // Gris oscuro para las lineas
    glLineWidth(2.0);
    
    for(int t=0; t < sys_viz->numterritorios; t++) {
        int col = t % columnas;
        int row = t / columnas;
        float x = col * ancho_terr;
        float y = ALTO_VENTANA - (row * alto_terr); // Invertir Y para dibujar de arriba abajo

        glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + ancho_terr, y);
            glVertex2f(x + ancho_terr, y - alto_terr);
            glVertex2f(x, y - alto_terr);
        glEnd();
    }

    // B) DIBUJAR PERSONAS (Puntos)
    glPointSize(TAMANO_BOLITA);
    glBegin(GL_POINTS);
    for (int t = 0; t < sys_viz->numterritorios; t++) {
        for (int k = 0; k < sys_viz->territorios[t].M; k++) {
            persona p = sys_viz->territorios[t].personas[k];

            // COLOR SEGÚN ESTADO
            if (p.estado == SANO) glColor3f(0.0, 1.0, 0.0);       // VERDE
            else if (p.estado == INFECTADO) glColor3f(1.0, 0.0, 0.0); // ROJO (Semilla/Infectado)
            else if (p.estado == RECUPERADO) glColor3f(0.0, 0.5, 1.0); // AZUL
            else if(p.estado == FALLECIDO) glColor3f(1.0, 1.0, 1.0); // Blanco (Fallecido)

            glVertex2f(p.x, p.y);
        }
    }
    glEnd();

    // C) DIBUJAR BOTÓN "INICIAR"
    if (!animacion_activa) {
        glColor3f(1.0, 1.0, 1.0); // Botón Blanco
        glRectf(ANCHO_VENTANA/2 - 50, 10, ANCHO_VENTANA/2 + 50, 50); // Abajo al centro
        
        // Texto (Negro)
        glColor3f(0.0, 0.0, 0.0);
        glRasterPos2f(ANCHO_VENTANA/2 - 20, 25);
        char *texto = "INICIAR";
        for (char *c = texto; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);

        if (dia_actual_viz >= limite_dias_viz && limite_dias_viz > 0) {
            glColor3f(1.0, 1.0, 0.0); // Amarillo
            glRasterPos2f(ANCHO_VENTANA/2 - 40, 70); // Un poco más arriba del botón
            char *fin = "FINALIZADO";
            for (char *c = fin; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
        }

    } else {
        // Texto del Día
        glColor3f(1.0, 1.0, 1.0);
        glRasterPos2f(20, 20);
        char buf[50];
        sprintf(buf, "DIA: %d", dia_actual_viz);
        for (char *c = buf; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    glFlush();
}

// =============================================================
// 3. LÓGICA DE TIEMPO (EL "BUCLE" DE SIMULACIÓN)
// =============================================================
void timer(int valor) {
    if (animacion_activa) {
        //Condicion de parada por dias indicados en BIOSIM main
        if (dia_actual_viz >= limite_dias_viz) {
            animacion_activa = 0; // Detener animación
            printf("--- Fin de la simulacion visual (%d dias alcanzados) ---\n", limite_dias_viz);
        } 
        else {
            // Si no hemos llegado, avanzamos un día
            simular_un_paso_logico(sys_viz); 
            dia_actual_viz++;
            glutPostRedisplay(); // Redibujar
        }
    }
    // Repetir cada 700ms (Medio segundo por día)
    glutTimerFunc(700, timer, 0); 
}

// =============================================================
// 4. CLIC DEL MOUSE
// =============================================================
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Coordenadas OpenGL (Y invertida)
        int gl_y = ALTO_VENTANA - y; 
        
        // Si clic en el botón (centro abajo)
        if (!animacion_activa && 
            x > (ANCHO_VENTANA/2 - 50) && x < (ANCHO_VENTANA/2 + 50) && 
            gl_y > 10 && gl_y < 50) {
            
            animacion_activa = 1;
            printf("Animacion Grafica Iniciada...\n");
        }
    }
}

// =============================================================
// 5. FUNCIÓN PRINCIPAL DE VISUALIZACIÓN
// =============================================================
void lanzar_visualizacion(sistema *s, int dias_meta) {
    // 1. Preparar datos visuales
    sys_viz = s;
    limite_dias_viz = dias_meta; // <--- Guardamos el límite
    dia_actual_viz = 0;          // Reseteamos el contador
    animacion_activa = 0;
    asignar_posiciones_graficas(s); 

    // 2. Configurar GLUT
    int argc = 1; char *argv[] = {"BIOSIM", NULL};
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(ANCHO_VENTANA, ALTO_VENTANA);
    glutCreateWindow("BIOSIM - Visualizacion de Propagacion");

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    // 3. Configurar Vista 2D (Pixel exacto)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, ANCHO_VENTANA, 0, ALTO_VENTANA);
    glClearColor(0.05, 0.05, 0.05, 1.0); // Fondo casi negro

    // 4. Callbacks
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutTimerFunc(500, timer, 0);

    // 5. ¡Acción!
    printf("Ventana grafica abierta. Pulse INICIAR.\n");
    glutMainLoop(); 
}