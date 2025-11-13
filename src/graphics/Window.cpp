#include "graphics/Window.h"
#include <iostream>

namespace csci3081 {

// Prototypes for user interaction
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Window::Window() {
    // -------------------------------------
    // Create window of height and width.  Initialize input callbacks
    // -------------------------------------
    GLFWmonitor* monitor =  glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    window = glfwCreateWindow(mode->width, mode->height, "Video Editor", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetWindowUserPointer(window, this);
}


Window::~Window() {
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::onResize(float width, float height) {
    this->width = width;
    this->height = height;

    glViewport(0, 0, width, height);

    for (int i = 0; i < observers.size(); i++) {
        observers[i]->onResize(width, height);
    }
}

void Window::onMouseMove(float x, float y) {
    for (int i = 0; i < observers.size(); i++) {
        observers[i]->onMouseMove(x, y);
    }
}

void Window::onMouseClick(int button, int action, int mods) {
    for (int i = 0; i < observers.size(); i++) {
        observers[i]->onMouseClick(button, action, mods);
    }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Window& win = *static_cast<Window*>(glfwGetWindowUserPointer(window));
    win.onResize(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Window& win = *static_cast<Window*>(glfwGetWindowUserPointer(window));
    win.onMouseClick(button, action, mods);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    Window& win = *static_cast<Window*>(glfwGetWindowUserPointer(window));

    // Get framebuffer size (actual pixel dimensions) for proper scaling
    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    // Get window size (screen coordinates)
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Scale mouse position from window coordinates to framebuffer coordinates
    double scaledX = xpos * framebufferWidth / windowWidth;
    double scaledY = ypos * framebufferHeight / windowHeight;

    // Normalize to 0-1 range using framebuffer dimensions
    win.onMouseMove(scaledX / framebufferWidth, scaledY / framebufferHeight);
}


}