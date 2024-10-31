// lights.cpp

#include "lights.h"

// Light properties
glm::vec3 lightPosition(0.0f, 10.0f, 0.0f);
glm::vec3 ambientColor(0.2f, 0.2f, 0.2f);
glm::vec3 diffuseColor(1.0f, 1.0f, 1.0f);
glm::vec3 specularColor(1.0f, 1.0f, 1.0f);

// Material properties
glm::vec3 materialAmbient(0.2f, 0.2f, 0.2f);
glm::vec3 materialDiffuse(0.8f, 0.0f, 0.0f);  // Red material
glm::vec3 materialSpecular(1.0f, 1.0f, 1.0f); // White specular highlights
float materialShininess = 32.0f; // Shininess coefficient

void setupLighting() {
    // Enable lighting
    glEnable(GL_LIGHTING);

    // Set up a directional light (like sunlight)
    GLfloat sunAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // Ambient light color
    GLfloat sunDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };   // Diffuse light color
    GLfloat sunSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Specular light color
    GLfloat sunPosition[] = { 0.0f, 1.0f, 0.0f, 0.0f };  // Position of the light (directional)

    // Set light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
    glLightfv(GL_LIGHT0, GL_POSITION, sunPosition);

    // Enable the light
    glEnable(GL_LIGHT0);

    // You can add more lights if needed by using GL_LIGHT1, GL_LIGHT2, etc.
}


void renderObjectWithLighting() {
    // Draw a simple cube with lighting applied
    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f); // Normal for lighting
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
}
