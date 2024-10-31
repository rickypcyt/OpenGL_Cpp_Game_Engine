#include "models.h"
#include <iostream>
#include <unordered_map>

Model::Model() : VAO(0), VBO(0), EBO(0), isInitialized(false) {}

Model::~Model() {
    cleanup();
}

bool Model::loadFromFile(const std::string& objFilename, const std::string& mtlBasePath) {
    // Clean up any existing resources first
    cleanup();

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilename.c_str(),
        mtlBasePath.empty() ? nullptr : mtlBasePath.c_str());

    if (!warn.empty()) std::cout << "Warning: " << warn << std::endl;
    if (!err.empty()) {
        std::cerr << "Error: " << err << std::endl;
        return false;
    }
    if (!success) {
        std::cerr << "Failed to load model: " << objFilename << std::endl;
        return false;
    }

    // Process the loaded data
    if (!processModelData(attrib, shapes)) {
        std::cerr << "Failed to process model data" << std::endl;
        return false;
    }

    // Setup OpenGL buffers
    if (!setupBuffers()) {
        std::cerr << "Failed to setup OpenGL buffers" << std::endl;
        cleanup();
        return false;
    }

    isInitialized = true;
    return true;
}

void Model::draw() const {
    if (!isInitialized) {
        std::cerr << "Attempting to draw uninitialized model" << std::endl;
        return;
    }

    glBindVertexArray(VAO);
    if (glGetError() != GL_NO_ERROR) {
        std::cerr << "Error binding VAO" << std::endl;
        return;
    }

    if (!indices.empty()) {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }
    else {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size()));
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "Error during draw call: " << err << std::endl;
    }

    glBindVertexArray(0);
}

bool Model::processModelData(const tinyobj::attrib_t& attrib, const std::vector<tinyobj::shape_t>& shapes) {
    try {
        vertices.clear();
        indices.clear();
        std::unordered_map<std::string, uint32_t> uniqueVertices;

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                Vertex vertex{};  // Zero-initialize the vertex

                // Check array bounds before accessing
                if (index.vertex_index * 3 + 2 >= attrib.vertices.size()) {
                    std::cerr << "Vertex index out of bounds" << std::endl;
                    return false;
                }

                // Get vertex position
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };

                // Get vertex normal if available
                if (index.normal_index >= 0 && index.normal_index * 3 + 2 < attrib.normals.size()) {
                    vertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }
                else {
                    vertex.normal = { 0.0f, 1.0f, 0.0f };  // Default normal
                }

                // Get texture coordinates if available
                if (index.texcoord_index >= 0 && index.texcoord_index * 2 + 1 < attrib.texcoords.size()) {
                    vertex.texCoord = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }
                else {
                    vertex.texCoord = { 0.0f, 0.0f };  // Default UV
                }

                // Vertex deduplication
                std::string vertexHash = std::to_string(vertex.position.x) + "," +
                    std::to_string(vertex.position.y) + "," +
                    std::to_string(vertex.position.z) + "," +
                    std::to_string(vertex.normal.x) + "," +
                    std::to_string(vertex.normal.y) + "," +
                    std::to_string(vertex.normal.z) + "," +
                    std::to_string(vertex.texCoord.x) + "," +
                    std::to_string(vertex.texCoord.y);

                if (uniqueVertices.count(vertexHash) == 0) {
                    uniqueVertices[vertexHash] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }

                indices.push_back(uniqueVertices[vertexHash]);
            }
        }

        return !vertices.empty();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in processModelData: " << e.what() << std::endl;
        return false;
    }
}

bool Model::setupBuffers() {
    if (vertices.empty()) {
        std::cerr << "No vertices to setup buffers" << std::endl;
        return false;
    }

    // Clean up any existing buffers first
    cleanup();

    try {
        // Generate and bind VAO
        glGenVertexArrays(1, &VAO);
        if (VAO == 0) {
            std::cerr << "Failed to generate VAO" << std::endl;
            return false;
        }
        glBindVertexArray(VAO);

        // Generate and setup VBO
        glGenBuffers(1, &VBO);
        if (VBO == 0) {
            std::cerr << "Failed to generate VBO" << std::endl;
            cleanup();
            return false;
        }
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

        // Setup vertex attributes
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(2);

        // Setup EBO if we have indices
        if (!indices.empty()) {
            glGenBuffers(1, &EBO);
            if (EBO == 0) {
                std::cerr << "Failed to generate EBO" << std::endl;
                cleanup();
                return false;
            }
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        }

        // Check for OpenGL errors
        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL error during buffer setup: " << error << std::endl;
            cleanup();
            return false;
        }

        glBindVertexArray(0);
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in setupBuffers: " << e.what() << std::endl;
        cleanup();
        return false;
    }
}

void Model::cleanup() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }
    if (VBO != 0) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }
    if (EBO != 0) {
        glDeleteBuffers(1, &EBO);
        EBO = 0;
    }
    isInitialized = false;
}