#pragma once

#include <GL/glew.h>
#include "KObject.h"
#include "vector"
#include "ShapeStruct.h"
#include "Shape.hh"
#include "functional"
#include <stdio.h>
#include <Text.hh>
#include <GLFW/glfw3.h>

class KroozButton : public KObject{
private:
  int btnX;
  int btnY;
  int btnWidth;
  int btnHeight;
  bool m_wasPressed;
  const char* Label;
  GLFWwindow* Window;
  Rect Button;
  std::function<void()> m_onClick;
  std::unique_ptr<KroozFont> fnt = std::make_unique<KroozFont>();

public:
  KroozButton() : m_wasPressed(false), m_onClick([](){}) {}

  void DrawButton(GLFWwindow* window , const char* label, int x ,int y ,int width , int height);
  bool isHovering(double mx, double my);
  bool isClicked(double mx , double my);

  void setOnClick(std::function<void()> callback);

  void Update(float DeltaTime, GLFWwindow* window, double mx, double my) override;

  void Render() override;

};
