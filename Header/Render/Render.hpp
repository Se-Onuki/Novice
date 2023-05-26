#pragma once
#include "Header/Math/Matrix4x4.h"
#include <vector>
struct Vector2;
struct Sphere;
struct Segment;
class Camera;
class Triangle;

class Render final {
	Matrix4x4 viewportMatrix_;

public:
	std::vector<Triangle> ndcSurface_;
	std::vector<Triangle> screenSurface_;

	Render() {}
	~Render() {}

	/// @brief 正射影行列関数
	/// @param LeftTop 左上
	/// @param RightBottom 右下
	/// @param nearClip 近平面
	/// @param farClip 遠平面
	/// @return 正射影行列
	static Matrix4x4 MakeOrthographicMatrix(
	    const Vector2& LeftTop, const Vector2& RightBottom, const float& nearClip,
	    const float& farClip);

	/// @brief 透視射影行列関数
	/// @param fovY 縦画角
	/// @param aspectRatio アスペクト比
	/// @param nearClip 近平面
	/// @param farClip 遠平面
	/// @return 透視射影行列
	static Matrix4x4 MakePerspectiveFovMatrix(
	    const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip);

	static Matrix4x4 MakeViewportMatrix(
	    const float& Left, const float& Top, const float& width, const float& height,
	    const float& minDepth, const float& maxDepth);

	void SetViewportMatrix(const Matrix4x4& viewport);

	_NODISCARD const Matrix4x4& GetViewportMatrix() { return viewportMatrix_; }

	void UpdateSurface();

	void Draw() const;

	void DrawGrid(
	    const Matrix4x4& viewProjectionMatrix, const float& radius = 2.f,
	    const uint32_t& subdivision = 10u) const;

	void DrawSphere(
	    const Matrix4x4& viewProjectionMatrix, const Sphere& sphere,
	    const uint32_t& color = 0xFFFFFFFF, const uint32_t& subdivision = 10u) const;

	void DrawLine(
	    const Matrix4x4& viewProjectionMatrix, const Segment& segment, const uint32_t& color) const;
};
