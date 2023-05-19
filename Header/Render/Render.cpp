#include "Render.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Vector2.h"
#include "Header/Math/Vector3.h"
#include "Header/Object/Object.h"

#include <Novice.h>

#include <cmath>

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
