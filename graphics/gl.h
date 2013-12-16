// Copyright (C) 2013 Che-Liang Chiou.

#ifndef GRAPHICS_GL_H_
#define GRAPHICS_GL_H_

// TODO(clchiou): Why couldn't we use <GL/gl.h> here?
#include <GL/glew.h>

#include "base/base.h"

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

  GLuint shader() const { return shader_; }

  void Compile(const GLchar* codes[], GLsizei count);

 private:
  GLuint shader_;
};

class Program {
 private:
  DISALLOW_COPY_AND_ASSIGN(Program);

 public:
  Program();
  ~Program();

  GLuint program() const { return program_; }

  void Attach(const Shader& shader);
  void Link();

 private:
  GLuint program_;
};

}  // namespace gl

#endif  // GRAPHICS_GL_H_
