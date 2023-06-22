#include "Quaternion.h"

Quaternion Quaternion::operator+(const Quaternion& other) const {
	return Quaternion{.w = (w + other.w), .v = (v + other.v)};
}

Quaternion& Quaternion::operator+=(const Quaternion& other) {
	w += other.w;
	v += other.v;
	return *this;
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
	return Quaternion{.w = (w - other.w), .v = (v + other.v)};
}

Quaternion& Quaternion::operator-=(const Quaternion& other) {
	w -= other.w;
	v -= other.v;
	return *this;
}

Quaternion Quaternion::operator*(const float& scalar) const {
	return Quaternion{.w = (w * scalar), .v = (v * scalar)};
}

Quaternion& Quaternion::operator*=(const float& scalar) {
	w *= scalar;
	v *= scalar;
	return *this;
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	return Quaternion{
	    .w = (w * other.w - v * other.v), .v = (w * other.v + other.w * v + v.cross(other.v))};
}

Quaternion& Quaternion::operator*=(const Quaternion& other) { return *this = *this * other; }

Quaternion Quaternion::Create(const Vector3& axis, const float& radian) {
	Quaternion quaternion = {};
	if (axis.Length() <= 0) { // もし回転軸の長さが0ならば初期値のクォータニオンを返す
		return quaternion;
	}
	float halfSin = std::sin(radian / 2), halfCos = std::cos(radian / 2);
	const Vector3& normalAxis = axis.Nomalize();

	quaternion.w = halfCos;
	quaternion.v = normalAxis * halfSin;

	return quaternion;
}

Quaternion Quaternion::FormDirection(const Vector3& direction) {
	return Quaternion{.w = 0, .v = direction};
}

Quaternion Quaternion::FromEuler(const Vector3& euler) {
	const Vector3 halfCos{
	    std::cos(euler.x / 2.f), std::cos(euler.y / 2.f), std::cos(euler.z / 2.f)};
	const Vector3 halfSin{
	    std::sin(euler.x / 2.f), std::sin(euler.y / 2.f), std::sin(euler.z / 2.f)};

	return Quaternion{
	    .w = halfCos.x * halfCos.y * halfCos.z - halfSin.x * halfSin.y * halfSin.z,
	    .v = {
	          halfSin.x * halfCos.y * halfCos.z + halfCos.x * halfSin.y * halfSin.z,
	          halfCos.x * halfSin.y * halfCos.z - halfSin.x * halfCos.y * halfSin.z,
	          halfCos.x * halfCos.y * halfSin.z + halfSin.x * halfSin.y * halfCos.z}
    };
}
