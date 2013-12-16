// Copyright (C) 2013 Che-Liang Chiou.

#include <iostream>

#include "third_party/box2d/Box2D/Box2D.h"

int main() {
  b2Vec2 gravity(0.0f, -10.0f);
  b2World world(gravity);

  b2BodyDef ground_body_definition;
  ground_body_definition.position.Set(0.0, -10.0f);
  b2Body* ground_body = world.CreateBody(&ground_body_definition);

  b2PolygonShape ground_box;
  ground_box.SetAsBox(50.0f, 10.0f);
  ground_body->CreateFixture(&ground_box, 0.0f);

  b2BodyDef body_definition;
  body_definition.type = b2_dynamicBody;
  body_definition.position.Set(0.0f, 4.0f);
  b2Body* body = world.CreateBody(&body_definition);

  b2PolygonShape body_box;
  body_box.SetAsBox(1.0f, 1.0f);
  b2FixtureDef fixture_definition;
  fixture_definition.shape = &body_box;
  fixture_definition.density = 1.0f;
  fixture_definition.friction = 0.3f;
  body->CreateFixture(&fixture_definition);

  const float step = 1.0f / 60.0f;
  const int num_velocity_iterations = 6;
  const int num_position_iterations = 2;

  std::cout << std::scientific;
  int count = 60;
  while (count-- > 0) {
    world.Step(step, num_velocity_iterations, num_position_iterations);
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();
    std::cout << "x=" << position.x << " y=" << position.y << " w=" << angle
              << std::endl;
  }

  return 0;
}
