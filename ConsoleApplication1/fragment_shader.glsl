#version 330 core
in vec3 fragColor; // Receive color from the vertex shader
out vec4 outputColor;

void main() {
    outputColor = vec4(fragColor, 1.0); // Set the output color
}
