#pragma once
#include "ShapeStruct.h"

class Shape{

  public:
  static void DrawRect(Rect &shape);
  static void DrawCircle(Circle &shape , int segment);
  static void DrawBar(Bar &shape);

};
