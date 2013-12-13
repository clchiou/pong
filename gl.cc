// Copyright (C) 2013 Che-Liang Chiou.

#include "gl.h"

using namespace gl;

//// class Buffers

Buffers::Buffers(GLuint* buffers, GLsizei size)
    : buffers_(buffers), size_(size) {
  glGenBuffers(size_, buffers_);
}

Buffers::~Buffers() { glDeleteBuffers(size_, buffers_); }

//// class VertexArrays

VertexArrays::VertexArrays(GLuint* vertex_arrays, GLsizei size)
    : vertex_arrays_(vertex_arrays), size_(size) {
  glGenVertexArrays(size_, vertex_arrays_);
}

VertexArrays::~VertexArrays() { glDeleteVertexArrays(size_, vertex_arrays_); }

//// class Shader

Shader::Shader(GLenum shader_type) {
  shader_ = glCreateShader(shader_type);
  if (!shader_)
    throw Exception("Could not create shader");
}

Shader::~Shader() { glDeleteShader(shader_); }
