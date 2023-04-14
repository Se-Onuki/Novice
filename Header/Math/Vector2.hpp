#pragma once

#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

#include <cmath>

class Vector2
{
public:
	Vector2();
	Vector2(float x, float y);

	float x;
	float y;

	/// <summary>
	/// �x�N�g�����f�o�b�N�\������
	/// </summary>
	/// <param name="x">x���W</param>
	/// <param name="y">y���W</param>
	void Printf(int x, int y) const;

	/// <summary>
	/// �x�N�g������]������
	/// </summary>
	/// <param name="theta">��]�p</param>
	/// <returns>��]�����x�N�g��</returns>
	Vector2 Rotate(const float& theta) const;

	/// <summary>
	/// �[���x�N�g��
	/// </summary>
	void Reset();

	/// <summary>
	/// �x�N�g�����֐�
	/// </summary>
	/// <returns>�x�N�g���̒���</returns>
	float Length() const;

	/// <summary>
	/// ���K��
	/// </summary>
	/// <returns>�x�N�g������1�̃x�N�g��</returns>
	Vector2 Nomalize() const;

	Vector2 operator+(const Vector2& Second) const;
	Vector2 operator-(const Vector2& Second) const;


	void operator +=(const Vector2& Second);
	void operator -=(const Vector2& Second);


	Vector2 operator*(const float& Second) const;
	Vector2 operator/(const float& Second) const;

	void operator *=(const float& Second);
	void operator /=(const float& Second);


	Vector2 operator*(const Matrix2x2& Second) const;

	void operator*=(const Matrix2x2& Second);

	Vector2 operator*(const Matrix3x3& Second) const;

	void operator*=(const Matrix3x3& Second);

	inline float GetTheta() const {
		if (*this == Vector2{ 0,0 }) {
			return 0;
		}
		return atan2(y, x);
	}

	// �t�x�N�g��
	inline Vector2 operator-() const { return *this * -1; }

	// ����
	inline float operator*(const Vector2& v) const { return x * v.x + y * v.y; }
	// �O��
	inline float operator^(const Vector2& v) const { return x * v.y - y * v.x; }


	inline static Vector2 zero() { return { 0,0 }; }

	inline Vector2 Reflect(Vector2 normal) const {
		return (*this) - normal * 2 * ((*this) * normal);

		//return {this->x- 2}
	}

	inline bool operator==(const Vector2& vec) const { return (this->x == vec.x) && (this->y == vec.y); }
private:

};

inline Vector2 operator*(const float& A, const Vector2& B) { return B * A; }



/// <summary>
	/// int�^�̓񎟌��x�N�g��
	/// </summary>
	/// <param name="x">��</param>
	/// <param name="y">�c</param>
class IntVector2 {
public:
	inline IntVector2() :x(0), y(0) {}
	inline IntVector2(Vector2 vec) : x(vec.x), y(vec.y) {}
	int x;
	int y;
};

const Vector2 ScreenSize{
	1280,
	720
};





/// �[���x�N�g��
const Vector2 ZeroVector2 = { 0,0 };




/// UI�Ȃǂ̊�l�ƂȂ���W
const Vector2 MiddleLeftOver = { (float)(-ScreenSize.x / 2), (float)(ScreenSize.y / 2) };
const Vector2 TopLeftOver = { (float)(-ScreenSize.x / 2), (float)(ScreenSize.y) };
const Vector2 DownLeftOver = { (float)(-ScreenSize.x / 2), 0 };

const Vector2 MiddleLeft = { 0, (float)(ScreenSize.y / 2) };
const Vector2 TopLeft = { 0, (float)(ScreenSize.y) };
const Vector2 DownLeft = { 0, 0 };

const Vector2 MiddleCentor = { (float)(ScreenSize.x / 2), (float)(ScreenSize.y / 2) };
const Vector2 TopCentor = { (float)(ScreenSize.x / 2), (float)(ScreenSize.y) };
const Vector2 DownCentor = { (float)(ScreenSize.x / 2), 0 };

const Vector2 MiddleRight = { (float)(ScreenSize.x) , (float)(ScreenSize.y / 2) };
const Vector2 TopRight = { (float)(ScreenSize.x) , (float)(ScreenSize.y) };
const Vector2 DownRight = { (float)(ScreenSize.x) , 0 };

const Vector2 MiddleRightOver = { (float)(ScreenSize.x * 1.5) , (float)(ScreenSize.y / 2) };
const Vector2 TopRightOver = { (float)(ScreenSize.x * 1.5) , (float)(ScreenSize.y) };
const Vector2 DownRightOver = { (float)(ScreenSize.x * 1.5) , 0 };

const Vector2 TopOverCentor = { (float)(ScreenSize.x * 0.5) , (float)(-ScreenSize.y / 2) };
const Vector2 DownOverCentor = { (float)(ScreenSize.x * 0.5) , (float)(ScreenSize.y * 1.5) };

