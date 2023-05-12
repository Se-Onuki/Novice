#include "Matrix4x4.h"

#include "Matrix2x2.h"
#include "Vector3.h"

#include <Novice.h>

#include "Math.hpp"
#include <cmath>

void Matrix4x4::Printf(const int& x, const int& y) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(
			    x + j * static_cast<int>(TextSize.x) * 2, y + i * static_cast<int>(TextSize.y),
			    "%10.6f", m[i][j]);
		}
	}
}

Matrix4x4 Matrix4x4::Inverse() {
	Matrix4x4 calc = *this;
	Matrix4x4 out = Identity();

	for (size_t i = 0; i < 4; i++) {
		float value = calc.m[i][i];
		for (size_t k = 0; k < 4; k++) {
			calc.m[i][k] /= value;
			out.m[i][k] /= value;
		}
		for (size_t k = 0; k < 4; k++) {
			if (k != i) {
				value = -calc.m[k][i];
				for (size_t l = 0; l < 4; l++) {
					calc.m[k][l] += calc.m[i][l] * value;
					out.m[k][l] += out.m[i][l] * value;
				}
			}
		}
	}
	return out;

	/*return Matrix4x4{
	           this->m[1][1] * this->m[2][2] - this->m[1][2] * this->m[2][1],
	           this->m[0][2] * this->m[2][1] - this->m[0][1] * this->m[2][2],
	           this->m[0][1] * this->m[1][2] - this->m[0][2] * this->m[1][1],

	           this->m[1][2] * this->m[2][0] - this->m[1][0] * this->m[2][2],
	           this->m[0][0] * this->m[2][2] - this->m[0][2] * this->m[2][0],
	           this->m[0][2] * this->m[1][0] - this->m[0][0] * this->m[1][2],

	           this->m[1][0] * this->m[2][1] - this->m[1][1] * this->m[2][0],
	           this->m[0][1] * this->m[2][0] - this->m[0][0] * this->m[2][1],
	           this->m[0][0] * this->m[1][1] - this->m[0][1] * this->m[1][0]} /
	       (this->m[0][0] * this->m[1][1] * this->m[2][2] +
	        this->m[0][1] * this->m[1][2] * this->m[2][0] +
	        this->m[0][2] * this->m[1][0] * this->m[2][1] -
	        this->m[0][0] * this->m[1][2] * this->m[2][1] -
	        this->m[0][1] * this->m[1][0] * this->m[2][2] -
	        this->m[0][2] * this->m[1][1] * this->m[2][0]);*/
}

Matrix4x4 Matrix4x4::Transpose() {
	return Matrix4x4{
	    this->m[0][0], this->m[1][0], this->m[2][0], this->m[3][0], this->m[0][1], this->m[1][1],
	    this->m[2][1], this->m[3][1], this->m[0][2], this->m[1][2], this->m[2][2], this->m[3][2],
	    this->m[0][3], this->m[1][3], this->m[2][3], this->m[3][3],
	};
}

Matrix4x4 Matrix4x4::Affine(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return Matrix4x4{
	    scale.x * (std::cos(rotate.y) * std::cos(rotate.z)),
	    scale.x * (std::cos(rotate.y) * std::sin(rotate.z)),
	    scale.x * (-std::sin(rotate.y)),
	    0,

	    scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) -
	               std::cos(rotate.x) * std::sin(rotate.z)),
	    scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) +
	               std::cos(rotate.x) * std::cos(rotate.z)),
	    scale.y * (std::sin(rotate.x) * std::cos(rotate.y)),
	    0,

	    scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) +
	               std::sin(rotate.x) * std::sin(rotate.z)),
	    scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) -
	               std::sin(rotate.x) * std::cos(rotate.z)),
	    scale.z * (std::cos(rotate.x) * std::cos(rotate.y)),
	    0,

	    translate.x,
	    translate.y,
	    translate.z,
	    1};
}

Matrix4x4 Matrix4x4::EulerRotate(EulerAngle eulerAngle, float angle) {
	switch (eulerAngle) {
	case Matrix4x4::Pitch:
		return Matrix4x4{1,
		                 0,
		                 0,
		                 0,
		                 0,
		                 std::cos(angle),
		                 std::sin(angle),
		                 0,
		                 0,
		                 -std::sin(angle),
		                 std::cos(angle),
		                 0,
		                 0,
		                 0,
		                 0,
		                 1};
		break;
	case Matrix4x4::Yaw:
		return Matrix4x4{std::cos(angle), 0, -std::sin(angle), 0, 0, 1, 0, 0,
		                 std::sin(angle), 0, std::cos(angle),  0, 0, 0, 0, 1};
		break;
	case Matrix4x4::Roll:
		return Matrix4x4{
		    std::cos(angle),
		    std::sin(angle),
		    0,
		    0,
		    -std::sin(angle),
		    std::cos(angle),
		    0,
		    0,
		    0,
		    0,
		    1,
		    0,
		    0,
		    0,
		    0,
		    1};
		break;
	default:
		return Identity();
		break;
	}
}

