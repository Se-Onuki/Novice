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
	[[nodiscard]] float Length() const { return sqrtf((*this) * (*this)); }

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>ベクトル長が1のベクトル</returns>
	[[nodiscard]] Vector3 Nomalize() const {

		float Length = this->Length();
		if (Length != 0) {
			return *this / Length;
		} else {
			return zero();
		}
	}

	[[nodiscard]] Vector3 operator+(const Vector3& Second) const {
		return Vector3{this->x + Second.x, this->y + Second.y, this->z + Second.z};
	}
	[[nodiscard]] Vector3 operator-(const Vector3& Second) const {
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

	[[nodiscard]] Vector3 operator*(const float& Second) const {
		return Vector3{this->x * Second, this->y * Second, this->z * Second};
	}
	[[nodiscard]] Vector3 operator/(const float& Second) const {
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

	[[nodiscard]] Vector3 operator*(const Matrix4x4& Second) const;

	Vector3& operator*=(const Matrix4x4& Second);

	// 逆ベクトル
	[[nodiscard]] inline Vector3 operator-() const { return *this * -1; }

	// 内積
	[[nodiscard]] inline float operator*(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}
	// 外積(クロス積)
	[[nodiscard]] inline Vector3 operator^(const Vector3& v) const {
		return Vector3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}

	[[nodiscard]] inline static Vector3 zero() { return Vector3{0.f, 0.f, 0.f}; }

	[[nodiscard]] inline Vector3 Reflect(Vector3 normal) const {
		return (*this) - normal * 2 * ((*this) * normal);

		// return {this->x- 2}
	}

	inline Vector3 Perpendicular() const {
		if (x != 0.f || y != 0.f)
			return Vector3{-y, x, 0.f};
		return Vector3{0.f, -z, y};
	}

	[[nodiscard]] inline bool operator==(const Vector3& vec) const {
		return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z);
	}

private:
};

[[nodiscard]] inline Vector3 operator*(const float& left, const Vector3& right) {
	return right * left;
}