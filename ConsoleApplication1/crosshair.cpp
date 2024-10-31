#include <GL/glew.h> // Asegúrate de incluir GLEW o GL si estás usando OpenGL
#include "crosshair.h"
#include "globals.h"

// Implementación de la función para dibujar el crosshair
void drawCrosshair(int screenWidth, int screenHeight) {
    // Tamaño del crosshair en píxeles
    float crosshairSize = 10.0f;

    // Calcula la posición central
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Cambia temporalmente a proyección ortográfica
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();       // Guarda la proyección actual
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();       // Guarda la matriz de modelo/vista actual
    glLoadIdentity();

    // Desactiva el Depth Test para que el crosshair esté siempre visible en pantalla
    glDisable(GL_DEPTH_TEST);

    // Configura el color del crosshair (rojo para visibilidad)
    glColor3f(0.0f, 255.0f, 255.0f);
    glLineWidth(3.0f);           // Grosor de las líneas (ajústalo a tu preferencia)

    // Dibuja el crosshair en el centro de la pantalla
    glBegin(GL_LINES);
    // Línea horizontal
    glVertex2f(centerX - crosshairSize, centerY);
    glVertex2f(centerX + crosshairSize, centerY);

    // Línea vertical
    glVertex2f(centerX, centerY - crosshairSize);
    glVertex2f(centerX, centerY + crosshairSize);
    glEnd();

    // Reactiva el Depth Test
    glEnable(GL_DEPTH_TEST);

    // Restaura las matrices de proyección y modelo/vista originales
    glPopMatrix();          // Restaura la matriz de modelo/vista
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();          // Restaura la matriz de proyección
    glMatrixMode(GL_MODELVIEW); // Vuelve al modo de vista de modelo
}