Matrix4x4 Matrix4x4::EulerRotate(const Vector3& rotate) {
	return Matrix4x4{
	    std::cos(rotate.y) * std::cos(rotate.z),
	    std::cos(rotate.y) * std::sin(rotate.z),
	    -std::sin(rotate.y),
	    0,

	    std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) -
	        std::cos(rotate.x) * std::sin(rotate.z),
	    std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) +
	        std::cos(rotate.x) * std::cos(rotate.z),
	    std::sin(rotate.x) * std::cos(rotate.y),
	    0,

	    std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) +
	        std::sin(rotate.x) * std::sin(rotate.z),
	    std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) -
	        std::sin(rotate.x) * std::cos(rotate.z),
	    std::cos(rotate.x) * std::cos(rotate.y),
	    0,

	    0,
	    0,
	    0,
	    1};
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& Second) const {
	return (Matrix4x4{
	    this->m[0][0] + Second.m[0][0], this->m[0][1] + Second.m[0][1],
	    this->m[0][2] + Second.m[0][2], this->m[0][3] + Second.m[0][3],

	    this->m[1][0] + Second.m[1][0], this->m[1][1] + Second.m[1][1],
	    this->m[1][2] + Second.m[1][2], this->m[1][3] + Second.m[1][3],

	    this->m[2][0] + Second.m[2][0], this->m[2][1] + Second.m[2][1],
	    this->m[2][2] + Second.m[2][2], this->m[2][3] + Second.m[2][3],

	    this->m[3][0] + Second.m[3][0], this->m[3][1] + Second.m[3][1],
	    this->m[3][2] + Second.m[3][2], this->m[3][3] + Second.m[3][3]});
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& Second) const {
	return (Matrix4x4{
	    this->m[0][0] - Second.m[0][0], this->m[0][1] - Second.m[0][1],
	    this->m[0][2] - Second.m[0][2], this->m[0][3] - Second.m[0][3],

	    this->m[1][0] - Second.m[1][0], this->m[1][1] - Second.m[1][1],
	    this->m[1][2] - Second.m[1][2], this->m[1][3] - Second.m[1][3],

	    this->m[2][0] - Second.m[2][0], this->m[2][1] - Second.m[2][1],
	    this->m[2][2] - Second.m[2][2], this->m[2][3] - Second.m[2][3],

	    this->m[3][0] - Second.m[3][0], this->m[3][1] - Second.m[3][1],
	    this->m[3][2] - Second.m[3][2], this->m[3][3] - Second.m[3][3]});
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& Second) const {

	return (Matrix4x4{
	    this->m[0][0] * Second.m[0][0] + this->m[0][1] * Second.m[1][0] +
	        this->m[0][2] * Second.m[2][0] + this->m[0][3] * Second.m[3][0],
	    this->m[0][0] * Second.m[0][1] + this->m[0][1] * Second.m[1][1] +
	        this->m[0][2] * Second.m[2][1] + this->m[0][3] * Second.m[3][1],
	    this->m[0][0] * Second.m[0][2] + this->m[0][1] * Second.m[1][2] +
	        this->m[0][2] * Second.m[2][2] + this->m[0][3] * Second.m[3][2],
	    this->m[0][0] * Second.m[0][3] + this->m[0][1] * Second.m[1][3] +
	        this->m[0][2] * Second.m[2][3] + this->m[0][3] * Second.m[3][3],

	    this->m[1][0] * Second.m[0][0] + this->m[1][1] * Second.m[1][0] +
	        this->m[1][2] * Second.m[2][0] + this->m[1][3] * Second.m[3][0],
	    this->m[1][0] * Second.m[0][1] + this->m[1][1] * Second.m[1][1] +
	        this->m[1][2] * Second.m[2][1] + this->m[1][3] * Second.m[3][1],
	    this->m[1][0] * Second.m[0][2] + this->m[1][1] * Second.m[1][2] +
	        this->m[1][2] * Second.m[2][2] + this->m[1][3] * Second.m[3][2],
	    this->m[1][0] * Second.m[0][3] + this->m[1][1] * Second.m[1][3] +
	        this->m[1][2] * Second.m[2][3] + this->m[1][3] * Second.m[3][3],

	    this->m[2][0] * Second.m[0][0] + this->m[2][1] * Second.m[1][0] +
	        this->m[2][2] * Second.m[2][0] + this->m[2][3] * Second.m[3][0],
	    this->m[2][0] * Second.m[0][1] + this->m[2][1] * Second.m[1][1] +
	        this->m[2][2] * Second.m[2][1] + this->m[2][3] * Second.m[3][1],
	    this->m[2][0] * Second.m[0][2] + this->m[2][1] * Second.m[1][2] +
	        this->m[2][2] * Second.m[2][2] + this->m[2][3] * Second.m[3][2],
	    this->m[2][0] * Second.m[0][3] + this->m[2][1] * Second.m[1][3] +
	        this->m[2][2] * Second.m[2][3] + this->m[2][3] * Second.m[3][3],

	    this->m[3][0] * Second.m[0][0] + this->m[3][1] * Second.m[1][0] +
	        this->m[3][2] * Second.m[2][0] + this->m[3][3] * Second.m[3][0],
	    this->m[3][0] * Second.m[0][1] + this->m[3][1] * Second.m[1][1] +
	        this->m[3][2] * Second.m[2][1] + this->m[3][3] * Second.m[3][1],
	    this->m[3][0] * Second.m[0][2] + this->m[3][1] * Second.m[1][2] +
	        this->m[3][2] * Second.m[2][2] + this->m[3][3] * Second.m[3][2],
	    this->m[3][0] * Second.m[0][3] + this->m[3][1] * Second.m[1][3] +
	        this->m[3][2] * Second.m[2][3] + this->m[3][3] * Second.m[3][3],
	});
}

