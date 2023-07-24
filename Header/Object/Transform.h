#pragma once
#include "Header/Math/Vector3.h"

struct Transform {
	Transform() = default;
	Transform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
	Transform* parent_ = nullptr;
	/// @brief 通常アフィン変換
	/// @return SRT行列
	_NODISCARD Matrix4x4 Affine() const;

	/// @brief スケーリング無効アフィン変換
	/// @return RT行列
	_NODISCARD Matrix4x4 AffineRT() const;

	Matrix4x4 AffineInv() const;


	bool ImGuiWidget();
	bool ImGuiWidget2D();
};