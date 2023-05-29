#pragma once
#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Transform.h"
#include <vector>

class Render;

struct Plane {
	Vector3 normal;
	float distance;

	static Plane Create(const Vector3& Normal, const Vector3& Point) {
		Plane out;
		out.normal = Normal;
		out.distance = Point * Normal;
		return out;
	}
	static Plane Create(const Vector3 Vertex[3]) {
		return Create(((Vertex[1] - Vertex[0]) ^ (Vertex[2] - Vertex[1])).Nomalize(), Vertex[0]);
	}

	float GetDistance(const Vector3& point) const { return normal * point - distance; }
};

class Vertices {
public:
	Vertices(const std::vector<Vector3>& vertices) : vertices_(vertices) {}
	std::vector<Vector3> vertices_;
};

/// @brief 3角ポリゴン
class Triangle {
public:
	// 頂点リスト(時計回り)
	Vector3 vertices_[3];

	Triangle() {}
	/// @param LocalVertices 頂点リスト(時計回り)
	Triangle(const Vector3 Vertices[3]);

	Triangle(const Triangle& other) { memcpy_s(this, sizeof(Triangle), &other, sizeof(Triangle)); }
	~Triangle();

	[[nodiscard]] const Triangle operator*(const Matrix4x4& matrix) const {
		Triangle out{};
		for (uint8_t i = 0; i < 3; i++) {
			out.vertices_[i] = (vertices_[i] * matrix);
		}
		return out;
	}

	[[nodiscard]] Vector3 GetNormal() const {
		const Vector3& VecA = vertices_[1] - vertices_[0]; // 0 から 1 に向けて
		const Vector3& VecB = vertices_[2] - vertices_[1]; // 1 から 2 に向けて
		return (VecA ^ VecB).Nomalize();
	}
};

struct Sphere {
	Vector3 center;
	float radius;

	bool IsCollision(const Sphere& other) {
		return (center - other.center).Length() <= radius + other.radius;
	}
	bool IsCollision(const Plane& plane) { return std::abs(plane.GetDistance(center)) <= radius; }
};

struct LineBase {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点へのベクトル

	[[nodiscard]] Vector3 GetEnd() const { return origin + diff; }
	[[nodiscard]] Vector3 GetProgress(const float& t) const;
	[[nodiscard]] Vector3 Project(const Vector3& point) const;
	[[nodiscard]] Vector3 ClosestPoint(const Vector3& point) const;

protected:
	[[nodiscard]] float ClosestProgress(const Vector3& point) const;
	[[nodiscard]] virtual const float Clamp(const float& t) const = 0;
};

/// @brief 直線
struct Line final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

/// @brief 半直線
struct Ray final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

/// @brief 線分
struct Segment final : public LineBase {
private:
	[[nodiscard]] const float Clamp(const float& t) const override;
};

class ModelClass {
public:
	ModelClass();
	~ModelClass();
	std::vector<Triangle> surfaceList_;
	void AddSurface(const Triangle& surface) { surfaceList_.emplace_back(surface); }
};

class Object3d {
public:
	enum class FillMode {
		kFillModeSolid,     // ソリッド
		kFillModeWireFrame, // ワイヤーフレーム
	};
	ModelClass* model_;
	Transform transform_;
	Matrix4x4 worldMatrix_;

	void UpdateMatrix() { worldMatrix_ = transform_.Affine(); }

	FillMode fillMode_ = FillMode::kFillModeSolid;

	Object3d(const Transform& transform, ModelClass* model);
	~Object3d();
};

class Camera {
	Transform transform_;
	Matrix4x4 viewMatrix_;
	Matrix4x4 projectonMatrix_;

public:
	Camera(const Matrix4x4& projectonMatrix);
	~Camera();

	void UpdateMatrix() { viewMatrix_ = transform_.Affine().Inverse(); }

	void SetTransform(const Transform& transform) { transform_ = transform; }
	[[nodiscard]] Transform GetTransform() const { return transform_; }

	void SetProjection(const Matrix4x4& projectonMatrix) { projectonMatrix_ = projectonMatrix; }

	[[nodiscard]] const Matrix4x4 GetViewProjection() const {
		return viewMatrix_ * projectonMatrix_;
	}
	[[nodiscard]] const Vector3 GetFacing() const {
		return TransformNormal(Vector3{0.f, 0.f, 1.f}, viewMatrix_);
	}

	[[nodiscard]] const Matrix4x4 wvVpMatrix(const Object3d& object) const;
	void CreateNDC(const Object3d& object, Render* render) const;
	void CreateNDC(const std::vector<Object3d>& objectList, Render* render) const;
};