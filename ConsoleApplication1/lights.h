#pragma once
// lights.h
#ifndef LIGHTS_H
#define LIGHTS_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Function declarations
void setupLighting();
void updateLighting();
void renderObjectWithLighting();

// Light and material properties
extern glm::vec3 lightPosition;
extern glm::vec3 ambientColor;
extern glm::vec3 diffuseColor;
extern glm::vec3 specularColor;
extern glm::vec3 materialAmbient;
extern glm::vec3 materialDiffuse;
extern glm::vec3 materialSpecular;
extern float materialShininess;

// Additional lighting parameters
extern glm::vec3 globalAmbient;  // Global ambient light
extern float lightIntensity;     // Overall light intensity multiplier

#endif // LIGHTS_H