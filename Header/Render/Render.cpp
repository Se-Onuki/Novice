#include "Render.hpp"

#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector2.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Object.h"

#include "Header/Math/Lerp.h"

#include <Novice.h>

#include <cmath>
#include <numbers>

Matrix4x4 Render::MakeOrthographicMatrix(
    const Vector2& LeftTop, const Vector2& RightBottom, const float& nearClip,
    const float& farClip) {
	return Matrix4x4{
	    2.f / (RightBottom.x - LeftTop.x),
	    0.f,
	    0.f,
	    0.f,

	    0.f,
	    2.f / (LeftTop.y - RightBottom.y),
	    0.f,
	    0.f,

	    0.f,
	    0.f,
	    1 / (farClip - nearClip),
	    0.f,

	    (RightBottom.x + LeftTop.x) / (LeftTop.x - RightBottom.x),
	    (LeftTop.y + RightBottom.y) / (RightBottom.y - LeftTop.y),
	    nearClip / (nearClip - farClip),
	    1.f};
}

Matrix4x4 Render::MakePerspectiveFovMatrix(
    const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip) {
	return Matrix4x4{
	    (1 / aspectRatio) * (1 / std::tan(fovY / 2)),
	    0,
	    0,
	    0,
	    0,
	    (1 / std::tan(fovY / 2)),
	    0,
	    0,
	    0,
	    0,
	    farClip / (farClip - nearClip),
	    1,
	    0,
	    0,
	    (-nearClip * farClip) / (farClip - nearClip),
	    0};
}

Matrix4x4 Render::MakeViewportMatrix(
    const float& Left, const float& Top, const float& width, const float& height,
    const float& minDepth, const float& maxDepth) {
	return Matrix4x4{
	    width / 2,
	    0,
	    0,
	    0,
	    0,
	    -height / 2,
	    0,
	    0,
	    0,
	    0,
	    maxDepth - minDepth,
	    0,
	    Left + width / 2,
	    Top + height / 2,
	    minDepth,
	    1};
}

void Render::SetViewportMatrix(const Matrix4x4& viewport) { viewportMatrix_ = viewport; }

void Render::UpdateSurface() {
	screenSurface_.clear();
	for (uint32_t i = 0; i < ndcSurface_.size(); i++) {
		screenSurface_.emplace_back(ndcSurface_[i] * viewportMatrix_);
	}
	ndcSurface_.clear();
}

void Render::Draw() const {

	for (const Triangle& surface : screenSurface_) {
		Novice::DrawTriangle(
		    static_cast<int>(surface.vertices_[0].x), static_cast<int>(surface.vertices_[0].y),
		    static_cast<int>(surface.vertices_[1].x), static_cast<int>(surface.vertices_[1].y),
		    static_cast<int>(surface.vertices_[2].x), static_cast<int>(surface.vertices_[2].y), RED,
		    kFillModeSolid);
	}
}

void Render::DrawGrid(
    const Matrix4x4& viewProjectionMatrix, const float& radius, const uint32_t& subdivision) const {
	const float kGridEvery = (radius * 2.f) / static_cast<float>(subdivision);

	for (uint32_t xIndex = 0u; xIndex <= subdivision; xIndex++) {
		Vector3 line[2]{
		    Vector3{-radius + kGridEvery * xIndex, 0, -radius},
		    Vector3{-radius + kGridEvery * xIndex, 0, +radius}
        };
		Vector3 startScreen = line[0] * viewProjectionMatrix * viewportMatrix_;
		Vector3 endScreen = line[1] * viewProjectionMatrix * viewportMatrix_;
		Novice::DrawLine(
		    static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
		    static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), WHITE);
	}
	for (uint32_t zIndex = 0u; zIndex <= subdivision; zIndex++) {
		Vector3 line[2]{
		    Vector3{-radius, 0, -radius + kGridEvery * zIndex},
		    Vector3{+radius, 0, -radius + kGridEvery * zIndex}
        };
		Vector3 startScreen = line[0] * viewProjectionMatrix * viewportMatrix_;
		Vector3 endScreen = line[1] * viewProjectionMatrix * viewportMatrix_;
		Novice::DrawLine(
		    static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
		    static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), WHITE);
	}

	Vector3 line[4]{
	    Vector3{0,       0, -radius},
        Vector3{0,       0, +radius},
        Vector3{-radius, 0, 0      },
	    Vector3{+radius, 0, 0      }
    };
	Vector3 xStartScreen = line[0] * viewProjectionMatrix * viewportMatrix_;
	Vector3 xEndScreen = line[1] * viewProjectionMatrix * viewportMatrix_;

	Vector3 yStartScreen = line[0] * viewProjectionMatrix * viewportMatrix_;
	Vector3 yEndScreen = line[1] * viewProjectionMatrix * viewportMatrix_;

	Novice::DrawLine(
	    static_cast<int>(xStartScreen.x), static_cast<int>(xStartScreen.y),
	    static_cast<int>(xEndScreen.x), static_cast<int>(xEndScreen.y), BLACK);
	Novice::DrawLine(
	    static_cast<int>(yStartScreen.x), static_cast<int>(yStartScreen.y),
	    static_cast<int>(yEndScreen.x), static_cast<int>(yEndScreen.y), BLACK);
}

