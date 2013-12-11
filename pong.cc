// Copyright (C) 2013 Che-Liang Chiou.

#include <iostream>

#include <GL/glew.h>
#include <GL/glfw.h>

int main() {
  int ret = 1;

  if (!glfwInit()) {
    std::cerr << "Could not initialize GLFW" << std::endl;
    return 1;
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW)) {
    std::cerr << "Could not open GLFW window" << std::endl;
    goto exit;
  }

  if (glewInit() != GLEW_OK) {
    std::cerr << "Could not initialize GLEW" << std::endl;
    goto exit;
  }

  glfwSetWindowTitle("PONG");
  glfwEnable(GLFW_STICKY_KEYS);
  while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
         glfwGetWindowParam(GLFW_OPENED)) {
    glfwSwapBuffers();
  }
  ret = 0;

exit:
  glfwTerminate();
  return ret;
}
