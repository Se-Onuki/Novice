#include "Object.h"
#include "Header/Math/Lerp.h"
#include "Header/Render/Render.hpp"
#include <algorithm>

#include <imgui.h>

Triangle::Triangle(const Vector3 Vertices[3]) : vertices_{Vertices[0], Vertices[1], Vertices[2]} {}

Triangle::~Triangle() {}

void Triangle::ImGuiDebug(const std::string& group) {
	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("triangleVertex[0]", &vertices_[0].x, 0.1f);
		ImGui::DragFloat3("triangleVertex[1]", &vertices_[1].x, 0.1f);
		ImGui::DragFloat3("triangleVertex[2]", &vertices_[2].x, 0.1f);

		ImGui::TreePop();
	}
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

const float LineBase::Clamp(const float& t) const {
	switch (lineType) {
	case LineBase::LineType::Line:
		return t;
		break;
	case LineBase::LineType::Ray:
		return (t > 0.f) ? t : 0.f;
		break;
	case LineBase::LineType::Segment:
		return std::clamp(t, 0.f, 1.f);
		break;
	default:
		return 0.f;
		break;
	}
}
const char* LineBase::typeList[3] = {"Line", "Ray", "Segment"};

void LineBase::ImGuiDebug(const std::string& group) {
	if (ImGui::TreeNode(group.c_str())) {

		ImGui::DragFloat3("Origin", &origin.x, 0.1f);
		ImGui::DragFloat3("Diff", &diff.x, 0.1f);
		if (ImGui::BeginCombo("LineType", typeList[(uint8_t)lineType])) {

			for (uint8_t i = 0u; i < 3u; i++) {
				if (ImGui::Selectable(typeList[i])) {
					lineType = (LineType)i;
					break;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
}

float LineBase::ClosestProgress(const Vector3& point) const {
	return Clamp(((point - origin) * diff) / std::powf(diff.Length(), 2));
}

Plane Plane::Create(const Vector3& Normal, const Vector3& Point) {
	Plane out;

	LineBase line{.origin = Vector3::zero(), .diff = Normal, .lineType = LineBase::LineType::Line};
	Vector3 centor = line.ClosestPoint(Point);
	if (centor != Vector3::zero()) {
		out.normal = centor.Nomalize();
		out.distance = centor.Length();
	} else {
		out.normal = Normal;
		out.distance = Point * Normal;
	}

	return out;
}
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
	if (tMin > 1.f && tMin != line.Clamp(tMin))
		return false;
	if (tMax < 0.f && tMax != line.Clamp(tMax))
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

	LineBase localLine{
	    .origin = localOrigin, .diff = localEnd - localOrigin, .lineType = line.lineType};

	return IsHit(localOBB, localLine);
}

const bool Collision::IsHit(const OBB& obbA, const OBB& obbB) {
	Vector3 objA[8]{
  // lower
	    {-obbA.size.x, -obbA.size.y, -obbA.size.z},
	    {-obbA.size.x, -obbA.size.y, +obbA.size.z},
	    {+obbA.size.x, -obbA.size.y, +obbA.size.z},
	    {+obbA.size.x, -obbA.size.y, -obbA.size.z},
 // higher
	    {-obbA.size.x, +obbA.size.y, -obbA.size.z},
	    {-obbA.size.x, +obbA.size.y, +obbA.size.z},
	    {+obbA.size.x, +obbA.size.y, +obbA.size.z},
	    {+obbA.size.x, +obbA.size.y, -obbA.size.z},
	};
	const Matrix4x4& worldA = obbA.GetWorldMatrix();
	for (uint8_t i = 0; i < 8u; i++) {
		objA[i] *= worldA;
	}

	Vector3 objB[8]{
  // lower
	    {-obbB.size.x, -obbB.size.y, -obbB.size.z},
	    {-obbB.size.x, -obbB.size.y, +obbB.size.z},
	    {+obbB.size.x, -obbB.size.y, +obbB.size.z},
	    {+obbB.size.x, -obbB.size.y, -obbB.size.z},
 // higher
	    {-obbB.size.x, +obbB.size.y, -obbB.size.z},
	    {-obbB.size.x, +obbB.size.y, +obbB.size.z},
	    {+obbB.size.x, +obbB.size.y, +obbB.size.z},
	    {+obbB.size.x, +obbB.size.y, -obbB.size.z},
	};
	const Matrix4x4& worldB = obbB.GetWorldMatrix();
	for (uint8_t i = 0; i < 8u; i++) {
		objB[i] *= worldB;
	}

	return IsHitAxis(obbA.orientations[0].Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[1].Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[2].Nomalize(), objA, objB) &&

	       IsHitAxis(obbB.orientations[0].Nomalize(), objA, objB) &&
	       IsHitAxis(obbB.orientations[1].Nomalize(), objA, objB) &&
	       IsHitAxis(obbB.orientations[2].Nomalize(), objA, objB) &&

	       IsHitAxis(obbA.orientations[0].cross(obbB.orientations[0]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[0].cross(obbB.orientations[1]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[0].cross(obbB.orientations[2]).Nomalize(), objA, objB) &&

	       IsHitAxis(obbA.orientations[1].cross(obbB.orientations[0]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[1].cross(obbB.orientations[1]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[1].cross(obbB.orientations[2]).Nomalize(), objA, objB) &&

	       IsHitAxis(obbA.orientations[2].cross(obbB.orientations[0]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[2].cross(obbB.orientations[1]).Nomalize(), objA, objB) &&
	       IsHitAxis(obbA.orientations[2].cross(obbB.orientations[2]).Nomalize(), objA, objB);
}

const bool
    Collision::IsHitAxis(const Vector3& axis, const Vector3 vertexA[8], const Vector3 vertexB[8]) {

	float minA = 0.f, maxA = 0.f;
	float minB = 0.f, maxB = 0.f;
	for (uint8_t i = 0u; i < 8u; i++) {
		const float A = axis * vertexA[i];
		if (i == 0u) {
			minA = A;
			maxA = A;
		} else {
			minA = min(minA, A);
			maxA = max(maxA, A);
		}

		const float B = axis * vertexB[i];
		if (i == 0u) {
			minB = B;
			maxB = B;
		} else {
			minB = min(minB, B);
			maxB = max(maxB, B);
		}
	}
	const float diffA = maxA - minA;
	const float diffB = maxB - minB;

	const float diffAll = max(maxA, maxB) - min(minA, minB);
	return diffAll <= diffA + diffB;
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

Vector3 Spring::GetAcceleration(const Ball& ball) {
	const Vector3 diff = ball.position - anchor;
	const float length = diff.Length();
	if (length) {
		Vector3 direction = diff.Nomalize();
		Vector3 restPosition = anchor + direction * naturalLength;
		Vector3 desplacement = length * (ball.position - restPosition);
		Vector3 restoringForce = -stiffness * desplacement;

		Vector3 dampingForce = -dampingCoefficient * ball.velocity;
		Vector3 force = restoringForce + dampingForce;
		return force / ball.mass;
	}
	return Vector3::zero();
}

void Pendulum::MoveSwing(const Vector3& kGravity, const float deltaTime) {
	angularAcceleration = kGravity.y / length * std::sin(angle);
	angularVelocity += angularAcceleration * deltaTime;
	angle += angularVelocity * deltaTime;

	angularAcceleration = 0.f;
}

void ConicalPendulum::MoveAngle(const Vector3& kGravity, const float deltaTime) {
	angularVelocity = std::sqrt(-kGravity.y / (length * std::cos(halfApexAngle)));
	angle += angularVelocity * deltaTime;
	angle = std::fmod(angle, Angle::PI_2);
}

Vector3 ConicalPendulum::GetPos() { 
	const float radius = std::sin(halfApexAngle) * length;
	const float height = std::cos(halfApexAngle) * length;
	return Vector3{std::cos(angle) * radius, -height, std::sin(angle) * radius} + anchor;
}
