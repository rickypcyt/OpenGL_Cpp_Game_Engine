#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 FragPos; // Pass the position to fragment shader
out vec3 Normal; // Pass the normal to fragment shader

void main() {
    FragPos = vec3(model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(model))) * normal; // Transform normal
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
