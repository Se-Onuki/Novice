#include "Vector2.hpp"

#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "Matrix2x2.hpp"
#include"Math.hpp"

#include <assert.h>


void Vector2::Reset() {
	*this = ZeroVector2;
}

Vector2 Vector2::Rotate(const float &theta) const {
	return *this * MakeRotateMatrix(theta);
}

float Vector2::Length() const {
	return sqrt(pow(x, 2) + pow(y, 2));
}

Vector2 Vector2::Nomalize() const {
	float Length = this->Length();
	if (Length != 0) {
		return *this / Length;
	}
	else {
		return ZeroVector2;
	}
}

void Vector2::Printf(int x, int y) const {
	Novice::ScreenPrintf(x, y, "%12.2f // %6.2f", this->x, Length());
	Novice::ScreenPrintf(x, y + TextHeight, "%12.2f", this->y);
}

Vector2 Vector2::operator+(const Vector2 &Second) const {
	return { (this->x) + (Second.x), (this->y) + (Second.y) };
}

Vector2 Vector2::operator-(const Vector2 &Second) const {
	return { (this->x) - (Second.x), (this->y) - (Second.y) };
}

void Vector2::operator +=(const Vector2 &Second) {
	this->x += Second.x;
	this->y += Second.y;
}

void Vector2::operator -=(const Vector2 &Second) {
	this->x -= Second.x;
	this->y -= Second.y;
}

Vector2 Vector2::operator*(const float &Second) const {
	return { (this->x) * (Second), (this->y) * (Second) };
}

Vector2 Vector2::operator/(const float &Second) const {
	return { (this->x) / (Second), (this->y) / (Second) };
}

void Vector2::operator *=(const float &Second) {
	this->x *= Second;
	this->y *= Second;
}


void Vector2::operator /=(const float &Second) {
	this->x /= Second;
	this->y /= Second;
}

Vector2 Vector2::operator*(const Matrix2x2 &Second) const {
	const Vector2 b[2]{ {Second.m[0][0],Second.m[1][0]},{Second.m[0][1],Second.m[1][1]} };
	return Vector2{
		DotProduct(*this, b[0]),
		DotProduct(*this, b[1])
	};
}

void Vector2::operator*=(const Matrix2x2 &Second) {
	const Vector2 b[2]{ {Second.m[0][0],Second.m[1][0]},{Second.m[0][1],Second.m[1][1]} };
	*this = {
		DotProduct(*this, b[0]),
		DotProduct(*this, b[1])
	};
}

Vector2 Vector2::operator*(const Matrix3x3 &matrix) const {
	Vector2 result;

	result.x = x * matrix.m[0][0] + y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	result.y = x * matrix.m[0][1] + y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	const float w = x * matrix.m[0][2] + y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	return result / w; // ���Z�q�̃I�[�o�[���C�h
}

void Vector2::operator*=(const Matrix3x3 &matrix) {
	this->x = x * matrix.m[0][0] + y * matrix.m[1][0] + 1.0f * matrix.m[2][0];
	this->y = x * matrix.m[0][1] + y * matrix.m[1][1] + 1.0f * matrix.m[2][1];
	const float w = x * matrix.m[0][2] + y * matrix.m[1][2] + 1.0f * matrix.m[2][2];
	assert(w != 0.0f);
	*this / w; // ���Z�q�̃I�[�o�[���C�h
}


Vector2::Vector2() : x(0), y(0)
{
}


Vector2::Vector2(float x, float y) : x(x), y(y)
{
}