void Render::DrawTriangle(
    const Matrix4x4& viewProjectionMatrix, const Triangle& triangle, const uint32_t& color,
    const FillMode& fillMode) const {
	Triangle screenTriangle(triangle);
	Matrix4x4 VPVp = viewProjectionMatrix * viewportMatrix_;
	for (auto& vertex : screenTriangle.vertices_) {
		vertex *= VPVp;
	}

	Novice::DrawTriangle(
	    static_cast<int>(screenTriangle.vertices_[0].x),
	    static_cast<int>(screenTriangle.vertices_[0].y),
	    static_cast<int>(screenTriangle.vertices_[1].x),
	    static_cast<int>(screenTriangle.vertices_[1].y),
	    static_cast<int>(screenTriangle.vertices_[2].x),
	    static_cast<int>(screenTriangle.vertices_[2].y), color, fillMode);
}

void Render::DrawSphere(
    const Matrix4x4& viewProjectionMatrix, const Sphere& sphere, const uint32_t& color,
    const uint32_t& subdivision) const {
	const float kLonEvery = Angle::Digree(360).ToRadian() / subdivision; // 経度
	const float kLatEvery = Angle::Digree(180).ToRadian() / subdivision; // 緯度
	// 緯度の方向に分解 -90 ~ 90
	for (uint32_t latIndex = 0u; latIndex < subdivision; ++latIndex) {
		const float lat = static_cast<float>(-std::numbers::pi) / 2.f + kLatEvery * latIndex;
		// 経度の方向に分解 0 ~ 360
		for (uint32_t lonIndex = 0u; lonIndex < subdivision; ++lonIndex) {
			const float lon = lonIndex * kLonEvery;
			Vector3 a, b, c;
			a = {// 中心
			     sphere.radius * std::cos(lat) * std::cos(lon), sphere.radius * std::sin(lat),
			     sphere.radius * std::cos(lat) * std::sin(lon)};
			b = {// 上
			     sphere.radius * std::cos(lat + kLatEvery) * std::cos(lon),
			     sphere.radius * std::sin(lat + kLatEvery),
			     sphere.radius * std::cos(lat + kLatEvery) * std::sin(lon)};
			c = {// 右
			     sphere.radius * std::cos(lat) * std::cos(lon + kLonEvery),
			     sphere.radius * std::sin(lat),
			     sphere.radius * std::cos(lat) * std::sin(lon + kLonEvery)};
			Vector3 screenVertex[3]{
			    (a + sphere.centor) * viewProjectionMatrix * viewportMatrix_,
			    (b + sphere.centor) * viewProjectionMatrix * viewportMatrix_,
			    (c + sphere.centor) * viewProjectionMatrix * viewportMatrix_};

			Novice::DrawLine( // a->b
			    static_cast<int>(screenVertex[0].x), static_cast<int>(screenVertex[0].y),
			    static_cast<int>(screenVertex[1].x), static_cast<int>(screenVertex[1].y), color);
			Novice::DrawLine( // a->c
			    static_cast<int>(screenVertex[0].x), static_cast<int>(screenVertex[0].y),
			    static_cast<int>(screenVertex[2].x), static_cast<int>(screenVertex[2].y), color);
		}
	}
}

