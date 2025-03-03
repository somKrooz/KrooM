#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <KObject.h>
#include <memory>
#include <vector>

class Application {
  private:
  std::vector<std::unique_ptr<KObject>> m_Components;

  public:
  void addComponents(std::unique_ptr<KObject> comp);
  void RenderComponents();
  void UpdateComponents(float Delta, GLFWwindow* window , double mx , double my);
};
