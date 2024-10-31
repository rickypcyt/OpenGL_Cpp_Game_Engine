// Required OpenGL and utility libraries
#include <GL/glew.h>        // OpenGL Extension Wrangler - for modern OpenGL functionality
#include <GLFW/glfw3.h>     // Window management and OpenGL context creation
#include <iostream>         // Standard I/O operations
#include <chrono>          // Time-related functionality
#include <sstream>         // String stream for text formatting
#include <thread>          // Thread management for frame timing
#include <glm/glm.hpp>     // OpenGL Mathematics library for vectors and matrices
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>   // For text rendering capabilities

// Custom engine components
#include "renderer.h"      // Graphics rendering utilities
#include "movement.h"      // Character movement and physics
#include "globals.h"       // Global variables and constants
#include "cursor.h"        // Cursor/mouse input handling
#include "crosshair.h"
#include "lights.h"


// Global window handle
GLFWwindow* window = nullptr;

// Frame rate control constants and variables
const float TARGET_FPS = 120.0f;                    // Desired frames per second
const float FRAME_DURATION_MS = 1000.0f / TARGET_FPS;  // Duration of one frame in milliseconds
float lastFrameTime = 0.0f;                        // Time of last frame render
float lastTime = 0.0f;                             // Time tracker for FPS calculation
int frameCount = 0;                                // Frame counter for FPS calculation
float fps = 0.0f;                                  // Current FPS value

/**
 * Displays the current FPS in the console
 * @param fps Current frames per second value
 */
void displayFPS(float fps) {
    std::cout << "FPS: " << fps << std::endl;
}

/**
 * Sets up the 3D projection matrix for the scene
 * Configures perspective projection with 90-degree FOV
 */
void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Parameters: FOV, aspect ratio, near clipping plane, far clipping plane
    gluPerspective(90.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/**
 * Renders text on the screen using GLUT bitmap fonts
 * @param text The string to render
 * @param x X-coordinate for text position
 * @param y Y-coordinate for text position
 */
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
    glEnable(GL_MULTISAMPLE);  // Enable anti-aliasing
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for 3D rendering

    // Initialize GLEW for modern OpenGL functionality
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW\n";
        return -1;
    }

    // Initialize GLUT for text rendering
    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv);

    // Set up input handling
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // Hide cursor for FPS-style camera

    setupProjection();

    setupLighting();

    updateLighting();

    renderObjectWithLighting();

    GLuint floorTextureID = loadTexture("C:/Users/ricar/Documents/floor2.png");

    // Main game loop
    auto lastFrameTimePoint = std::chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(window)) {
        // Frame timing
        auto frameStartTime = std::chrono::high_resolution_clock::now();
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        // Clear the screen and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera view
        glm::vec3 cameraPosition(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
            cameraTarget.x, cameraTarget.y, cameraTarget.z,
            0.0f, 1.0f, 0.0f);

        // Render scene elements

        drawFloor(floorTextureID);

        // After drawing 3D elements like the floor
        glDisable(GL_DEPTH_TEST); // Disable depth test for 2D overlay

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, WIDTH, 0, HEIGHT); // Set orthographic projection

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        drawCrosshair(WIDTH, HEIGHT); // Draw crosshair in 2D

        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glEnable(GL_DEPTH_TEST); // Re-enable depth test for next frame

        glColor3f(1.0f, 1.0f, 1.0f);



        updateMovement(deltaTime);

        frameCount++;
        if (glfwGetTime() - lastTime >= 1.0) {
            fps = static_cast<float>(frameCount);
            displayFPS(fps);
            frameCount = 0;
            lastTime += 1.0;
        }

        // Render FPS counter on screen
        std::ostringstream fpsStream;
        fpsStream << "FPS: " << fps;
        renderText(fpsStream.str(), -0.9f, 0.9f);

        // Swap buffers and process events
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Frame rate limiting
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        if (frameDuration < FRAME_DURATION_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_DURATION_MS - frameDuration)));
        }
    }

    // Clean up
    glfwTerminate();
    return 0;
}
