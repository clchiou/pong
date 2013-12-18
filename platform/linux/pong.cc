// Copyright (C) 2013 Che-Liang Chiou.

#include <cmath>
#include <iostream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glfw.h>

#include "base/base.h"
#include "graphics/gl.h"
#include "platform/linux/graphics/glfw.h"
#include "third_party/box2d/Box2D/Box2D.h"

const float WIDTH = 10.0f;
const float HEIGHT = 10.0f;

class Rectangle {
 private:
  DISALLOW_COPY_AND_ASSIGN(Rectangle);

 public:
  Rectangle(float x, float y, float w, float h);

  float x() const { return x_; }
  float y() const { return y_; }
  float w() const { return w_; }
  float h() const { return h_; }

  void SetPosition(float x, float y);

  void CreateBuffer(GLuint index);
  void Draw();

 private:
  static const int NUM_BUFFERS = 2;
  static const int NUM_POINTS = 4;
  static const int NUM_TRIANGLES = 2;

  void ComputeVertices();

  float x_, y_, w_, h_;

  GLuint buffer_ids_[NUM_BUFFERS];
  gl::Buffers buffers_;

  GLfloat vertices_[NUM_POINTS * 3];
  GLubyte indices_[NUM_TRIANGLES * 3];
};

class World {
 private:
  DISALLOW_COPY_AND_ASSIGN(World);

 public:
  World(const Rectangle& body, const Rectangle& ground);

  void Step();

  void GetPosition(float* x, float* y);

 private:
  static constexpr float STEP = 1.0f / 60.0f;
  static const int NUM_VELOCITY_ITERATIONS = 6;
  static const int NUM_POSITION_ITERATIONS = 2;

  b2World world_;
  b2Body* body_;
};

void LoadShaders(GLuint index, gl::Program* program);

void Rotate(GLfloat angle, GLfloat* x, GLfloat* y);

float Scale(float v, float max_v);

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

  GLuint vertex_array;
  gl::VertexArrays arrays(&vertex_array, 1);
  glBindVertexArray(vertex_array);

  const GLuint index = 0;
  gl::Program program;
  LoadShaders(index, &program);

  Rectangle body(0, 8, 1, 1);
  Rectangle ground(0, -10, 50, 10);
  World world(body, ground);

  const double step_time = 1.0 / 60.0;

  double start_time, last_time;
  int num_frames;

  start_time = last_time = glfwGetTime();
  num_frames = 0;
  while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS &&
         glfwGetWindowParam(GLFW_OPENED)) {
    double current_time = glfwGetTime();
    if (current_time >= last_time + step_time) {
      float x = body.x(), y = body.y();
      world.Step();
      world.GetPosition(&x, &y);
      body.SetPosition(x, y);
      last_time = current_time;
    }

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program.program());

    body.CreateBuffer(index);
    body.Draw();

    ground.CreateBuffer(index);
    ground.Draw();

    glfwSwapBuffers();

    num_frames++;
  }

  double end_time = glfwGetTime();
  std::cout << "seconds/frame = " << (end_time - start_time) / num_frames
            << std::endl;

  glDisableVertexAttribArray(index);

  return 0;
}

void LoadShaders(GLuint index, gl::Program* program) {
  gl::Shader vertex_shader(GL_VERTEX_SHADER);
  std::stringstream vertex_shader_code;
  vertex_shader_code << "# version 330 core\n"
                     << "layout(location = " << index << ") in vec3 position;"
                     << "void main() {"
                     << "gl_Position.xyz = position;"
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

float Scale(float v, float max_v) {
  return std::copysign(std::min(1.0f, std::abs(v) / max_v), v);
}

Rectangle::Rectangle(float x, float y, float w, float h)
    : x_(x), y_(y), w_(w), h_(h), buffers_(buffer_ids_, NUM_BUFFERS) {
  ComputeVertices();

  vertices_[2] = 0.0f;
  vertices_[5] = 0.0f;
  vertices_[8] = 0.0f;
  vertices_[11] = 0.0f;

  indices_[0] = 0;
  indices_[1] = 1;
  indices_[2] = 2;

  indices_[3] = 2;
  indices_[4] = 3;
  indices_[5] = 0;
}

void Rectangle::SetPosition(float x, float y) {
  x_ = x;
  y_ = y;
  ComputeVertices();
}

void Rectangle::ComputeVertices() {
  float x = Scale(x_, WIDTH);
  float w = Scale(w_, WIDTH);
  float y = Scale(y_, HEIGHT);
  float h = Scale(h_, HEIGHT);

  vertices_[0] = x - w;
  vertices_[1] = y + h;

  vertices_[3] = x - w;
  vertices_[4] = y - h;

  vertices_[6] = x + w;
  vertices_[7] = y - h;

  vertices_[9] = x + w;
  vertices_[10] = y + h;
}

void Rectangle::CreateBuffer(GLuint index) {
  glBindBuffer(GL_ARRAY_BUFFER, buffer_ids_[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_), vertices_, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, NULL);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_ids_[1]);
  glBufferData(
      GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_), indices_, GL_DYNAMIC_DRAW);
}

void Rectangle::Draw() {
  glDrawElements(GL_TRIANGLES, NUM_TRIANGLES * 3, GL_UNSIGNED_BYTE, NULL);
}

World::World(const Rectangle& body, const Rectangle& ground)
    : world_(b2World(b2Vec2(0.0f, -10.0f))) {
  b2BodyDef ground_body_definition;
  ground_body_definition.position.Set(ground.x(), ground.y());
  b2Body* ground_body = world_.CreateBody(&ground_body_definition);

  b2PolygonShape ground_box;
  ground_box.SetAsBox(ground.w(), ground.h());
  ground_body->CreateFixture(&ground_box, 0.0f);

  b2BodyDef body_definition;
  body_definition.type = b2_dynamicBody;
  body_definition.position.Set(body.x(), body.y());
  body_ = world_.CreateBody(&body_definition);

  b2PolygonShape body_box;
  body_box.SetAsBox(body.w(), body.h());
  b2FixtureDef fixture_definition;
  fixture_definition.shape = &body_box;
  fixture_definition.density = 1.0f;
  fixture_definition.friction = 0.3f;
  body_->CreateFixture(&fixture_definition);
}

void World::Step() {
  world_.Step(STEP, NUM_VELOCITY_ITERATIONS, NUM_POSITION_ITERATIONS);
}

void World::GetPosition(float* x, float* y) {
  b2Vec2 position = body_->GetPosition();
  *x = position.x;
  *y = position.y;
}
