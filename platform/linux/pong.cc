// Copyright (C) 2013 Che-Liang Chiou.

#include <cmath>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "base/base.h"
#include "graphics/gl.h"
#include "platform/linux/graphics/glfw.h"

void LoadShaders(GLuint index, gl::Program* program);

void Rotate(GLfloat angle, GLfloat* x, GLfloat* y);

int main() {
  glfw::Library library;

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
  glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  if (!glfwOpenWindow(600, 600, 0, 0, 0, 0, 32, 0, GLFW_WINDOW))
    throw Exception("Could not open GLFW window");

  // NOTE: This is absolutely needed for the core profile.
  glewExperimental = true;

  if (glewInit() != GLEW_OK)
    throw Exception("Could not initialize GLEW");

  glfwSetWindowTitle("PONG");
  glfwEnable(GLFW_STICKY_KEYS);

  GLfloat vertex_buffer_data[] = {
      -0.5f * cos(M_PI / 6), -0.5f * sin(M_PI / 6), 0.0f,
      0.5f * cos(M_PI / 6),  -0.5f * sin(M_PI / 6), 0.0f,
      0.0f,                  0.5f,                  0.0f, };
  GLuint vertex_buffer;
  gl::Buffers buffers(&vertex_buffer, 1);

  GLuint vertex_array;
  gl::VertexArrays arrays(&vertex_array, 1);
  glBindVertexArray(vertex_array);

  const GLuint index = 0;
  gl::Program program;
  LoadShaders(index, &program);

  const double angular_velocity = M_PI / 5;
  double last_time = glfwGetTime();
  while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
         glfwGetWindowParam(GLFW_OPENED)) {
    double current_time = glfwGetTime();
    double step = current_time - last_time;
    for (int i = 0; i < 9; i += 3) {
      Rotate(angular_velocity * step,
             vertex_buffer_data + i,
             vertex_buffer_data + i + 1);
    }
    last_time = current_time;

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program.program());
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vertex_buffer_data),
                 vertex_buffer_data,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
    glfwSwapBuffers();
  }

  return 0;
}

void LoadShaders(GLuint index, gl::Program* program) {
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
  vertex_shader.Compile(vertex_shader_codes, 1);

  gl::Shader fragment_shader(GL_FRAGMENT_SHADER);
  std::stringstream fragment_shader_code;
  fragment_shader_code << "# version 330 core\n"
                       << "out vec3 color;"
                       << "void main() {"
                       << "color = vec3(1, 1, 1);"
                       << "}";
  std::string fragment_shader_code_str = fragment_shader_code.str();
  const char* fragment_shader_codes[] = {fragment_shader_code_str.c_str()};
  fragment_shader.Compile(fragment_shader_codes, 1);

  program->Attach(vertex_shader);
  program->Attach(fragment_shader);
  program->Link();
}

void Rotate(GLfloat angle, GLfloat* x, GLfloat* y) {
  GLfloat cos_angle = cos(angle);
  GLfloat sin_angle = sin(angle);
  GLfloat x_new = *x * cos_angle - *y * sin_angle;
  GLfloat y_new = *x * sin_angle + *y * cos_angle;
  *x = x_new;
  *y = y_new;
}
