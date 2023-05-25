#pragma once
#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Transform.h"
#include <vector>

class Render;

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

	_NODISCARD const Triangle operator*(const Matrix4x4& matrix) const {
		Triangle out{};
		for (uint8_t i = 0; i < 3; i++) {
			out.vertices_[i] = (vertices_[i] * matrix);
		}
		return out;
	}

	_NODISCARD Vector3 GetNormal() const {
		const Vector3& VecA = vertices_[1] - vertices_[0]; // 0 から 1 に向けて
		const Vector3& VecB = vertices_[2] - vertices_[1]; // 1 から 2 に向けて
		return (VecA ^ VecB).Nomalize();
	}
};

struct LineBase {
	Vector3 origin; // 始点
	Vector3 diff;   // 終点へのベクトル
};

/// @brief 直線
struct Line : public LineBase {};

/// @brief 半直線
struct Ray : public LineBase {};

/// @brief 線分
struct Segment : public LineBase {};

struct Sphere {
	Vector3 center;
	float radius;
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
	_NODISCARD Transform GetTransform() const { return transform_; }

	void SetProjection(const Matrix4x4& projectonMatrix) { projectonMatrix_ = projectonMatrix; }

	_NODISCARD const Vector3 GetFacing() const {
		return TransformNormal(Vector3{0.f, 0.f, 1.f}, viewMatrix_);
	}

	_NODISCARD const Matrix4x4 wvVpMatrix(const Matrix4x4& worldMatrix) const;
	_NODISCARD const Matrix4x4 wvVpMatrix(const Object3d& object) const;
	void CreateNDC(const Object3d& object, Render* render) const;
	void CreateNDC(const std::vector<Object3d>& objectList, Render* render) const;
};