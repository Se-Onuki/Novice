#pragma once


class Matrix3x3 {
public:
	inline Matrix3x3() : m{ {0, 0, 0}, {0, 0, 0}, {0, 0, 0} } {	}

	inline Matrix3x3(float A, float B, float C, float D, float E, float F, float G, float H, float I) : m{ {A, B, C}, {D, E, F}, {G, H, I} } {	}

	float m[3][3];

	void Printf(const int &x, const int &y) const;

	/// <summary>
	/// �t�s��֐�
	/// </summary>
	/// <returns>�t�s��</returns>
	Matrix3x3 Inverse();

	/// <summary>
	/// �]�u�s��
	/// </summary>
	/// <returns>�]�u�s��</returns>
	Matrix3x3 Transpose();

	Matrix3x3 operator+(const Matrix3x3 &Second) const;

	Matrix3x3 operator-(const Matrix3x3 &Second) const;

	Matrix3x3 operator*(const Matrix3x3 &Second) const;


	Matrix3x3 operator*(const float &Second) const;
	Matrix3x3 operator/(const float &Second) const;



	void operator+=(const Matrix3x3 &Second);

	void operator-=(const Matrix3x3 &Second);

	void operator*=(const Matrix3x3 &Second);


	void operator*=(const float &Second);
	void operator/=(const float &Second);


	/// <summary>
	/// �P�ʍs��
	/// </summary>
	/// <returns>�P�ʍs��</returns>
	static Matrix3x3 Identity(){
		return Matrix3x3{
			1, 0, 0,
		 	0, 1, 0,
			0, 0, 1
		};
	}
};
