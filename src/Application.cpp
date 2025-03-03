#include "Application.hh"

void Application::addComponents(std::unique_ptr<KObject> comp){
  m_Components.push_back(std::move(comp));
}

void Application::RenderComponents(){
  for(auto &comps : m_Components){
    comps->Render();
  }
}

void Application::UpdateComponents(float Delta, GLFWwindow* window , double mx , double my){
  for(auto &comps : m_Components){
    comps->Update( Delta, window ,mx , my );
  }
}
