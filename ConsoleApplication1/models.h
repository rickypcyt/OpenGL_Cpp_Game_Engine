#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h> // Make sure to include GLEW (or your OpenGL loader)
#include <tiny_obj_loader.h> // Include TinyOBJ loader

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Model {
public:
    Model();
    ~Model();
    bool loadFromFile(const std::string& objFilename, const std::string& mtlBasePath);
    void draw(GLuint shaderProgram) const;
    // Other methods...

private:
    GLuint VAO, VBO, EBO;
    bool isInitialized;
    glm::mat4 modelMatrix;  // This should be a member variable
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    bool processModelData(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
    bool setupBuffers();
    void cleanup();
};


#endif // MODELS_H
