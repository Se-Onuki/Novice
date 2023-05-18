#pragma once
// #include "Matrix3x3.hpp"

#include <cmath>

struct Matrix4x4;

struct Vector3 {
	Vector3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

	float x;
	float y;
	float z;

	/// <summary>
	/// ベクトルをデバック表示する
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	void Printf(int x, int y) const;

	/// <summary>
	/// ベクトル長関数
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	_NODISCARD float Length() const { return sqrtf((*this) * (*this)); }

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>ベクトル長が1のベクトル</returns>
	_NODISCARD Vector3 Nomalize() const {

		float Length = this->Length();
		if (Length != 0) {
			return *this / Length;
		} else {
			return zero();
		}
	}

	_NODISCARD Vector3 operator+(const Vector3& Second) const {
		return Vector3{this->x + Second.x, this->y + Second.y, this->z + Second.z};
	}
	_NODISCARD Vector3 operator-(const Vector3& Second) const {
		return Vector3{this->x - Second.x, this->y - Second.y, this->z - Second.z};
	}

	Vector3& operator+=(const Vector3& Second) {
		this->x += Second.x;
		this->y += Second.y;
		this->z += Second.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& Second) {
		this->x -= Second.x;
		this->y -= Second.y;
		this->z -= Second.z;
		return *this;
	}

	_NODISCARD Vector3 operator*(const float& Second) const {
		return Vector3{this->x * Second, this->y * Second, this->z * Second};
	}
	_NODISCARD Vector3 operator/(const float& Second) const {
		return Vector3{this->x / Second, this->y / Second, this->z / Second};
	}

	Vector3& operator*=(const float& Second) {
		this->x *= Second;
		this->y *= Second;
		this->z *= Second;
		return *this;
	}
	Vector3& operator/=(const float& Second) {
		this->x /= Second;
		this->y /= Second;
		this->z /= Second;
		return *this;
	}

	// Vector3 operator*(const Matrix3x3& Second) const {}

	// Vector3 operator*=(const Matrix3x3& Second) {}

	_NODISCARD Vector3 operator*(const Matrix4x4& Second) const;

	Vector3& operator*=(const Matrix4x4& Second);

	// 逆ベクトル
	_NODISCARD inline Vector3 operator-() const { return *this * -1; }

	// 内積
	_NODISCARD inline float operator*(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	// 外積(クロス積)
	_NODISCARD inline Vector3 operator^(const Vector3& v) const {
		return Vector3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}

	_NODISCARD inline static Vector3 zero() { return Vector3{0.f, 0.f, 0.f}; }

	_NODISCARD inline Vector3 Reflect(Vector3 normal) const {
		return (*this) - normal * 2 * ((*this) * normal);

		// return {this->x- 2}
	}

	_NODISCARD inline bool operator==(const Vector3& vec) const {
		return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z);
	}

private:
};