void Render::DrawLine(
    const Matrix4x4& viewProjectionMatrix, const LineBase& segment, const uint32_t& color) const {
	Vector3 vertex[2];
	vertex[0] = segment.origin;
	vertex[1] = segment.GetEnd();

	for (auto& element : vertex) {
		element = element * viewProjectionMatrix * viewportMatrix_;
	}

	Novice::DrawLine(
	    static_cast<int>(vertex[0].x), static_cast<int>(vertex[0].y), static_cast<int>(vertex[1].x),
	    static_cast<int>(vertex[1].y), color);
}

void Render::DrawPlane(
    const Matrix4x4& viewProjectionMatrix, const Plane& plane, const uint32_t& color) const {
	Vector3 centor = plane.distance * plane.normal;
	Vector3 perpendiculars[4];
	perpendiculars[0] = plane.normal.Perpendicular().Nomalize();
	perpendiculars[1] = perpendiculars[0] * -1;
	perpendiculars[2] = plane.normal.cross(perpendiculars[0]);
	perpendiculars[3] = perpendiculars[2] * -1;

	Vector3 vertex[4];
	for (uint32_t i = 0; i < 4; i++) {
		vertex[i] = (perpendiculars[i] * 2 + centor) * viewProjectionMatrix * viewportMatrix_;
	}
	Novice::DrawLine(
	    static_cast<int>(vertex[0].x), static_cast<int>(vertex[0].y), static_cast<int>(vertex[2].x),
	    static_cast<int>(vertex[2].y), color);
	Novice::DrawLine(
	    static_cast<int>(vertex[0].x), static_cast<int>(vertex[0].y), static_cast<int>(vertex[3].x),
	    static_cast<int>(vertex[3].y), color);
	Novice::DrawLine(
	    static_cast<int>(vertex[1].x), static_cast<int>(vertex[1].y), static_cast<int>(vertex[2].x),
	    static_cast<int>(vertex[2].y), color);
	Novice::DrawLine(
	    static_cast<int>(vertex[1].x), static_cast<int>(vertex[1].y), static_cast<int>(vertex[3].x),
	    static_cast<int>(vertex[3].y), color);
}

void Render::DrawAABB(
    const Matrix4x4& viewProjectionMatrix, const AABB& aabb, const uint32_t& color) const {
	Vector3 lower[4]{
	    {aabb.min.x, aabb.min.y, aabb.min.z},
	    {aabb.min.x, aabb.min.y, aabb.max.z},
	    {aabb.max.x, aabb.min.y, aabb.max.z},
	    {aabb.max.x, aabb.min.y, aabb.min.z},
	};
	Vector3 higher[4]{
	    {aabb.min.x, aabb.max.y, aabb.min.z},
	    {aabb.min.x, aabb.max.y, aabb.max.z},
	    {aabb.max.x, aabb.max.y, aabb.max.z},
	    {aabb.max.x, aabb.max.y, aabb.min.z},
	};
	const Matrix4x4 VPVp = viewProjectionMatrix * viewportMatrix_;
	for (uint8_t i = 0; i < 4; i++) {
		lower[i] *= VPVp;
		higher[i] *= VPVp;
	}
	for (uint8_t i = 0; i < 4; i++) {
		uint8_t k = (i + 1) % 4;
		Novice::DrawLine(
		    static_cast<int>(lower[i].x), static_cast<int>(lower[i].y),
		    static_cast<int>(lower[k].x), static_cast<int>(lower[k].y), color);
		Novice::DrawLine(
		    static_cast<int>(higher[i].x), static_cast<int>(higher[i].y),
		    static_cast<int>(higher[k].x), static_cast<int>(higher[k].y), color);

		Novice::DrawLine(
		    static_cast<int>(lower[i].x), static_cast<int>(lower[i].y),
		    static_cast<int>(higher[i].x), static_cast<int>(higher[i].y), color);
	}
}

