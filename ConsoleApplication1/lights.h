#pragma once
// lights.h

#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


void setupLighting();
void renderObjectWithLighting();

extern glm::vec3 lightPosition;
extern glm::vec3 ambientColor;
extern glm::vec3 diffuseColor;
extern glm::vec3 specularColor;

extern glm::vec3 materialAmbient;
extern glm::vec3 materialDiffuse;
extern glm::vec3 materialSpecular;
extern float materialShininess;

#endif // LIGHTS_H
