#include <Slider.hh>

void KroozSlider::DrawSlider(){
  Bar KroozBar = {this->posX , this->posY , this->width,0.3f ,0.3f ,0.3f};
  Shape::DrawBar(KroozBar);

  float HandleX = this->posX + this->value * this->width;
  this->KroozCircle = {HandleX  , (float) this->posY , 10.0f ,this->KroozCircle.r,this->KroozCircle.g,this->KroozCircle.b};
  Shape::DrawCircle(KroozCircle,20);
}

void KroozSlider::UpdateSlider(double mx, double my){
  int width , height;
  glfwGetWindowSize(this->Window ,&width , &height);

  double screenY = height - my;
  bool pressed = glfwGetMouseButton(this->Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
  float handleX = this->posX + this->value * this->width;

  bool hovering = (mx >= handleX - 10 && mx <= handleX + 10) &&
                  (screenY >= posY - 10 && screenY <= posY + 10);

  if (hovering)
  {
      this->KroozCircle.r = 0.8f;
      this->KroozCircle.g = 0.8f;
      this->KroozCircle.b = 0.8f;
  } else {
      this->KroozCircle.r = 0.3f;
      this->KroozCircle.g = 0.3f;
      this->KroozCircle.b = 0.3f;
  }

  if (hovering && pressed && !isDragging) isDragging = true;

  if (!pressed) isDragging = false;

  if (isDragging) {
      value = (mx - posX) / this->width;
      value = value < 0.0f ? 0.0f : value;
      value = value > 1.0f ? 1.0f : value;
  }
}


void KroozSlider::Update(float DeltaTime, GLFWwindow* window, double mx, double my){
  this->Window = window;
  UpdateSlider(mx ,my);
}

void KroozSlider::Render(){
  DrawSlider();
}

float KroozSlider::GetValue(){
  return this->value;
}
