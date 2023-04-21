#pragma once

class Matrix4x4 {
public:
	inline Matrix4x4()
	    : m{
	          {0, 0, 0, 0},
              {0, 0, 0, 0},
              {0, 0, 0, 0},
              {0, 0, 0, 0}
    } {}

	inline Matrix4x4(
	    float A, float B, float C, float D,
		float E, float F, float G, float H,
		float I, float J, float K, float L, 
		float M, float N, float O, float P)
	    : m{
	          {A, B, C, D},
              {E, F, G, H},
              {I, J, K, L},
              {M, N, O, P}
    } {}

	float m[4][4];

	void Printf(const int& x, const int& y) const;

	/// <summary>
	/// 逆行列関数
	/// </summary>
	/// <returns>逆行列</returns>
	Matrix4x4 Inverse();

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <returns>転置行列</returns>
	Matrix4x4 Transpose();

	Matrix4x4 operator+(const Matrix4x4& Second) const;

	Matrix4x4 operator-(const Matrix4x4& Second) const;

	Matrix4x4 operator*(const Matrix4x4& Second) const;

	Matrix4x4 operator*(const float& Second) const;
	Matrix4x4 operator/(const float& Second) const;

	Matrix4x4 operator+=(const Matrix4x4& Second);

	Matrix4x4 operator-=(const Matrix4x4& Second);

	Matrix4x4 operator*=(const Matrix4x4& Second);

	Matrix4x4 operator*=(const float& Second);
	Matrix4x4 operator/=(const float& Second);

	/// <summary>
	/// 単位行列
	/// </summary>
	/// <returns>単位行列</returns>
	static Matrix4x4 Identity() {
		return Matrix4x4{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
	}
};
