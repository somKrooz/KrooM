#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class KObject{
public:
  virtual void Render() = 0;
  virtual void Update(float DeltaTime, GLFWwindow* window, double mx, double my) = 0;;
  virtual ~KObject() = default;
};
