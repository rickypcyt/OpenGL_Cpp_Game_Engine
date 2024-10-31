#include "lights.h"

// Initialize external variables
glm::vec3 lightPosition(100.0f, 100.0f, 100.0f);
glm::vec3 ambientColor(0.3f, 0.3f, 0.3f);
glm::vec3 diffuseColor(1.0f, 1.0f, 1.0f);
glm::vec3 specularColor(1.0f, 1.0f, 1.0f);
glm::vec3 materialAmbient(0.2f, 0.2f, 0.2f);
glm::vec3 materialDiffuse(0.8f, 0.8f, 0.8f);
glm::vec3 materialSpecular(0.5f, 0.5f, 0.5f);
float materialShininess = 50.0f;
glm::vec3 globalAmbient(0.4f, 0.4f, 0.4f);
float lightIntensity = 1.0f;

void setupLighting() {
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    // Set global ambient light
    GLfloat globalAmbientArray[] = {
        globalAmbient.x, globalAmbient.y, globalAmbient.z, 1.0f
    };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientArray);

    // Convert glm::vec3 to arrays for OpenGL
    GLfloat ambientArray[] = {
        ambientColor.x, ambientColor.y, ambientColor.z, 1.0f
    };
    GLfloat diffuseArray[] = {
        diffuseColor.x, diffuseColor.y, diffuseColor.z, 1.0f
    };
    GLfloat specularArray[] = {
        specularColor.x, specularColor.y, specularColor.z, 1.0f
    };
    GLfloat positionArray[] = {
        lightPosition.x, lightPosition.y, lightPosition.z, 0.0f
    };

    // Set light parameters
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientArray);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseArray);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularArray);
    glLightfv(GL_LIGHT0, GL_POSITION, positionArray);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void updateLighting() {
    glPushMatrix();
    glLoadIdentity();

    // Update light position
    GLfloat positionArray[] = {
        lightPosition.x, lightPosition.y, lightPosition.z, 0.0f
    };
    glLightfv(GL_LIGHT0, GL_POSITION, positionArray);

    glPopMatrix();
}

void renderObjectWithLighting() {
    // Convert material properties to arrays
    GLfloat matAmbient[] = {
        materialAmbient.x, materialAmbient.y, materialAmbient.z, 1.0f
    };
    GLfloat matDiffuse[] = {
        materialDiffuse.x, materialDiffuse.y, materialDiffuse.z, 1.0f
    };
    GLfloat matSpecular[] = {
        materialSpecular.x, materialSpecular.y, materialSpecular.z, 1.0f
    };

    // Apply material properties
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
}