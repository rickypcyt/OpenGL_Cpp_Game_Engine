#pragma once
#ifndef SHADERLOADER_H
#define SHADERLOADER_H

#include <GL/glew.h>
#include <string>

class ShaderLoader {
public:
    // Loads a shader from a file and returns its ID
    static GLuint loadShader(const std::string& filename, GLenum shaderType);

    // Creates a shader program from vertex and fragment shader source files
    static GLuint createShaderProgram(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);

private:
    // Helper function to read the shader source code from a file
    static std::string loadShaderFromFile(const std::string& filename);
};

#endif // SHADERLOADER_H
