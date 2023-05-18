#include "Object.h"

Triangle::Triangle(const Vector3 Vertices[3]) : vertices_{Vertices[0], Vertices[1], Vertices[2]} {}

Triangle::~Triangle() {}

// Triangle Triangle::GetWorldTriangle(const Matrix4x4& worldMatrix) const { return Triangle{}; }

// Triangle Triangle::GetScreenTriangle() const { return Triangle{}; }


Object::Object(const Transform& transform) : transform_(transform) {}

Object::~Object() {}

Camera::Camera(const Vector3& position, const Vector3& rotate)
    : transform_{
          {1.f, 1.f, 1.f},
          rotate, position
} {}

Camera::~Camera() {}