#ifndef WINDOW_H
#define WINDOW_H

// Include glad and glfw for graphics and windowing
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

namespace csci3081 {

class IWindowObserver {
public:
  virtual ~IWindowObserver() {}
  virtual void onResize(float width, float height) = 0;
  virtual void onMouseMove(float x, float y) = 0;
  virtual void onMouseClick(int button, int action, int mods) = 0;
};

class Window {
public:
  Window();
  ~Window();

  int getWidth() const { return width; }
  int getHeight() const { return height; }
  void setHeight(int height) { this->height = height; }
  void setWidth(int width) { this->width = width; }
  void onResize(float width, float height);
  void onMouseMove(float x, float y);
  void onMouseClick(int button, int action, int mods);
  void addObserver(IWindowObserver &observer) {
    observers.push_back(&observer);
  }
  void swapBuffers();
  GLFWwindow *getWindow() { return window; }

private:
  GLFWwindow *window;
  int width;
  int height;
  std::vector<IWindowObserver *> observers;
};

} // namespace csci3081

#endif