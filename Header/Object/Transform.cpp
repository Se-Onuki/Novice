#include "Transform.h"
#include "Header/Math/Matrix4x4.h"

Transform::Transform(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
    : scale(scale), rotate(rotate), translate(translate) {}

Matrix4x4 Transform::Affine() const { return Matrix4x4::Affine(scale, rotate, translate); }

Matrix4x4 Transform::AffineRT() const {
	return Matrix4x4::Affine(Vector3::one(), rotate, translate);
}

Matrix4x4 Transform::AffineInv() const { return Matrix4x4(); }
