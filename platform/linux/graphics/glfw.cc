// Copyright (C) 2013 Che-Liang Chiou.

#include "platform/linux/graphics/glfw.h"

#include <GL/glfw.h>

using namespace glfw;

//// class Library

Library::Library() {
  if (!glfwInit())
    throw Exception("Could not initialize GLFW");
}

Library::~Library() { glfwTerminate(); }
