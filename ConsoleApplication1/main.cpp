// Required OpenGL and utility libraries
#include <GL/glew.h>        
#include <GLFW/glfw3.h>     
#include <iostream>         
#include <chrono>          
#include <sstream>         
#include <thread>          
#include <glm/glm.hpp>     
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>   

// Custom engine components
#include "renderer.h"      
#include "movement.h"      
#include "globals.h"       
#include "cursor.h"        
#include "crosshair.h"
#include "lights.h"
#include "models.h"
#include "shaders.h"

// Global window handle
GLFWwindow* window = nullptr;

// Frame rate control constants and variables
const float TARGET_FPS = 120.0f;
const float FRAME_DURATION_MS = 1000.0f / TARGET_FPS;
float lastFrameTime = 0.0f;
float lastTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;

GLuint shaderProgram; // Your shader program ID
Model myModel; // Instance of your Model class

void displayFPS(float fps) {
    std::cout << "FPS: " << fps << std::endl;
}

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void renderText(const std::string& text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW\n";
        return -1;
    }

    // Create fullscreen window using primary monitor's resolution
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(1920, 1080, "MyOpenGLGame", monitor, nullptr);
    if (!window) {
        std::cerr << "Error creating GLFW window\n";
        glfwTerminate();
        return -1;
    }

    // Initialize OpenGL context and settings
    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1920, 1080);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW\n";
        return -1;
    }

    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setupProjection();
    setupLighting();
    updateLighting();
    renderObjectWithLighting();

    GLuint vertexShader = ShaderLoader::loadShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = ShaderLoader::loadShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);

    // Load textures
    GLuint floorTextureID = loadTexture("C:/Users/ricar/Documents/floor2.png");
    GLuint wallTextureID = loadTexture("C:/Users/ricar/Documents/wall1.jpg");

    // Create and load the model
    if (!myModel.loadFromFile("C:/Users/ricar/Documents/Cubone/model.obj", "C:/Users/ricar/Documents/Cubone/materials.mtl")) {
        std::cerr << "Failed to load model" << std::endl;
        return -1;
    }

    auto lastFrameTimePoint = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        auto frameStartTime = std::chrono::high_resolution_clock::now();

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera view
        glm::vec3 cameraPosition(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
            cameraTarget.x, cameraTarget.y, cameraTarget.z,
            0.0f, 1.0f, 0.0f);

        // Draw floor
        drawFloor(floorTextureID);

        // Draw wall (as before)
        drawWall(wallTextureID, 0.0f, 0.0f, 10.0f, 5.0f, true);

        // Draw the loaded model
        myModel.draw(shaderProgram); // Render the model using the shader program

        // 2D overlay rendering
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, WIDTH, 0, HEIGHT);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        drawCrosshair(WIDTH, HEIGHT);

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);

        updateMovement(deltaTime);

        // FPS calculation and display
        frameCount++;
        if (glfwGetTime() - lastTime >= 1.0) {
            fps = static_cast<float>(frameCount);
            displayFPS(fps);
            frameCount = 0;
            lastTime += 1.0;
        }

        std::ostringstream fpsStream;
        fpsStream << "FPS: " << fps;
        renderText(fpsStream.str(), -0.9f, 0.9f);

        glfwSwapBuffers(window);
        glfwPollEvents();

        // Frame rate limiting
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        if (frameDuration < FRAME_DURATION_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_DURATION_MS - frameDuration)));
        }
    }

    glfwTerminate();
    return 0;
}