Matrix4x4 Matrix4x4::operator*(const float& Second) const {
	return (Matrix4x4{
	    this->m[0][0] * Second, this->m[0][1] * Second, this->m[0][2] * Second,
	    this->m[0][3] * Second,

	    this->m[1][0] * Second, this->m[1][1] * Second, this->m[1][2] * Second,
	    this->m[1][3] * Second,

	    this->m[2][0] * Second, this->m[2][1] * Second, this->m[2][2] * Second,
	    this->m[2][3] * Second,

	    this->m[3][0] * Second, this->m[3][1] * Second, this->m[3][2] * Second,
	    this->m[3][3] * Second});
}

Matrix4x4 Matrix4x4::operator/(const float& Second) const {
	return (Matrix4x4{
	    this->m[0][0] / Second, this->m[0][1] / Second, this->m[0][2] / Second,
	    this->m[0][3] / Second,

	    this->m[1][0] / Second, this->m[1][1] / Second, this->m[1][2] / Second,
	    this->m[1][3] / Second,

	    this->m[2][0] / Second, this->m[2][1] / Second, this->m[2][2] / Second,
	    this->m[2][3] / Second,

	    this->m[3][0] / Second, this->m[3][1] / Second, this->m[3][2] / Second,
	    this->m[3][3] / Second});
}

Matrix4x4 Matrix4x4::operator+=(const Matrix4x4& Second) {

	this->m[0][0] += Second.m[0][0], this->m[0][1] += Second.m[0][1];
	this->m[0][2] += Second.m[0][2], this->m[0][3] += Second.m[0][3];

	this->m[1][0] += Second.m[1][0], this->m[1][1] += Second.m[1][1];
	this->m[1][2] += Second.m[1][2], this->m[1][3] += Second.m[1][3];

	this->m[2][0] += Second.m[2][0], this->m[2][1] += Second.m[2][1];
	this->m[2][2] += Second.m[2][2], this->m[2][3] += Second.m[2][3];

	this->m[3][0] += Second.m[3][0], this->m[3][1] += Second.m[3][1];
	this->m[3][2] += Second.m[3][2], this->m[3][3] += Second.m[3][3];

	return *this;
}

