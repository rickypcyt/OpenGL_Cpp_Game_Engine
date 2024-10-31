#version 330 core
in vec3 fragNormal;
in vec3 fragPosition;

uniform vec3 lightPosition;
uniform vec3 viewPosition; // Camera position

out vec4 color;

void main() {
    // Ambient
    vec3 ambient = vec3(0.2);

    // Diffuse
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPosition - fragPosition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0); // Light color (white)

    // Specular
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(1.0) * spec; // Specular color (white)

    color = vec4(ambient + diffuse + specular, 1.0);
}
