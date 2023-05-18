#pragma once
#include "Header/Math/Vector3.h"

struct Transform
{
	Transform(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;

	_NODISCARD Matrix4x4 Affine() const;
};