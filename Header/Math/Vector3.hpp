//#pragma once
//#include "Matrix3x3.hpp"
//
//#include <cmath>
//
//class Vector3 {
//public:
//	Vector3(float x = 0.f, float y = 0.f, float z = 0.f) :x(x),y(y),z(z){}
//
//	float x;
//	float y;
//	float z;
//
//	/// <summary>
//	/// ベクトルをデバック表示する
//	/// </summary>
//	/// <param name="x">x座標</param>
//	/// <param name="y">y座標</param>
//	void Printf(int x, int y) const;
//
//	/// <summary>
//	/// ベクトル長関数
//	/// </summary>
//	/// <returns>ベクトルの長さ</returns>
//	float Length() const { return 0; }
//
//	/// <summary>
//	/// 正規化
//	/// </summary>
//	/// <returns>ベクトル長が1のベクトル</returns>
//	Vector3 Nomalize() const;
//
//	Vector3 operator+(const Vector3& Second) const {}
//	Vector3 operator-(const Vector3& Second) const {}
//
//	void operator+=(const Vector3& Second) {}
//	void operator-=(const Vector3& Second) {}
//
//	Vector3 operator*(const float& Second) const {}
//	Vector3 operator/(const float& Second) const {}
//
//	void operator*=(const float& Second){}
//	void operator/=(const float& Second){}
//
//	Vector3 operator*(const Matrix3x3& Second) const {}
//
//	void operator*=(const Matrix3x3& Second) {}
//
//	//Vector3 operator*(const Matrix4x4& Second) const {}
//
//	//void operator*=(const Matrix4x4& Second){}
//
//
//	// 逆ベクトル
//	inline Vector3 operator-() const { return *this * -1; }
//
//	// 内積
//	inline float operator*(const Vector3& v) const { return x * v.x + y * v.y; }
//	// 外積
//	// inline float operator^(const Vector3& v) const { return x * v.y - y * v.x; }
//
//	inline static Vector3 zero() { return {0, 0}; }
//
//	inline Vector3 Reflect(Vector3 normal) const {
//		return (*this) - normal * 2 * ((*this) * normal);
//
//		// return {this->x- 2}
//	}
//
//	inline bool operator==(const Vector3& vec) const {
//		return (this->x == vec.x) && (this->y == vec.y);
//	}
//
//private:
//};