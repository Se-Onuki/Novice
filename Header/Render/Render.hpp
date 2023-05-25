#pragma once
#include <cmath>
struct Vector2;
struct Matrix4x4;
struct Sphere;

class Render final {
public:
	Render();
	~Render();

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

	static void DrawGrid(
	    const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix,
	    const float& radius = 2.f, const uint32_t& subdivision = 10u);

	static void DrawSphere(
	    const Sphere& sphere, const Matrix4x4& viewProjectionMatrix,
	    const Matrix4x4& viewportMatrix, const uint32_t& subdivision = 10u, const uint32_t& color = 0xFFFFFFFF);
};