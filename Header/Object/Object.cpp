#include "Object.h"
#include "Header/Math/Lerp.h"
#include "Header/Render/Render.hpp"
#include <algorithm>

Triangle::Triangle(const Vector3 Vertices[3]) : vertices_{Vertices[0], Vertices[1], Vertices[2]} {}

Triangle::~Triangle() {}

ModelClass::ModelClass() {}

ModelClass::~ModelClass() {}

Object3d::Object3d(const Transform& transform, ModelClass* model)
    : transform_(transform), model_(model) {}

Object3d::~Object3d() {}

Camera::Camera(const Matrix4x4& projectonMatrix)
    : transform_{
          {1.f, 1.f, 1.f},
          {0.f, 0.f, 0.f},
          {0.f, 0.f, 0.f}
} {
	viewMatrix_ = Matrix4x4::Identity();
	projectonMatrix_ = projectonMatrix;
}

Camera::~Camera() {}

const Matrix4x4 Camera::wvVpMatrix(const Object3d& object) const {
	return object.worldMatrix_ * viewMatrix_ * projectonMatrix_;
}

void Camera::CreateNDC(const Object3d& object, Render* render) const {
	const Matrix4x4 wvVpMatrix = Camera::wvVpMatrix(object);
	const Vector3 facing = GetFacing();
	for (const Triangle& localSurface : object.model_->surfaceList_) {
		const Triangle buff = (localSurface * wvVpMatrix);
		if (facing * buff.GetNormal() <= 0) {
			render->ndcSurface_.emplace_back(buff);
		}
	}
}

void Camera::CreateNDC(const std::vector<Object3d>& objectList, Render* render) const {
	for (const Object3d& object : objectList) {
		CreateNDC(object, render);
	}
}

Vector3 LineBase::GetProgress(const float& t) const { return Lerp(origin, GetEnd(), t); }

Vector3 LineBase::Project(const Vector3& point) const {
	return (point - origin) - ClosestProgress(point) * diff;
}

Vector3 LineBase::ClosestPoint(const Vector3& point) const {
	return ClosestProgress(point) * diff + origin;
}

float LineBase::ClosestProgress(const Vector3& point) const {
	return Clamp(((point - origin) * diff) / std::powf(diff.Length(), 2));
}

const float Line::Clamp(const float& t) const { return t; }

const float Ray::Clamp(const float& t) const { return (t > 0.f) ? t : 0.f; }

const float Segment::Clamp(const float& t) const { return std::clamp(t, 0.f, 1.f); }

bool Plane::IsCollision(const LineBase& other) {
	const float dot = normal * other.diff;
	if (dot == 0.f)
		return false;
	const float t = (distance - (other.origin * normal)) / dot;
	const float clampT = other.Clamp(t);
	return clampT == t;
}
