#include "Object.h"
#include "Header/Math/Lerp.h"
#include "Header/Render/Render.hpp"
#include <algorithm>

#include <imgui.h>

Triangle::Triangle(const Vector3 Vertices[3]) : vertices_{Vertices[0], Vertices[1], Vertices[2]} {}

Triangle::~Triangle() {}

void Triangle::ImGuiDebug() {
	ImGui::DragFloat3("triangleVertex[0]", &vertices_[0].x, 0.1f);
	ImGui::DragFloat3("triangleVertex[1]", &vertices_[1].x, 0.1f);
	ImGui::DragFloat3("triangleVertex[2]", &vertices_[2].x, 0.1f);
}

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

Vector3 LineBase::GetProgress(const float& t) const { return origin + diff * t; }

Vector3 LineBase::Project(const Vector3& point) const {
	return (point - origin) - ClosestProgress(point) * diff;
}

Vector3 LineBase::ClosestPoint(const Vector3& point) const {
	return ClosestProgress(point) * diff + origin;
}

void LineBase::ImGuiDebug(const std::string& group) {
	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Origin", &origin.x, 0.1f);
		ImGui::DragFloat3("Diff", &diff.x, 0.1f);
	}
	ImGui::TreePop();
}

float LineBase::ClosestProgress(const Vector3& point) const {
	return Clamp(((point - origin) * diff) / std::powf(diff.Length(), 2));
}

const float Line::Clamp(const float& t) const { return t; }

const float Ray::Clamp(const float& t) const { return (t > 0.f) ? t : 0.f; }

const float Segment::Clamp(const float& t) const { return std::clamp(t, 0.f, 1.f); }

Plane Plane::Create(const Triangle& trinagle) { return Create(trinagle.vertices_); }

const bool Plane::IsCollision(const LineBase& other) const {
	return Collision::IsHit(other, *this);
}

const bool Collision::IsHit(const LineBase& line, const Plane& plane) {
	const float dot = plane.normal * line.diff;
	if (dot == 0.f)
		return false;
	const float t = (plane.distance - (line.origin * plane.normal)) / dot;
	const float clampT = line.Clamp(t);
	return clampT == t;
}

const bool Collision::IsHit(const Sphere& sphereA, const Sphere& sphereB) {
	return (sphereA.center - sphereB.center).Length() <= sphereA.radius + sphereB.radius;
}

const bool Collision::IsHit(const Sphere& sphere, const Plane& plane) {
	return std::abs(plane.GetDistance(sphere.center)) <= sphere.radius;
}

const bool Collision::IsHit(const LineBase& line, const Triangle& triangle) {
	Plane plane = Plane::Create(triangle);
	if (!IsHit(line, plane))
		return false;
	const Vector3 pos = HitPoint(line, plane);
	const Vector3 closs[3] = {
	    {(triangle.vertices_[1] - triangle.vertices_[0]) ^ (pos - triangle.vertices_[1])},
	    {(triangle.vertices_[2] - triangle.vertices_[1]) ^ (pos - triangle.vertices_[2])},
	    {(triangle.vertices_[0] - triangle.vertices_[2]) ^ (pos - triangle.vertices_[0])},
	};
	const Vector3& normal = triangle.GetNormal();
	return ((normal * closs[0]) >= 0.f && (normal * closs[1]) >= 0.f && (normal * closs[2]) >= 0.f);
}

const bool Collision::IsHit(const AABB& a, const AABB& b) {
	return (
	    (a.min.x <= b.max.x && a.max.x >= b.min.x) && (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
	    (a.min.z <= b.max.z && a.max.z >= b.min.z));
}

const bool Collision::IsHit(const AABB& aabb, const Sphere& sphere) {
	Vector3 clampPos{
	    std::clamp(sphere.center.x, aabb.min.x, aabb.max.x),
	    std::clamp(sphere.center.y, aabb.min.y, aabb.max.y),
	    std::clamp(sphere.center.z, aabb.min.z, aabb.max.z),
	};
	return ((clampPos - sphere.center).Length() <= sphere.radius);
}

const bool Collision::IsHit(const AABB&, const LineBase&) { return false; }

const Vector3 Collision::HitPoint(const LineBase& line, const Plane& plane) {
	const float dot = plane.normal * line.diff;
	if (dot == 0.f)
		return Vector3::zero(); // 平行
	const float t = (plane.distance - (line.origin * plane.normal)) / dot;
	return line.GetProgress(t);
}

void AABB::ImGuiDebug(const std::string& group) {
	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Min", &min.x, 0.1f);
		ImGui::DragFloat3("Max", &max.x, 0.1f);
		Swaping();

		ImGui::TreePop();
	}
}

void AABB::Swaping() {
	if (min.x > max.x) {
		std::swap(min.x, max.x);
	}
	if (min.y > max.y) {
		std::swap(min.y, max.y);
	}
	if (min.z > max.z) {
		std::swap(min.z, max.z);
	}
}

void Sphere::ImGuiDebug(const std::string& group) {

	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Centor", &center.x, 0.1f);
		ImGui::DragFloat("Radius", &radius, 0.1f);

		ImGui::TreePop();
	}
}
