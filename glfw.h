// Copyright (C) 2013 Che-Liang Chiou.

#ifndef GLFW_H_
#define GLFW_H_

#include "utils.h"

namespace glfw {

class Library {
 private:
  DISALLOW_COPY_AND_ASSIGN(Library);

 public:
  Library();
  ~Library();
};

}  // namespace glfw

#endif  // GLFW_H_
