#pragma once
// #include "Matrix3x3.hpp"

#include <cmath>
#include <numbers>

#include <immintrin.h>

struct Matrix4x4;

struct Vector3 {
	// Vector3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}

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

	/// @brief 2乗ベクトル長関数
	/// @return ベクトル長
	[[nodiscard]] float LengthSQ() const { return (*this) * (*this); }

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

	[[nodiscard]] Vector3 operator+(const Vector3& v) const {
		__m128 self = _mm_set_ps(0.0f, z, y, x);
		__m128 other = _mm_set_ps(0.0f, v.z, v.y, v.x);
		__m128 result = _mm_add_ps(self, other);

		return *reinterpret_cast<Vector3*>(&result);
	}
	[[nodiscard]] Vector3 operator-(const Vector3& v) const {
		__m128 self = _mm_set_ps(0.0f, z, y, x);
		__m128 other = _mm_set_ps(0.0f, v.z, v.y, v.x);
		__m128 result = _mm_sub_ps(self, other);

		return *reinterpret_cast<Vector3*>(&result);
	}

	Vector3& operator+=(const Vector3& v) {
		Vector3 buff = *this + v;
		std::memcpy(this, &buff, 3);
		return *this;
	}
	Vector3& operator-=(const Vector3& v) {
		Vector3 buff = *this - v;
		std::memcpy(this, &buff, 3);
		return *this;
	}

	[[nodiscard]] Vector3 operator*(const float& value) const {
		const __m128 self = _mm_set_ps(0.0f, z, y, x); // floatが4つの { x, y, z, 0.f } に変換
		const __m128 other = _mm_set_ps1(value); // スカラを { value, value, value, value } に変換
		const __m128 result = _mm_mul_ps(self, other); // ベクタとスカラの各要素を乗算

		return *reinterpret_cast<const Vector3*>(&result);
		// float{ 0, 1, 2, 3 } の順序で構成される__m128型を、
		// float{ x, y, z } で構成されるVector3に解釈して変換
	}
	[[nodiscard]] Vector3 operator/(const float& value) const {
		const __m128 self = _mm_set_ps(0.0f, z, y, x); // floatが4つの { x, y, z, 0.f } に変換
		const __m128 other = _mm_set_ps1(value); // スカラを { value, value, value, value } に変換
		const __m128 result = _mm_div_ps(self, other); // ベクタとスカラの各要素を除算

		return *reinterpret_cast<const Vector3*>(&result);
		// float{ x, y, z, 0.f } の順序で構成される__m128型を、
		// float{ x, y, z } で構成されるVector3に解釈して変換
	}

	Vector3& operator*=(const float& value) {
		Vector3 buff = *this * value;
		std::memcpy(this, &buff, 3);
		return *this;
	}
	Vector3& operator/=(const float& value) {
		Vector3 buff = *this / value;
		std::memcpy(this, &buff, 3);
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

		const __m128 self = _mm_set_ps(0.f, z, y, x);
		const __m128 other = _mm_set_ps(0.f, v.z, v.y, v.x);
		const __m128 result = _mm_dp_ps(self, other, 0x71);

		return _mm_cvtss_f32(result);
	}
	// 外積(クロス積)
	[[nodiscard]] inline Vector3 cross(const Vector3& v) const {
		return Vector3{y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
	}

	//[[nodiscard]] inline Vector3 Reflect(Vector3 normal) const {
	//	return (*this) - normal * 2 * ((*this) * normal);

	//	// return {this->x- 2}
	//}

	/// @brief 垂直ベクトル
	/// @return 90度回転したベクトル
	inline Vector3 Perpendicular() const {
		if (x != 0.f || y != 0.f)
			return Vector3{-y, x, 0.f};
		return Vector3{0.f, -z, y};
	}

	Vector3 Direction2Euler() const {
		// return Vector3{std::atan2(-y, sqrt(x * x + z * z)), std::atan2(x, z), 0};
		Vector3 out = zero();
		out.y = std::atan2(x, z);
		out.x = std::atan2(-y, std::sqrt(std::powf(x, 2) + std::powf(z, 2)));
		return out;
	}

	inline static Vector3 zero() { return Vector3{0.f, 0.f, 0.f}; }

	static Vector3 up() { return Vector3{0, 1, 0}; }
	static Vector3 down() { return Vector3{0, -1, 0}; }

	static Vector3 front() { return Vector3{0, 0, 1}; }
	static Vector3 back() { return Vector3{0, 0, -1}; }

	static Vector3 right() { return Vector3{1, 0, 0}; }
	static Vector3 left() { return Vector3{-1, 0, 0}; }

	static Vector3 one() { return Vector3{1.f, 1.f, 1.f}; }

	// inline operator bool() const { return x || y || z; }
	inline bool operator==(const Vector3& vec) const {
		return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z);
	}

private:
};

[[nodiscard]] inline Vector3 operator*(const float& left, const Vector3& right) {
	return right * left;
}