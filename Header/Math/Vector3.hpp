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
//	/// �x�N�g�����f�o�b�N�\������
//	/// </summary>
//	/// <param name="x">x���W</param>
//	/// <param name="y">y���W</param>
//	void Printf(int x, int y) const;
//
//	/// <summary>
//	/// �x�N�g�����֐�
//	/// </summary>
//	/// <returns>�x�N�g���̒���</returns>
//	float Length() const { return 0; }
//
//	/// <summary>
//	/// ���K��
//	/// </summary>
//	/// <returns>�x�N�g������1�̃x�N�g��</returns>
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
//	// �t�x�N�g��
//	inline Vector3 operator-() const { return *this * -1; }
//
//	// ����
//	inline float operator*(const Vector3& v) const { return x * v.x + y * v.y; }
//	// �O��
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