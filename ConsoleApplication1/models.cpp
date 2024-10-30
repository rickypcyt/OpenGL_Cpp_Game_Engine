#include "models.h"
#include <iostream>

Model::Model() : VAO(0), VBO(0), EBO(0) {}

Model::~Model() {
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

bool Model::loadFromFile(const std::string& filename) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    // Load the OBJ file
    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
    if (!warn.empty()) {
        std::cout << warn << std::endl;
    }
    if (!err.empty()) {
        std::cerr << err << std::endl;
    }
    if (!success) {
        return false;
    }

    // Process loaded data
    for (const auto& shape : shapes) {
        size_t index_offset = 0;
        for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
            size_t num_verts = size_t(shape.mesh.num_face_vertices[f]);
            for (size_t v = 0; v < num_verts; v++) {
                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                Vertex vertex;

                // Load vertex positions
                vertex.position[0] = attrib.vertices[3 * idx.vertex_index + 0];
                vertex.position[1] = attrib.vertices[3 * idx.vertex_index + 1];
                vertex.position[2] = attrib.vertices[3 * idx.vertex_index + 2];

                // Load normals
                if (idx.normal_index >= 0) {
                    vertex.normal[0] = attrib.normals[3 * idx.normal_index + 0];
                    vertex.normal[1] = attrib.normals[3 * idx.normal_index + 1];
                    vertex.normal[2] = attrib.normals[3 * idx.normal_index + 2];
                }
                else {
                    vertex.normal[0] = 0.0f;
                    vertex.normal[1] = 0.0f;
                    vertex.normal[2] = 0.0f;
                }

                // Load texture coordinates
                if (idx.texcoord_index >= 0) {
                    vertex.texCoord[0] = attrib.texcoords[2 * idx.texcoord_index + 0];
                    vertex.texCoord[1] = attrib.texcoords[2 * idx.texcoord_index + 1];
                }
                else {
                    vertex.texCoord[0] = 0.0f;
                    vertex.texCoord[1] = 0.0f;
                }

                vertices.push_back(vertex);
            }
            index_offset += num_verts;
        }
    }

    // Setup OpenGL buffers
    setupBuffers();

    return true;
}

void Model::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Element Buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Model::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
