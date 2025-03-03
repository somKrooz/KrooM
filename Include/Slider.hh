#pragma once

#include "KObject.h"
#include "ShapeStruct.h"
#include "Shape.hh"
#include "iostream"

class KroozSlider : public KObject{

  private:
  int posX;
  int posY;
  float value;
  int width;
  GLFWwindow* Window;
  bool isDragging;
  Circle KroozCircle;

  public:
  KroozSlider(GLFWwindow* window,int x, int y, int w) : posX(x), posY(y), width(w), value(0.0f), isDragging(false) {}
  void DrawSlider();
  void UpdateSlider(double mx, double my);
  void Render() override;
  void Update(float DeltaTime, GLFWwindow* window, double mx, double my) override;
  float GetValue();
};

