#include <Button.hh>

void KroozButton::DrawButton(GLFWwindow* window , const char* label, int x ,int y ,int width , int height){
  this->btnX = x;
  this->btnY = y;
  this->btnWidth = width;
  this->btnHeight = height;
  this->Label = label;
  this->Window = window;

  Rect BtnRect = { x,y,width,height,0.3f, 0.3f , 0.3f};
  this->Button = BtnRect;
}

bool KroozButton::isHovering( double mx , double my){
  int width, height;
  glfwGetWindowSize(this->Window, &width, &height);
  bool hovering = (mx >= btnX && mx <= btnX + btnWidth &&
  my >= height - btnY - btnHeight && my <= height - btnY);
  if (hovering) {
    Button.r = 0.5f;
    Button.g = 0.5f;
    Button.b = 0.5f;
  }
  else {
    Button.r = 0.3f;
    Button.g = 0.3f;
    Button.b = 0.3f;
  }

  return hovering;
}

bool KroozButton::isClicked(double mx , double my){
  bool IsHovering = isHovering(mx,my);
  bool isPressed = glfwGetMouseButton(this->Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

  if (IsHovering && isPressed && !m_wasPressed) {
    m_wasPressed = true;
    m_onClick();
    return true;
  }

  if (!isPressed) m_wasPressed = false;
  return false;
}

void KroozButton::setOnClick(std::function<void()> callback){
  m_onClick = callback;
}

void KroozButton::Update(float DeltaTime, GLFWwindow* window, double mx, double my){
  this->Window = window;
  isClicked(mx ,my);
}

void KroozButton::Render(){
  Shape::DrawRect(Button);
  fnt->loadDefaultFont();
  fnt->SetFontSize(std::min(this->btnWidth, this->btnHeight) * 0.55f);

  float textWidth = fnt->getTextWidth(this->Label);
  float textHeight = fnt->getTextHeight();

  float centeredX = this->btnX + (this->btnWidth - textWidth) / 2;
  float centeredY = this->btnY + (this->btnHeight - textHeight) / 2;
  fnt->SetColor(.5f , .5f , .5f);
  fnt->renderText(this->Label, centeredX, centeredY);
}
