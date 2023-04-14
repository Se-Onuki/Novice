#include"Math.hpp"

#define _USE_MATH_DEFINES
#include<math.h>

#include "Vector2.hpp"
#include "PolarCoordinates.hpp"

#include <assert.h>


float DotProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB) {
	Vector2 a = { ((endA.x) - (startA.x)),((endA.y) - (startA.y)) };
	Vector2 b = { ((endB.x) - (startB.x)),((endB.y) - (startB.y)) };

	return a.x * b.x + a.y * b.y;
}

float DotProduct(Vector2 VectorA, Vector2 VectorB) {
	return VectorA.x * VectorB.x + VectorA.y * VectorB.y;
}


float CrossProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB) {
	Vector2 a = { ((endA.x) - (startA.x)),((endA.y) - (startA.y)) };
	Vector2 b = { ((endB.x) - (startB.x)),((endB.y) - (startB.y)) };

	return a.x * b.y - a.y * b.x;
}
float CrossProduct(Vector2 VectorA, Vector2 VectorB) {
	return VectorA.x * VectorB.y - VectorA.y * VectorB.x;
}



Vector2 Polar2Rectangular(Polar Polar) {
	return { (float)(cos(Polar.theta) * Polar.radius),(float)(sin(Polar.theta) * Polar.radius) };
}

Polar Rectangular2Polar(Vector2 Vector) {
	return { (float)(atan2(Vector.y,Vector.x)),Vector.Length() };
}


Matrix2x2 MakeRotateMatrix(const float &theta) {
	return Matrix2x2(
		cos(theta), sin(theta),
		-sin(theta), cos(theta)
	);
}


Matrix3x3 MakeScaleMatrix3x3(const Vector2 &scale) {
	return Matrix3x3(
		scale.x, 0, 0,
		0, scale.y, 0,
		0, 0, 1
	);
}



Matrix3x3 MakeRotateMatrix3x3(const float &theta) {
	return Matrix3x3(
		cos(theta), sin(theta), 0,
		-sin(theta), cos(theta), 0,
		0, 0, 1
	);
}

Matrix3x3 MakeTranslateMatrix(const Vector2 &translate) {
	return Matrix3x3{
		1, 0, 0,
		0, 1, 0,
		translate.x, translate.y, 1
	};
}


Matrix3x3 MakeAffineMatrix(const Vector2 &scale, const float &theta, const Vector2 &translate) {
	return Matrix3x3{
		scale.x * (float)cos(theta) , scale.x * (float)sin(theta), 0,
		scale.y * (float)-sin(theta), scale.y * (float)cos(theta), 0,
		translate.x, translate.y, 1
	};
}


Vector2 Transform(const Vector2 &vector, const Matrix3x3 &matrix) {
	Vector2 result;

	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	const float w = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	return result / w; // 演算子のオーバーライド
}


Matrix3x3 MakeOrthographicMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom) {
	return
	{
		2.0f / (RightBottom.x - LeftTop.x), 0,0,
		0, 2.0f / (LeftTop.y - RightBottom.y),0,
		(RightBottom.x + LeftTop.x) / (LeftTop.x - RightBottom.x), (LeftTop.y + RightBottom.y) / (RightBottom.y - LeftTop.y), 1
	};
}

Matrix3x3 MakeViewportMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom) {
	return
	{
		(RightBottom.x - LeftTop.x) / 2.0f, 0,0,
		0, -(RightBottom.y - LeftTop.y) / 2.0f,0,
		(RightBottom.x - LeftTop.x) / 2.0f + LeftTop.x,(RightBottom.y - LeftTop.y) / 2.0f + LeftTop.y, 1
	};
}
