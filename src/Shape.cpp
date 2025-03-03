#include <Shape.hh>

void Shape::DrawRect(Rect &shape){
  glBegin(GL_QUADS);
  glColor3f(shape.r , shape.g , shape.b);
  glVertex2i(shape.x  ,shape.y);
  glVertex2i(shape.x + shape.width  ,shape.y);
  glVertex2i(shape.x + shape.width  ,shape.y + shape.height);
  glVertex2i(shape.x  ,shape.y + shape.height);
  glEnd();
  glColor3f(0.0f, 0.0f, 0.0f);
}

void Shape::DrawCircle(Circle &shape , int segment){
  glBegin(GL_POLYGON);
  glColor3f(shape.r , shape.g , shape.b);
  glVertex2f(shape.cx, shape.cy);

  for(int i = 0; i<= segment ; i++){
    float theta = 2.0f * 3.1415 * (float) i/ (float) segment;
    float x  = shape.radius * cosf(theta);
    float y  = shape.radius * sinf(theta);
    glVertex2f(shape.cx + x, shape.cy + y);
  }
  glEnd();
  glColor3f(0.0f, 0.0f, 0.0f);
}

void Shape::DrawBar(Bar &shape){
  glColor3f(shape.r, shape.g, shape.b);
  glLineWidth(5.0f);
  glBegin(GL_LINES);
  glVertex2f(shape.x, shape.y);
  glVertex2f(shape.x + shape.width, shape.y);
  glEnd();
  glColor3f(0.0f, 0.0f, 0.0f);
}
