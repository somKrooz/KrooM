#define PY_SSIZE_T_CLEAN
#define STB_TRUETYPE_IMPLEMENTATION
#include <Python.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Application.hh"
#include "Button.hh"
#include "Slider.hh"
#include "Text.hh"
#include "Input.hh"

GLFWwindow* window;
Application App;
std::vector<PyObject*> GlobalUpdate;


void UpdateTrigger(std::vector<PyObject*> Updator){
  for(auto &up : Updator){
    PyObject* result = PyObject_CallObject(up, nullptr);
    if (result == nullptr) PyErr_Print();
    Py_XDECREF(result);
  }
}


// Init
static PyObject* Krooz_Init(PyObject* self, PyObject* args) {
    const char* title;
    int width , height;
    if (!PyArg_ParseTuple(args, "sii", &title , &width , &height)) {
        return nullptr;
    }

    if (!glfwInit()) {
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize GLFW");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);

    if (!window) {
        glfwTerminate();
        PyErr_SetString(PyExc_RuntimeError, "Failed to create window");
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        PyErr_SetString(PyExc_RuntimeError, "Failed to initialize GLEW");
        return nullptr;
    }

    glfwSetWindowAttrib(window, GLFW_RESIZABLE,false);
    glViewport(0, 0, 940, 640);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 940, 0, 640, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Py_RETURN_NONE;
}

// Window
static PyObject* Krooz_Window(PyObject* self, PyObject* args) {
  float before = glfwGetTime();
  double mx, my;

  while (!glfwWindowShouldClose(window)) {
      glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwGetCursorPos(window, &mx, &my);

      float currentTime = glfwGetTime();
      float DeltaTime = currentTime - before;
      before = currentTime;

      App.UpdateComponents(DeltaTime, window, mx, my);
      App.RenderComponents();

      UpdateTrigger(GlobalUpdate);
      glfwSwapBuffers(window);
      glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  Py_RETURN_NONE;
}

//Updator
static PyObject* Krooz_Update(PyObject* self, PyObject* args){
  PyObject* updateFunction;
  if(!PyArg_ParseTuple(args , "O" ,&updateFunction )) return nullptr;
  if(!PyCallable_Check(updateFunction)) return nullptr;
  GlobalUpdate.push_back(updateFunction);


  Py_RETURN_NONE;
}


static PyObject* Krooz_Input(PyObject* self, PyObject* args) {
  float x, y, width, height;
  if (!PyArg_ParseTuple(args, "ffff", &x, &y, &width, &height))
      return nullptr;
  extern GLFWwindow* window;

  if (!window) {
      PyErr_SetString(PyExc_RuntimeError, "GLFW window is not initialized");
      return nullptr;
  }

  std::unique_ptr<KroozInput> Input = std::make_unique<KroozInput>(window, x, y, width, height);
  PyObject* KroozInputCapsule = PyCapsule_New(Input.get(), "KroozInput", nullptr);
  if(!KroozInputCapsule) return nullptr;
  App.addComponents(std::move(Input));

  return KroozInputCapsule;
}

static PyObject* Krooz_InputValue(PyObject* self, PyObject* args) {
  PyObject* InputObj;
  if(!PyArg_ParseTuple(args ,"O", &InputObj )) return nullptr;
  KroozInput* input = reinterpret_cast<KroozInput*>(PyCapsule_GetPointer(InputObj, "KroozInput"));
  if(!input) return nullptr;

  std::string Val = input->getText();
  return PyUnicode_FromString(Val.c_str());

}

static PyObject* Krooz_Text(PyObject* self, PyObject* args){
  const char* text;
  float x;
  float y;

  if(!PyArg_ParseTuple(args , "sff" , &text , &x, &y)){
    return nullptr;
  }

  std::unique_ptr<KroozFont> font = std::make_unique<KroozFont>();
  font->loadDefaultFont();
  font->renderText(text , x , y);
  Py_RETURN_NONE;
}


// Krooz Button
static PyObject* Krooz_Button(PyObject* self, PyObject* args) {
    int x, y, width, height;
    const char* text;

    if (!PyArg_ParseTuple(args, "siiii", &text , &x, &y, &width, &height)) {
        return nullptr;
    }

    std::unique_ptr<KroozButton> btn = std::make_unique<KroozButton>();
    btn->DrawButton(window , text, x, y, width, height);

    PyObject* KroozBtnCapsule = PyCapsule_New(btn.get(), "KroozButton", nullptr);
    if (!KroozBtnCapsule) {
      return nullptr;
    }

    App.addComponents(std::move(btn));
    return KroozBtnCapsule;
}

static PyObject* Krooz_SetButtonCallback(PyObject* self, PyObject* args) {
  PyObject* btnObj;
  PyObject* callback;

  if (!PyArg_ParseTuple(args, "OO", &btnObj, &callback)) {
      return nullptr;
  }


  KroozButton* btn = reinterpret_cast<KroozButton*>(PyCapsule_GetPointer(btnObj, "KroozButton"));
  if (!btn) return nullptr;
  if(!PyCallable_Check(callback)) return nullptr;
  Py_INCREF(callback);

  std::function<void()> onClick = [callback]() {
    PyGILState_STATE gstate = PyGILState_Ensure();
    PyObject* result = PyObject_CallObject(callback, nullptr);
    if (result == nullptr) PyErr_Print();
    Py_XDECREF(result);
    PyGILState_Release(gstate);
  };

  btn->setOnClick(onClick);
  Py_RETURN_NONE;
}


// Krooz Slider
static PyObject* Krooz_Slider(PyObject* self, PyObject* args){
  int x;
  int y;
  int w;
  PyObject* Capsule;

  if(!PyArg_ParseTuple(args , "iii" ,&x ,&y ,&w)){
    return nullptr;
  }

  auto Slider = std::make_unique<KroozSlider>(window,x, y, w);

  KroozSlider* sliderRawPtr = Slider.get();
  App.addComponents(std::move(Slider));
  return PyCapsule_New(sliderRawPtr, "KroozSlider", nullptr);

}

static PyObject* Krooz_SliderValue(PyObject* self, PyObject* args) {
  PyObject* sliderObj;

  if (!PyArg_ParseTuple(args, "O", &sliderObj)) {
      return nullptr;
  }

  KroozSlider* slider = reinterpret_cast<KroozSlider*>(PyCapsule_GetPointer(sliderObj, "KroozSlider"));
  if (!slider) return nullptr;
  float value = slider->GetValue();
  return PyFloat_FromDouble(value);
}

static PyMethodDef KroozMethod[] = {
    {"Init", Krooz_Init, METH_VARARGS, "Initialize the window"},
    {"RunWindow", Krooz_Window, METH_VARARGS, "Start the window loop"},

    {"CreateButton", Krooz_Button, METH_VARARGS, "Create a button"},
    {"setCallback", Krooz_SetButtonCallback, METH_VARARGS, "Set Button Callback"},

    {"CreateSlider", Krooz_Slider, METH_VARARGS, "Create a Slider"},
    {"Getvalue",  Krooz_SliderValue , METH_VARARGS , "Get Slider Value"},
    {"Updator" ,  Krooz_Update , METH_VARARGS , "Update Values"},

    {"CreateText" ,Krooz_Text , METH_VARARGS , "Create texts"},
    {"CreateInput" , Krooz_Input ,METH_VARARGS , "Create Input" },
    {"GetTex" , Krooz_InputValue , METH_VARARGS , "Get The Text"},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef KrooM_module = {
    PyModuleDef_HEAD_INIT,
    "KrooM",
    "UI Library For Som Krooz",
    -1,
    KroozMethod,
    NULL, NULL, NULL, NULL
};

PyMODINIT_FUNC PyInit_KrooM(void) {
    return PyModule_Create(&KrooM_module);
}