Matrix4x4 Matrix4x4::operator-=(const Matrix4x4& Second) {

	this->m[0][0] -= Second.m[0][0], this->m[0][1] -= Second.m[0][1];
	this->m[0][2] -= Second.m[0][2], this->m[0][3] -= Second.m[0][3];

	this->m[1][0] -= Second.m[1][0], this->m[1][1] -= Second.m[1][1];
	this->m[1][2] -= Second.m[1][2], this->m[1][3] -= Second.m[1][3];

	this->m[2][0] -= Second.m[2][0], this->m[2][1] -= Second.m[2][1];
	this->m[2][2] -= Second.m[2][2], this->m[2][3] -= Second.m[2][3];

	this->m[3][0] -= Second.m[3][0], this->m[3][1] -= Second.m[3][1];
	this->m[3][2] -= Second.m[3][2], this->m[3][3] -= Second.m[3][3];

	return *this;
}

Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& Second) {

	return *this = Matrix4x4{
	           this->m[0][0] * Second.m[0][0] + this->m[0][1] * Second.m[1][0] +
	               this->m[0][2] * Second.m[2][0] + this->m[0][3] * Second.m[3][0],
	           this->m[0][0] * Second.m[0][1] + this->m[0][1] * Second.m[1][1] +
	               this->m[0][2] * Second.m[2][1] + this->m[0][3] * Second.m[3][1],
	           this->m[0][0] * Second.m[0][2] + this->m[0][1] * Second.m[1][2] +
	               this->m[0][2] * Second.m[2][2] + this->m[0][3] * Second.m[3][2],
	           this->m[0][0] * Second.m[0][3] + this->m[0][1] * Second.m[1][3] +
	               this->m[0][2] * Second.m[2][3] + this->m[0][3] * Second.m[3][3],

	           this->m[1][0] * Second.m[0][0] + this->m[1][1] * Second.m[1][0] +
	               this->m[1][2] * Second.m[2][0] + this->m[1][3] * Second.m[3][0],
	           this->m[1][0] * Second.m[0][1] + this->m[1][1] * Second.m[1][1] +
	               this->m[1][2] * Second.m[2][1] + this->m[1][3] * Second.m[3][1],
	           this->m[1][0] * Second.m[0][2] + this->m[1][1] * Second.m[1][2] +
	               this->m[1][2] * Second.m[2][2] + this->m[1][3] * Second.m[3][2],
	           this->m[1][0] * Second.m[0][3] + this->m[1][1] * Second.m[1][3] +
	               this->m[1][2] * Second.m[2][3] + this->m[1][3] * Second.m[3][3],

	           this->m[2][0] * Second.m[0][0] + this->m[2][1] * Second.m[1][0] +
	               this->m[2][2] * Second.m[2][0] + this->m[2][3] * Second.m[3][0],
	           this->m[2][0] * Second.m[0][1] + this->m[2][1] * Second.m[1][1] +
	               this->m[2][2] * Second.m[2][1] + this->m[2][3] * Second.m[3][1],
	           this->m[2][0] * Second.m[0][2] + this->m[2][1] * Second.m[1][2] +
	               this->m[2][2] * Second.m[2][2] + this->m[2][3] * Second.m[3][2],
	           this->m[2][0] * Second.m[0][3] + this->m[2][1] * Second.m[1][3] +
	               this->m[2][2] * Second.m[2][3] + this->m[2][3] * Second.m[3][3],

	           this->m[3][0] * Second.m[0][0] + this->m[3][1] * Second.m[1][0] +
	               this->m[3][2] * Second.m[2][0] + this->m[3][3] * Second.m[3][0],
	           this->m[3][0] * Second.m[0][1] + this->m[3][1] * Second.m[1][1] +
	               this->m[3][2] * Second.m[2][1] + this->m[3][3] * Second.m[3][1],
	           this->m[3][0] * Second.m[0][2] + this->m[3][1] * Second.m[1][2] +
	               this->m[3][2] * Second.m[2][2] + this->m[3][3] * Second.m[3][2],
	           this->m[3][0] * Second.m[0][3] + this->m[3][1] * Second.m[1][3] +
	               this->m[3][2] * Second.m[2][3] + this->m[3][3] * Second.m[3][3],
	       };
}
Matrix4x4 Matrix4x4::operator*=(const float& Second) {

	this->m[0][0] *= Second;
	this->m[0][1] *= Second;
	this->m[0][2] *= Second;
	this->m[0][3] *= Second;

	this->m[1][0] *= Second;
	this->m[1][1] *= Second;
	this->m[1][2] *= Second;
	this->m[1][3] *= Second;

	this->m[2][0] *= Second;
	this->m[2][1] *= Second;
	this->m[2][2] *= Second;
	this->m[2][3] *= Second;

	this->m[3][0] *= Second;
	this->m[3][1] *= Second;
	this->m[3][2] *= Second;
	this->m[3][3] *= Second;

	return *this;
}

Matrix4x4 Matrix4x4::operator/=(const float& Second) {

	this->m[0][0] /= Second;
	this->m[0][1] /= Second;
	this->m[0][2] /= Second;
	this->m[0][3] /= Second;

	this->m[1][0] /= Second;
	this->m[1][1] /= Second;
	this->m[1][2] /= Second;
	this->m[1][3] /= Second;

	this->m[2][0] /= Second;
	this->m[2][1] /= Second;
	this->m[2][2] /= Second;
	this->m[2][3] /= Second;

	this->m[3][0] /= Second;
	this->m[3][1] /= Second;
	this->m[3][2] /= Second;
	this->m[3][3] /= Second;
	return *this;
}