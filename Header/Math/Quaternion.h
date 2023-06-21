#pragma once
#include "Header/Math/Vector3.h"

struct Quaternion {
	float w;
	Vector3 v;

	Quaternion operator+(const Quaternion& other) const;
	Quaternion& operator+=(const Quaternion& other);

	Quaternion operator-(const Quaternion& other) const;
	Quaternion& operator-=(const Quaternion& other);

	Quaternion operator*(const float& scalar) const;
	Quaternion& operator*=(const float& scalar);

	Quaternion operator*(const Quaternion& other) const;
	Quaternion& operator*=(const Quaternion& other);

	static Quaternion Create(const Vector3& axis, const float& radian);
	static Quaternion FormDirection(const Vector3& direction);
	static Quaternion FromEuler(const Vector3& euler);
};