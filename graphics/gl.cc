// Copyright (C) 2013 Che-Liang Chiou.

#include "graphics/gl.h"

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

void Shader::Compile(const GLchar* codes[], GLsizei count) {
  glShaderSource(shader_, count, codes, NULL);
  glCompileShader(shader_);

  GLint status = GL_FALSE;
  glGetShaderiv(shader_, GL_COMPILE_STATUS, &status);
  if (status)
    return;

  GLint length = 0;
  glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &length);
  if (!length) {
    throw Exception("shader: unknown error");
  } else {
    char message[length];
    glGetShaderInfoLog(shader_, length, NULL, message);
    throw Exception(std::string("shader: error: ") + message);
  }
}

//// class Program

Program::Program() { program_ = glCreateProgram(); }

Program::~Program() { glDeleteProgram(program_); }

void Program::Attach(const Shader& shader) {
  glAttachShader(program_, shader.shader());
}

void Program::Link() {
  glLinkProgram(program_);

  GLint status = GL_FALSE;
  glGetProgramiv(program_, GL_LINK_STATUS, &status);
  if (status)
    return;

  GLint length = 0;
  glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &length);
  if (!length) {
    throw Exception("program: unknown error");
  } else {
    char message[length];
    glGetProgramInfoLog(program_, length, NULL, message);
    throw Exception(std::string("program: error: ") + message);
  }
}
