#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>

#include "..\..\..\..\Documents\CODEO 2024\CppOpenGLGameEngine-main\include\renderer.h"
#include "..\..\..\..\Documents\CODEO 2024\CppOpenGLGameEngine-main\include\movement.h"
#include "..\..\..\..\Documents\CODEO 2024\CppOpenGLGameEngine-main\include\globals.h"
#include "..\..\..\..\Documents\CODEO 2024\CppOpenGLGameEngine-main\include\cursor.h"

GLFWwindow* window = nullptr;

const float TARGET_FPS = 60.0f;
const float FRAME_DURATION_MS = 1000.0f / TARGET_FPS; // in milliseconds
float lastFrameTime = 0.0f;
float lastTime = 0.0f;
int frameCount = 0;
float fps = 0.0f;

void displayFPS(float fps) {
    std::cout << "FPS: " << fps << std::endl;
}

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f); // Increased FOV to 90
    glMatrixMode(GL_MODELVIEW);
}

void renderText(const std::string& text, float x, float y) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Error initializing GLFW\n";
        return -1;
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    window = glfwCreateWindow(1920, 1080, "MyOpenGLGame", monitor, nullptr);
    if (!window) {
        std::cerr << "Error creating GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glViewport(0, 0, 1920, 1080);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW\n";
        return -1;
    }

    // Initialize FreeGLUT
    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv);

    // Set input callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setupProjection();

    auto lastFrameTimePoint = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto frameStartTime = std::chrono::high_resolution_clock::now();

        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera position and direction
        glm::vec3 cameraPosition(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
            cameraTarget.x, cameraTarget.y, cameraTarget.z,
            0.0f, 1.0f, 0.0f);

        drawFloor();
        updateMovement(deltaTime); // Handle movement

        // FPS calculation
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

        // Frame capping logic
        auto frameEndTime = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration<float, std::milli>(frameEndTime - frameStartTime).count();

        if (frameDuration < FRAME_DURATION_MS) {
            std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(FRAME_DURATION_MS - frameDuration)));
        }
    }

    glfwTerminate();
    return 0;
}
