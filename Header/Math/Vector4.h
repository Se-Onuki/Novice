#pragma once

#include <cmath>
#include <immintrin.h>

struct Vector4 {

	float x;
	float y;
	float z;
	float w;

	/// <summary>
	/// ベクトルをデバック表示する
	/// </summary>
	/// <param name="x">x座標</param>
	/// <param name="y">y座標</param>
	// void Printf(int x, int y) const;

	/// <summary>
	/// ベクトル長関数
	/// </summary>
	/// <returns>ベクトルの長さ</returns>
	_NODISCARD float Length() const { return sqrtf((*this) * (*this)); }

	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>ベクトル長が1のベクトル</returns>
	_NODISCARD Vector4 Nomalize() const {

		float Length = this->Length();
		if (Length != 0) {
			return *this / Length;
		} else {
			return zero();
		}
	}

	_NODISCARD Vector4 operator+(const Vector4& v) const {
		__m128 self = _mm_set_ps(w, z, y, x);
		__m128 other = _mm_set_ps(v.w, v.z, v.y, v.x);
		__m128 result = _mm_add_ps(self, other);

		return *reinterpret_cast<Vector4*>(&result);
	}
	_NODISCARD Vector4 operator-(const Vector4& v) const {
		__m128 self = _mm_set_ps(w, z, y, x);
		__m128 other = _mm_set_ps(v.w, v.z, v.y, v.x);
		__m128 result = _mm_sub_ps(self, other);

		return *reinterpret_cast<Vector4*>(&result);
	}

	Vector4& operator+=(const Vector4& value) {
		Vector4 buff = *this + value;
		std::memcpy(this, &buff, 4);
		return *this;
	}
	Vector4& operator-=(const Vector4& value) {
		Vector4 buff = *this - value;
		std::memcpy(this, &buff, 4);
		return *this;
	}

	_NODISCARD Vector4 operator*(const float& value) const {
		const __m128 self = _mm_set_ps(w, z, y, x); // floatが4つの { x, y, z, 0.f } に変換
		const __m128 other = _mm_set_ps1(value); // スカラを { value, value, value, value } に変換
		const __m128 result = _mm_mul_ps(self, other); // ベクタとスカラの各要素を乗算

		return *reinterpret_cast<const Vector4*>(&result);
		// float{ 0, 1, 2, 3 } の順序で構成される__m128型を、
		// float{ x, y, z, w } で構成されるVector4に解釈して変換
	}
	_NODISCARD Vector4 operator/(const float& value) const {
		const __m128 self = _mm_set_ps(w, z, y, x); // floatが4つの { x, y, z, 0.f } に変換
		const __m128 other = _mm_set_ps1(value); // スカラを { value, value, value, value } に変換
		const __m128 result = _mm_div_ps(self, other); // ベクタとスカラの各要素を乗算

		return *reinterpret_cast<const Vector4*>(&result);
		// float{ 0, 1, 2, 3 } の順序で構成される__m128型を、
		// float{ x, y, z, w } で構成されるVector4に解釈して変換
	}

	Vector4& operator*=(const float& value) {
		Vector4 buff = *this * value;
		std::memcpy(this, &buff, 4);
		return *this;
	}
	Vector4& operator/=(const float& value) {
		Vector4 buff = *this / value;
		std::memcpy(this, &buff, 4);
		return *this;
	}

	// Vector3 operator*(const Matrix3x3& Second) const {}

	// Vector3 operator*=(const Matrix3x3& Second) {}

	// Vector4 operator*(const Matrix4x4& Second) const;

	// Vector4& operator*=(const Matrix4x4& Second);

	// 逆ベクトル
	_NODISCARD inline Vector4 operator-() const { return *this * -1; }

	// 内積
	_NODISCARD inline float operator*(const Vector4& v) const {

		const __m128 self = _mm_set_ps(w, z, y, x);
		const __m128 other = _mm_set_ps(v.w, v.z, v.y, v.x);
		const __m128 result = _mm_dp_ps(self, other, 0x71);

		return _mm_cvtss_f32(result);
	}
	// 外積
	// inline float operator^(const Vector3& v) const { return x * v.y - y * v.x; }

	_NODISCARD inline static Vector4 zero() { return Vector4{0, 0, 0, 0}; }

	// inline Vector4 Reflect(Vector4 normal) const {
	//	return (*this) - normal * 2 * ((*this) * normal);

	//	// return {this->x- 2}
	//}

	_NODISCARD inline bool operator==(const Vector4& vec) const {
		return (this->x == vec.x) && (this->y == vec.y) && (this->z == vec.z) && (this->w == vec.w);
	}

private:
};