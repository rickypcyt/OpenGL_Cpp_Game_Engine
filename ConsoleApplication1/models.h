#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>  // Include OpenGL headers
#include <glm/glm.hpp>  // Include GLM
#include <string>
#include <vector>
#include <tiny_obj_loader.h>  // Include tiny_obj_loader

// Vertex structure definition
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
    void draw() const;

private:
    bool processModelData(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes);
    bool setupBuffers();
    void cleanup();

    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    bool isInitialized;
};

#endif // MODEL_H