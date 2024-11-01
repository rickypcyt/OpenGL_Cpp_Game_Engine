#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

void main() {
    FragColor = vec4(TexCoord, 0.0, 1.0); // Example color output
}
