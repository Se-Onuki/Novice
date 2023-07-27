#pragma once
#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Transform.h"
#include <functional>
#include <string>
#include <vector>

class Render;
struct LineBase;
struct Plane;
struct Triangle;
struct Sphere;
struct AABB;
struct OBB;

namespace Collision {
const bool IsHit(const LineBase& line, const Plane& plane);
const bool IsHit(const Sphere& sphereA, const Sphere& sphereB);
const bool IsHit(const Sphere& sphere, const Plane& plane);
const bool IsHit(const LineBase& line, const Triangle& triangle);
const bool IsHit(const AABB& a, const AABB& b);
const bool IsHit(const AABB& aabb, const Sphere& sphere);
const bool IsHit(const AABB& aabb, const LineBase& line);

const bool IsHit(const OBB& obb, const Sphere& sphere);
const bool IsHit(const OBB& obb, const LineBase& line);

const bool IsHit(const OBB& obbA, const OBB& obbB);

const bool IsHitAxis(const Vector3& axis, const Vector3 vertexA[8], const Vector3 vertexB[8]);

const Vector3 HitPoint(const LineBase& line, const Plane& plane);

// const bool

} // namespace Collision

struct OBB {
	Vector3 centor; // 中心点
	Vector3 orientations[3u] = {
	    Vector3::right(), Vector3::up(), Vector3::front()}; // ローカル座標軸。正規化、直交必須
	Vector3 size;                                           // 中心点からの各軸の半径

	void SetRotate(const Vector3& euler);

	const Matrix4x4 GetWorldMatrix() const;
	const Matrix4x4 GetInverseMatrix() const;

	void ImGuiDebug(const std::string& group, Vector3& rotate);
};

struct Plane {
	Vector3 normal;
	float distance;

	static Plane Create(const Vector3& Normal, const Vector3& Point);
	static Plane Create(const Vector3 Vertex[3]) {
		return Create(((Vertex[1] - Vertex[0]).cross(Vertex[2] - Vertex[1])).Nomalize(), Vertex[0]);
	}
	static Plane Create(const Triangle& trinagle);

	float GetDistance(const Vector3& point) const { return normal * point - distance; }
	const bool IsCollision(const LineBase& other) const;
};

class Vertices {
public:
	Vertices(const std::vector<Vector3>& vertices) : vertices_(vertices) {}
	std::vector<Vector3> vertices_;
};

/// @brief 3角ポリゴン
struct Triangle {
	// 頂点リスト(時計回り)
	Vector3 vertices_[3] = {};

	Triangle() {}
	/// @param LocalVertices 頂点リスト(時計回り)
	Triangle(const Vector3 Vertices[3]);
	Triangle(const Vector3& Vertice0, const Vector3& Vertice1, const Vector3& Vertice2)
	    : vertices_{Vertice0, Vertice1, Vertice2} {}

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
		return (VecA.cross(VecB)).Nomalize();
	}

	void ImGuiDebug(const std::string& group);
};

struct Sphere {
	Vector3 centor;
	float radius;

	bool IsCollision(const Sphere& other) { return Collision::IsHit(*this, other); }
	bool IsCollision(const Plane& plane) { return Collision::IsHit(*this, plane); }

	void ImGuiDebug(const std::string& group);
};

struct LineBase final {
	enum class LineType { Line, Ray, Segment };
	// LineBase(const Vector3& Origin, const Vector3& Diff) : origin(Origin), diff(Diff) {}
	Vector3 origin; // 始点
	Vector3 diff;   // 終点へのベクトル
	LineType lineType = LineType::Segment;

	[[nodiscard]] Vector3 GetEnd() const { return origin + diff; }
	[[nodiscard]] Vector3 GetProgress(const float& t) const;
	[[nodiscard]] Vector3 Project(const Vector3& point) const;
	[[nodiscard]] Vector3 ClosestPoint(const Vector3& point) const;
	[[nodiscard]] const float Clamp(const float& t) const;

	void ImGuiDebug(const std::string& group);

private:
	[[nodiscard]] float ClosestProgress(const Vector3& point) const;
	static const char* typeList[3];
};

struct AABB {
	Vector3 min;
	Vector3 max;

	void ImGuiDebug(const std::string& group);
	void Swaping();
};

struct Spring {
	Vector3 anchor;           // 固定された端
	float naturalLength;      // 自然長
	float stiffness;          // 剛性 ばね定数k
	float dampingCoefficient; // 減衰係数
};

struct Ball {
	Vector3 position;     // ボールの位置
	Vector3 velocity;     // ボールの速度
	Vector3 acceleration; // ボールの加速度
	float mass;           // ボールの質量
	float radius;         // ボールの半径
	uint32_t color;       // ボールの色
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

	void UpdateMatrix() { viewMatrix_ = transform_.Affine().InverseRT(); }

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