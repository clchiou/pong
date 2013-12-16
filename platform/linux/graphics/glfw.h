// Copyright (C) 2013 Che-Liang Chiou.

#ifndef PLATFORM_LINUX_GRAPHICS_GLFW_H_
#define PLATFORM_LINUX_GRAPHICS_GLFW_H_

#include "base/base.h"

namespace glfw {

class Library {
 private:
  DISALLOW_COPY_AND_ASSIGN(Library);

 public:
  Library();
  ~Library();
};

}  // namespace glfw

#endif  // PLATFORM_LINUX_GRAPHICS_GLFW_H_
