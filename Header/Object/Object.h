#pragma once
#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Transform.h"
#include <vector>

struct Sphere {
	Vector3 center;
	float radius;
};

/// @brief 3角ポリゴン
class Triangle {
	// 頂点リスト(時計回り)
	Vector3 vertices_[3];

public:
	/// @brief コンストラクタ
	/// @param LocalVertices 頂点リスト(時計回り)
	Triangle(const Vector3 Vertices[3]);
	Triangle(const Triangle& other) { memcpy_s(this, sizeof(Triangle), &other, sizeof(Triangle)); }
	~Triangle();

	_NODISCARD Triangle GetWorldTriangle(const Matrix4x4& worldMatrix) const;

	_NODISCARD Triangle GetScreenTriangle() const;

	_NODISCARD Vector3 GetNormal() const {
		const Vector3& VecA = vertices_[1] - vertices_[0]; // 0 から 1 に向けて
		const Vector3& VecB = vertices_[2] - vertices_[1]; // 1 から 2 に向けて
		return (VecA ^ VecB).Nomalize();
	}
};

class Object {
	std::vector<Triangle> object_;
	Transform transform_;

public:
	Object(const Transform& transform);
	~Object();
};

class Camera {
	Transform transform_;
	Matrix4x4 viewMatrix;

public:
	Camera(const Vector3& position, const Vector3& rotate = {0.f, 0.f, 0.f});
	~Camera();
	const Vector3 GetFacing() const { return TransformNormal(Vector3{0.f, 0.f, 1.f}, viewMatrix); }
	void Draw(const Object& object) const;
};