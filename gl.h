// Copyright (C) 2013 Che-Liang Chiou.

#ifndef GL_H_
#define GL_H_

#include <GL/glew.h>

#include "utils.h"

namespace gl {

class Buffers {
 private:
  DISALLOW_COPY_AND_ASSIGN(Buffers);

 public:
  Buffers(GLuint* buffers, GLsizei size);
  ~Buffers();

  const GLuint* buffers() { return buffers_; }
  GLsizei size() { return size_; }

 private:
  GLuint* buffers_;
  GLsizei size_;
};

class VertexArrays {
 private:
  DISALLOW_COPY_AND_ASSIGN(VertexArrays);

 public:
  VertexArrays(GLuint* vertex_arrays, GLsizei size);
  ~VertexArrays();

  const GLuint* vertex_arrays() { return vertex_arrays_; }
  GLsizei size() { return size_; }

 private:
  GLuint* vertex_arrays_;
  GLsizei size_;
};

class Shader {
 private:
  DISALLOW_COPY_AND_ASSIGN(Shader);

 public:
  explicit Shader(GLenum shader_type);
  ~Shader();

  GLuint shader() { return shader_; }

 private:
  GLuint shader_;
};

}  // namespace gl

#endif  // GL_H_
