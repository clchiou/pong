// Copyright (C) 2013 Che-Liang Chiou.

#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glfw.h>

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

class Exception : public std::exception {
 public:
  explicit Exception(std::string message) : message_(std::move(message)) {}

  virtual ~Exception() noexcept(true) {}

  virtual const char* what() const throw() override { return message_.c_str(); }

 private:
  std::string message_;
};

namespace glfw {

class Library {
 private:
  DISALLOW_COPY_AND_ASSIGN(Library);

 public:
  Library();
  ~Library();
};

}  // namespace glfw

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

void LoadShaders(GLuint index, GLuint* program_out);

int main() {
  glfw::Library library;

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!glfwOpenWindow(1024, 768, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
    throw Exception("Could not open GLFW window");

  // NOTE: This is absolutely needed for the core profile.
  glewExperimental = true;

  if (glewInit() != GLEW_OK)
    throw Exception("Could not initialize GLEW");

  glfwSetWindowTitle("PONG");
  glfwEnable(GLFW_STICKY_KEYS);

  GLuint vertex_buffer;
  gl::Buffers buffers(&vertex_buffer, 1);

  const GLfloat vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
  };
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(vertex_buffer_data),
               vertex_buffer_data,
               GL_STATIC_DRAW);

  GLuint vertex_array;
  gl::VertexArrays arrays(&vertex_array, 1);

  glBindVertexArray(vertex_array);

  const GLuint index = 0;
  GLuint program;
  LoadShaders(index, &program);

  while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
         glfwGetWindowParam(GLFW_OPENED)) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDisableVertexAttribArray(0);

    glfwSwapBuffers();
  }

  return 0;
}

void LoadShaders(GLuint index, GLuint* program_out) {
  GLint status = GL_FALSE, length = 0;

  gl::Shader vertex_shader(GL_VERTEX_SHADER);
  std::stringstream vertex_shader_code;
  vertex_shader_code << "# version 330 core\n"
                     << "layout(location = " << index
                     << ") in vec3 vertexPosition_modelspace;"
                     << "void main() {"
                     << "gl_Position.xyz = vertexPosition_modelspace;"
                     << "gl_Position.w = 1.0;"
                     << "}";
  std::string vertex_shader_code_str = vertex_shader_code.str();
  const char* vertex_shader_codes[] = {vertex_shader_code_str.c_str()};
  glShaderSource(vertex_shader.shader(),
                 sizeof(vertex_shader_codes) / sizeof(*vertex_shader_codes),
                 vertex_shader_codes,
                 NULL);
  glCompileShader(vertex_shader.shader());

  glGetShaderiv(vertex_shader.shader(), GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderiv(vertex_shader.shader(), GL_INFO_LOG_LENGTH, &length);
    if (!length) {
      throw Exception("vertex_shader: unknown error");
    }
    char message[length];
    glGetShaderInfoLog(vertex_shader.shader(), length, NULL, message);
    throw Exception(std::string("vertex_shader: error: ") + message);
  }

  gl::Shader fragment_shader(GL_FRAGMENT_SHADER);
  std::stringstream fragment_shader_code;
  fragment_shader_code << "# version 330 core\n"
                       << "out vec3 color;"
                       << "void main() {"
                       << "color = vec3(1, 1, 1);"
                       << "}";
  std::string fragment_shader_code_str = fragment_shader_code.str();
  const char* fragment_shader_codes[] = {fragment_shader_code_str.c_str()};
  glShaderSource(fragment_shader.shader(),
                 sizeof(fragment_shader_codes) / sizeof(*fragment_shader_codes),
                 fragment_shader_codes,
                 NULL);
  glCompileShader(fragment_shader.shader());

  glGetShaderiv(fragment_shader.shader(), GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderiv(fragment_shader.shader(), GL_INFO_LOG_LENGTH, &length);
    if (!length) {
      throw Exception("fragment_shader: unknown error");
    }
    char message[length];
    glGetShaderInfoLog(fragment_shader.shader(), length, NULL, message);
    throw Exception(std::string("fragment_shader: error: ") + message);
  }

  GLuint program = glCreateProgram();
  glAttachShader(program, vertex_shader.shader());
  glAttachShader(program, fragment_shader.shader());
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    if (!length) {
      throw Exception("program: unknown error");
    }
    char message[length];
    glGetProgramInfoLog(program, length, NULL, message);
    throw Exception(std::string("program: error: ") + message);
  }

  *program_out = program;
}

namespace glfw {

//// class Library

Library::Library() {
  if (!glfwInit())
    throw Exception("Could not initialize GLFW");
}

Library::~Library() { glfwTerminate(); }

}  // namespace glfw

namespace gl {

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

}  // namespace gl
