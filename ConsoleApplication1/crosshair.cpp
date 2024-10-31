#include <GL/glew.h> // Aseg�rate de incluir GLEW o GL si est�s usando OpenGL
#include "crosshair.h"
#include "globals.h"

// Implementaci�n de la funci�n para dibujar el crosshair
void drawCrosshair(int screenWidth, int screenHeight) {
    // Tama�o del crosshair en p�xeles
    float crosshairSize = 10.0f;

    // Calcula la posici�n central
    float centerX = screenWidth / 2.0f;
    float centerY = screenHeight / 2.0f;

    // Cambia temporalmente a proyecci�n ortogr�fica
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();       // Guarda la proyecci�n actual
    glLoadIdentity();
    gluOrtho2D(0, screenWidth, 0, screenHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();       // Guarda la matriz de modelo/vista actual
    glLoadIdentity();

    // Desactiva el Depth Test para que el crosshair est� siempre visible en pantalla
    glDisable(GL_DEPTH_TEST);

    // Configura el color del crosshair (rojo para visibilidad)
    glColor3f(0.0f, 255.0f, 255.0f);
    glLineWidth(3.0f);           // Grosor de las l�neas (aj�stalo a tu preferencia)

    // Dibuja el crosshair en el centro de la pantalla
    glBegin(GL_LINES);
    // L�nea horizontal
    glVertex2f(centerX - crosshairSize, centerY);
    glVertex2f(centerX + crosshairSize, centerY);

    // L�nea vertical
    glVertex2f(centerX, centerY - crosshairSize);
    glVertex2f(centerX, centerY + crosshairSize);
    glEnd();

    // Reactiva el Depth Test
    glEnable(GL_DEPTH_TEST);

    // Restaura las matrices de proyecci�n y modelo/vista originales
    glPopMatrix();          // Restaura la matriz de modelo/vista
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();          // Restaura la matriz de proyecci�n
    glMatrixMode(GL_MODELVIEW); // Vuelve al modo de vista de modelo
}

