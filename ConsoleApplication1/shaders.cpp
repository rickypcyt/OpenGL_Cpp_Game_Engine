#include "shaders.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string ShaderLoader::loadShaderFromFile(const std::string& filename) {
    std::ifstream shaderFile(filename);
    if (!shaderFile) {
        std::cerr << "Failed to open shader file: " << filename << std::endl;
        return "";
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf(); // Read the file's buffer contents into the stream
    return shaderStream.str(); // Convert stream into string
}

GLuint ShaderLoader::loadShader(const std::string& filename, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    std::string shaderSource = loadShaderFromFile(filename);

    const char* sourceCStr = shaderSource.c_str();
    glShaderSource(shader, 1, &sourceCStr, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint ShaderLoader::createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile) {
    GLuint vertexShader = loadShader(vertexShaderFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

    // Create shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Delete the shaders as they're linked now
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}
