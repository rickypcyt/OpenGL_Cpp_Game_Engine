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

float lastFrameTime = 0.0f;
float lastTime = 0.0f;
const float TARGET_FPS = 60.0f;
const float FRAME_DURATION = 1000.0f / TARGET_FPS; // in milliseconds

void displayFPS(float fps) {
    std::ostringstream oss;
    oss << "FPS: " << fps;
    std::cout << oss.str() << std::endl;
}

using namespace std::chrono;
int frameCount = 0;
float fps = 0.0f;

void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f); // Increased FOV to 90
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
    window = glfwCreateWindow(1920, 1080, "MyOpenGLGame", monitor, NULL);
    if (!window) {
        std::cerr << "Error creating GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 1920, 1080);
    glfwMakeContextCurrent(window);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing GLEW\n";
        return -1;
    }

    int argc = 0;
    char** argv = nullptr;
    glutInit(&argc, argv); // Initialize FreeGLUT

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setupProjection();

    high_resolution_clock::time_point lastFrameTimePoint = high_resolution_clock::now();

    while (!glfwWindowShouldClose(window)) {
        auto frameStartTime = high_resolution_clock::now();

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrameTime;
        lastFrameTime = currentFrame;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();

        // Set up camera position and direction
        glm::vec3 cameraPosition = glm::vec3(characterPosX, characterPosY + 1.5f, characterPosZ);
        glm::vec3 cameraTarget = cameraPosition + cameraFront;
        gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
            cameraTarget.x, cameraTarget.y, cameraTarget.z,
            0.0f, 1.0f, 0.0f);

        drawFloor();
        updateMovement(deltaTime); // Handle movement

        frameCount++;
        if (glfwGetTime() - lastTime >= 1.0) {
            fps = frameCount;
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
        auto frameEndTime = high_resolution_clock::now();
        duration<float, std::milli> frameDuration = frameEndTime - frameStartTime;

        if (frameDuration.count() < FRAME_DURATION) {
            std::this_thread::sleep_for(milliseconds(static_cast<int>(FRAME_DURATION - frameDuration.count())));
        }
    }

    glfwTerminate();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return 0;
}
