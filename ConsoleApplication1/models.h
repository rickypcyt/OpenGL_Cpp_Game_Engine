#ifndef MODELS_H
#define MODELS_H

#include <vector>
#include <GL/glew.h>
#include "tiny_obj_loader.h" // Make sure to include the tiny_obj_loader header

struct Vertex {
    float position[3];
    float normal[3];
    float texCoord[2];
};

class Model {
public:
    Model();
    ~Model();

    bool loadFromFile(const std::string& filename);
    void draw() const;

private:
    void setupBuffers();

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    GLuint VAO, VBO, EBO;
};

#endif // MODELS_H