void Render::DrawOBB(
    const Matrix4x4& viewProjectionMatrix, const OBB& obb, const uint32_t& color) const {
	Vector3 lower[4]{
	    {-obb.size.x, -obb.size.y, -obb.size.z},
	    {-obb.size.x, -obb.size.y, +obb.size.z},
	    {+obb.size.x, -obb.size.y, +obb.size.z},
	    {+obb.size.x, -obb.size.y, -obb.size.z},
	};
	Vector3 higher[4]{
	    {-obb.size.x, +obb.size.y, -obb.size.z},
	    {-obb.size.x, +obb.size.y, +obb.size.z},
	    {+obb.size.x, +obb.size.y, +obb.size.z},
	    {+obb.size.x, +obb.size.y, -obb.size.z},
	};
	const Matrix4x4& wVPVp = obb.GetWorldMatrix() * viewProjectionMatrix * viewportMatrix_;
	for (uint8_t i = 0; i < 4; i++) {
		lower[i] *= wVPVp;
		higher[i] *= wVPVp;
	}
	for (uint8_t i = 0; i < 4; i++) {
		uint8_t k = (i + 1) % 4;
		Novice::DrawLine(
		    static_cast<int>(lower[i].x), static_cast<int>(lower[i].y),
		    static_cast<int>(lower[k].x), static_cast<int>(lower[k].y), color);
		Novice::DrawLine(
		    static_cast<int>(higher[i].x), static_cast<int>(higher[i].y),
		    static_cast<int>(higher[k].x), static_cast<int>(higher[k].y), color);

		Novice::DrawLine(
		    static_cast<int>(lower[i].x), static_cast<int>(lower[i].y),
		    static_cast<int>(higher[i].x), static_cast<int>(higher[i].y), color);
	}
}

void Render::DrawCurve(
    const Matrix4x4& viewProjectionMatrix, const Bezier& bezier, const uint32_t& color,
    const uint32_t& subdivision) const {

	const Matrix4x4& VPVp = viewProjectionMatrix * viewportMatrix_;
	std::vector<Vector3> posList_;
	posList_.reserve(subdivision + 1);
	for (uint32_t i = 0; i < subdivision + 1; i++) {
		posList_.push_back(bezier.GetPosition(float(i) / subdivision) * VPVp);
	}
	for (uint32_t i = 0; i < subdivision; i++) {
		Novice::DrawLine(
		    static_cast<int>(posList_[i].x), static_cast<int>(posList_[i].y),
		    static_cast<int>(posList_[i + 1].x), static_cast<int>(posList_[i + 1].y), color);
	}
	for (auto& pos : bezier.GetPointList()) {
		const Vector3& position = pos * VPVp;
		const int32_t radius = 2;
		Novice::DrawBox(
		    static_cast<int>(position.x) - radius, static_cast<int>(position.y) - radius,
		    radius * 2, radius * 2, 0.f, 0xFF0000FF, kFillModeSolid);
	}
}

void Render::DrawCurve(
    const Matrix4x4& viewProjectionMatrix, const Catmull& catmull, const uint32_t& color,
    const uint32_t& subdivision) const {

	const Matrix4x4& VPVp = viewProjectionMatrix * viewportMatrix_;
	std::vector<Vector3> posList_;
	posList_.reserve(subdivision + 1);
	for (uint32_t i = 0; i < subdivision + 1; i++) {
		posList_.push_back(
		    catmull.GetStructPosition(float(i) / subdivision * catmull.GetPointList().size()) *
		    VPVp);
	}
	for (uint32_t i = 0; i < subdivision; i++) {
		Novice::DrawLine(
		    static_cast<int>(posList_[i].x), static_cast<int>(posList_[i].y),
		    static_cast<int>(posList_[i + 1].x), static_cast<int>(posList_[i + 1].y), color);
	}
	for (auto& pos : catmull.GetPointList()) {
		const Vector3& position = pos * VPVp;
		const int32_t radius = 2;
		Novice::DrawBox(
		    static_cast<int>(position.x) - radius, static_cast<int>(position.y) - radius,
		    radius * 2, radius * 2, 0.f, 0xFF0000FF, kFillModeSolid);
	}
}

void Render::DrawAxis(const Matrix4x4& viewProjectionMatrix) const { viewProjectionMatrix; }
