#include <pbCntx.h>

namespace pbCntx {
  pbGLFW::window::window(int rend, std::string winName, int winWidth, int winHeight) {
    if (rend == VULKAN13) {
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
      win = glfwCreateWindow(winWidth, winHeight, winName.c_str(), nullptr, nullptr);

    }
    else {
      //Update for OPENGL
      glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    }//Else

  } //window

  void pbGLFW::winResizeF(GLFWwindow* win, int winWidth, int winHeight) {
    auto glfw = reinterpret_cast<pbGLFW*>(glfwGetWindowUserPointer(win));
    glfw->winResizeB = true;
  };

  pbGLFW::pbGLFW() {
    glfwInit();
  } //GLFW

  void pbGLFW::createWindow(int rend, std::string winName = "", int winWidth, int winHeight = 1) {
    window* win = &window(rend, winName, winWidth, winHeight);
    winVec.emplace_back(win);

    glfwSetWindowUserPointer(win->win, this);

    //Utiity
    glfwSetFramebufferSizeCallback(win->win, winResizeF);

  } //Create WIndow


  pbCntx::pbCntx() {
    switch (rend) {
    case VULKAN13:
    case OPENGL46:
    case OPENGL45:
    case OPENGL44:
    case OPENGL43:
    case OPENGL42:
    case OPENGL41:
    case OPENGL40:
    case OPENGL30:
    case OPENGL20:
      r->glfw = new pbGLFW();
      r->glfw->createWindow(rend);
    }
  }; //PbCntx Construct

}; //pbCntx Namespace 