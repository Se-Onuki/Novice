#pragma once
#include "Header/Math/Vector3.h"
/// @brief 回転順制御
enum class EulerOrder { XYZ, XZY, YXZ, YZX, ZXY, ZYX };
/// @brief オイラー角
struct Euler {
	Vector3 rotate;
	EulerOrder order = EulerOrder::XYZ;

	Matrix4x4 ToMatrix() const;
};