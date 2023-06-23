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
		ImGui::TreePop();
	}
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
	return (sphereA.centor - sphereB.centor).Length() <= sphereA.radius + sphereB.radius;
}

const bool Collision::IsHit(const Sphere& sphere, const Plane& plane) {
	return std::abs(plane.GetDistance(sphere.centor)) <= sphere.radius;
}

const bool Collision::IsHit(const LineBase& line, const Triangle& triangle) {
	Plane plane = Plane::Create(triangle);
	if (!IsHit(line, plane))
		return false;
	const Vector3 pos = HitPoint(line, plane);
	const Vector3 closs[3] = {
	    {(triangle.vertices_[1] - triangle.vertices_[0]).cross(pos - triangle.vertices_[1])},
	    {(triangle.vertices_[2] - triangle.vertices_[1]).cross(pos - triangle.vertices_[2])},
	    {(triangle.vertices_[0] - triangle.vertices_[2]).cross(pos - triangle.vertices_[0])},
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
	    std::clamp(sphere.centor.x, aabb.min.x, aabb.max.x),
	    std::clamp(sphere.centor.y, aabb.min.y, aabb.max.y),
	    std::clamp(sphere.centor.z, aabb.min.z, aabb.max.z),
	};
	return ((clampPos - sphere.centor).Length() <= sphere.radius);
}

const bool Collision::IsHit(const AABB& aabb, const LineBase& line) {

	return IsHit(aabb, line, line);
}

const bool Collision::IsHit(const AABB& aabb, const LineBase& line, const LineBase& clampF) {
	const Vector3 tMinVec{
	    {(aabb.min.x - line.origin.x) / line.diff.x},
	    {(aabb.min.y - line.origin.y) / line.diff.y},
	    {(aabb.min.z - line.origin.z) / line.diff.z}};
	const Vector3 tMaxVec{
	    {(aabb.max.x - line.origin.x) / line.diff.x},
	    {(aabb.max.y - line.origin.y) / line.diff.y},
	    {(aabb.max.z - line.origin.z) / line.diff.z}};

	const Vector3 tNear{
	    min(tMinVec.x, tMaxVec.x), min(tMinVec.y, tMaxVec.y), min(tMinVec.z, tMaxVec.z)};
	const Vector3 tFar{
	    max(tMinVec.x, tMaxVec.x), max(tMinVec.y, tMaxVec.y), max(tMinVec.z, tMaxVec.z)};

	const float tMin{max(max(tNear.x, tNear.y), tNear.z)};
	const float tMax{min(min(tFar.x, tFar.y), tFar.z)};
	if (tMin > 1.f && tMin != clampF.Clamp(tMin))
		return false;
	if (tMax < 0.f && tMax != clampF.Clamp(tMax))
		return false;
	return tMin <= tMax;
}

const bool Collision::IsHit(const OBB& obb, const Sphere& sphere) {
	Sphere localSphere = {
	    .centor = sphere.centor * obb.GetInverseMatrix(), .radius = sphere.radius};
	AABB localOBB{.min = -obb.size, .max = obb.size};
	return IsHit(localOBB, localSphere);
}

const bool Collision::IsHit(const OBB& obb, const LineBase& line) {
	AABB localOBB{.min = -obb.size, .max = obb.size};

	const Vector3& localOrigin = line.origin * obb.GetInverseMatrix();
	const Vector3& localEnd = line.GetEnd() * obb.GetInverseMatrix();

	auto localLine = Segment{localOrigin, localEnd - localOrigin};

	return IsHit(localOBB, localLine, line);
}

const Vector3 Collision::HitPoint(const LineBase& line, const Plane& plane) {
	const float dot = plane.normal * line.diff;
	if (dot == 0.f)
		return Vector3::zero(); // 平行
	const float t = (plane.distance - (line.origin * plane.normal)) / dot;
	return line.GetProgress(t);
}

void AABB::ImGuiDebug(const std::string& group) {
	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Max", &max.x, 0.1f);
		ImGui::DragFloat3("Min", &min.x, 0.1f);
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

		ImGui::DragFloat3("Centor", &centor.x, 0.1f);
		ImGui::DragFloat("Radius", &radius, 0.1f);

		ImGui::TreePop();
	}
}

const Matrix4x4 OBB::GetWorldMatrix() const {
	return Matrix4x4{
	    {orientations[0].x, orientations[0].y, orientations[0].z, 0.f},
	    {orientations[1].x, orientations[1].y, orientations[1].z, 0.f},
	    {orientations[2].x, orientations[2].y, orientations[2].z, 0.f},
	    {centor.x,          centor.y,          centor.z,          1.f}
    };
}

const Matrix4x4 OBB::GetInverseMatrix() const { return GetWorldMatrix().InverseRT(); }

void OBB::ImGuiDebug(const std::string& group, Vector3& rotate) {

	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Centor", &centor.x, 0.1f);
		ImGui::DragFloat3("Size", &size.x, 0.1f);

		if (ImGui::DragFloat3("Rotate", &rotate.x, Angle::Dig2Rad)) {
			SetRotate(rotate);
		}

		ImGui::TreePop();
	}
}

void OBB::SetRotate(const Vector3& euler) {
	const Matrix4x4& rotateMat = Matrix4x4::EulerRotate(euler);
	std::memcpy(&orientations[0], &rotateMat.m[0], sizeof(Vector3));
	std::memcpy(&orientations[1], &rotateMat.m[1], sizeof(Vector3));
	std::memcpy(&orientations[2], &rotateMat.m[2], sizeof(Vector3));
}