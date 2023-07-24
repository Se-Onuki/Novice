#include "Matrix4x4.h"

#include "Matrix2x2.h"
#include "Vector3.h"

#include <Novice.h>

#include "Math.hpp"
#include <cmath>
#include <immintrin.h>

void Matrix4x4::Printf(const int& x, const int& y) const {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			Novice::ScreenPrintf(
			    x + j * static_cast<int>(TextSize.x) * 2, y + i * static_cast<int>(TextSize.y),
			    "%10.6f", m[i][j]);
		}
	}
}

Matrix4x4 Matrix4x4::Inverse() const {
	Matrix4x4 calc = *this;
	Matrix4x4 out = Identity();

	for (size_t i = 0; i < 4; i++) {
		float value = calc.m[i][i];
		for (size_t k = 0; k < 4; k++) {
			calc.m[i][k] /= value;
			out.m[i][k] /= value;
		}
		for (size_t k = 0; k < 4; k++) {
			if (k != i) {
				value = -calc.m[k][i];
				for (size_t l = 0; l < 4; l++) {
					calc.m[k][l] += calc.m[i][l] * value;
					out.m[k][l] += out.m[i][l] * value;
				}
			}
		}
	}
	return out;
}

Matrix4x4 Matrix4x4::InverseRT() const {
	/*return Matrix4x4{
	    {m[0][0],	                                                m[1][0], m[2][0], 0.f},
	    {m[0][1],	                                                m[1][1], m[2][1], 0.f},
	    {m[0][2],	                                                m[1][2], m[2][2], 0.f},
	    {-m[3][0] * m[0][0] - m[3][1] * m[0][1] - m[3][2] * m[0][2],
	     -m[3][0] * m[1][0] - m[3][1] * m[1][1] - m[3][2] * m[1][2],
	     -m[3][0] * m[2][0] - m[3][1] * m[2][1] - m[3][2] * m[2][2],                   1.f}
	};*/
	return Matrix4x4{
	    {m[0][0], m[1][0], m[2][0], 0.f},
	    {m[0][1], m[1][1], m[2][1], 0.f},
	    {m[0][2], m[1][2], m[2][2], 0.f},
	    {-_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), _mm_load_ps(m[0]), 0x71)),
	     -_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), _mm_load_ps(m[1]), 0x71)),
	     -_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), _mm_load_ps(m[2]), 0x71)), 1.f}
    };
}
Matrix4x4 Matrix4x4::InverseSRT() const {
	const Vector4 vecX2 =
	    *(Vector4*)&m[0] / _mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[0]), _mm_load_ps(m[0]), 0x71));
	const Vector4 vecY2 =
	    *(Vector4*)&m[1] / _mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[1]), _mm_load_ps(m[1]), 0x71));
	const Vector4 vecZ2 =
	    *(Vector4*)&m[2] / _mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[2]), _mm_load_ps(m[2]), 0x71));

	return Matrix4x4{
	    {vecX2.x, vecY2.x, vecZ2.x, 0.f},
	    {vecX2.y, vecY2.y, vecZ2.y, 0.f},
	    {vecX2.z, vecY2.z, vecZ2.z, 0.f},
	    {-_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), *(__m128*)&vecX2, 0x71)),
	     -_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), *(__m128*)&vecY2, 0x71)),
	     -_mm_cvtss_f32(_mm_dp_ps(_mm_load_ps(m[3]), *(__m128*)&vecZ2, 0x71)), 1.f}
    };
};

Matrix4x4 Matrix4x4::Transpose() const {
	return Matrix4x4{
	    {m[0][0], m[1][0], m[2][0], m[3][0]},
	    {m[0][1], m[1][1], m[2][1], m[3][1]},
	    {m[0][2], m[1][2], m[2][2], m[3][2]},
	    {m[0][3], m[1][3], m[2][3], m[3][3]}
    };
}

Matrix4x4 Matrix4x4::Affine(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	return Matrix4x4{
	    {scale.x * (std::cos(rotate.y) * std::cos(rotate.z)),
	     scale.x * (std::cos(rotate.y) * std::sin(rotate.z)),                       scale.x * (-std::sin(rotate.y)), 0.f},

	    {scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) -
	                std::cos(rotate.x) * std::sin(rotate.z)),
	     scale.y * (std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) +
	                std::cos(rotate.x) * std::cos(rotate.z)),
	     scale.y * (std::sin(rotate.x) * std::cos(rotate.y)),                                                        0.f},

	    {scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) +
	                std::sin(rotate.x) * std::sin(rotate.z)),
	     scale.z * (std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) -
	                std::sin(rotate.x) * std::cos(rotate.z)),
	     scale.z * (std::cos(rotate.x) * std::cos(rotate.y)),                                                        0.f},

	    {translate.x,	                                              translate.y, translate.z,                     1.f}
    };
}

Matrix4x4 Matrix4x4::EulerRotate(EulerAngle eulerAngle, float angle) {
	switch (eulerAngle) {
	case Matrix4x4::Pitch:
		return Matrix4x4{
		    {1.f, 0.f,              0.f,             0.f},
		    {0.f, std::cos(angle),  std::sin(angle), 0.f},
		    {0.f, -std::sin(angle), std::cos(angle), 0.f},
		    {0.f, 0.f,              0.f,             1.f}
        };
		break;
	case Matrix4x4::Yaw:
		return Matrix4x4{
		    {std::cos(angle), 0.f, -std::sin(angle), 0.f},
		    {0.f,             1.f, 0.f,              0.f},
		    {std::sin(angle), 0,   std::cos(angle),  0.f},
		    {0.f,             0.f, 0.f,              1.f}
        };
		break;
	case Matrix4x4::Roll:
		return Matrix4x4{
		    {std::cos(angle),  std::sin(angle), 0.f, 0.f},
		    {-std::sin(angle), std::cos(angle), 0.f, 0.f},
		    {0.f,              0.f,             1.f, 0.f},
		    {0.f,              0.f,             0.f, 1.f}
        };
		break;
	default:
		return Identity();
		break;
	}
}

Matrix4x4 Matrix4x4::EulerRotate(const Vector3& rotate) {
	return Matrix4x4{
	    std::cos(rotate.y) * std::cos(rotate.z),
	    std::cos(rotate.y) * std::sin(rotate.z),
	    -std::sin(rotate.y),
	    0,

	    std::sin(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) -
	        std::cos(rotate.x) * std::sin(rotate.z),
	    std::sin(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) +
	        std::cos(rotate.x) * std::cos(rotate.z),
	    std::sin(rotate.x) * std::cos(rotate.y),
	    0,

	    std::cos(rotate.x) * std::sin(rotate.y) * std::cos(rotate.z) +
	        std::sin(rotate.x) * std::sin(rotate.z),
	    std::cos(rotate.x) * std::sin(rotate.y) * std::sin(rotate.z) -
	        std::sin(rotate.x) * std::cos(rotate.z),
	    std::cos(rotate.x) * std::cos(rotate.y),
	    0,

	    0,
	    0,
	    0,
	    1};
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& Second) const {
	/*return (Matrix4x4{
	    this->m[0][0] + Second.m[0][0], this->m[0][1] + Second.m[0][1],
	    this->m[0][2] + Second.m[0][2], this->m[0][3] + Second.m[0][3],

	    this->m[1][0] + Second.m[1][0], this->m[1][1] + Second.m[1][1],
	    this->m[1][2] + Second.m[1][2], this->m[1][3] + Second.m[1][3],

	    this->m[2][0] + Second.m[2][0], this->m[2][1] + Second.m[2][1],
	    this->m[2][2] + Second.m[2][2], this->m[2][3] + Second.m[2][3],

	    this->m[3][0] + Second.m[3][0], this->m[3][1] + Second.m[3][1],
	    this->m[3][2] + Second.m[3][2], this->m[3][3] + Second.m[3][3]});*/
	Matrix4x4 result;
	*(__m256*)&result.m[0] = _mm256_add_ps(_mm256_load_ps(m[0]), _mm256_load_ps(Second.m[0]));
	*(__m256*)&result.m[2] = _mm256_add_ps(_mm256_load_ps(m[2]), _mm256_load_ps(Second.m[2]));
	return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& Second) const {
	Matrix4x4 result;
	*(__m256*)&result.m[0] = _mm256_sub_ps(_mm256_load_ps(m[0]), _mm256_load_ps(Second.m[0]));
	*(__m256*)&result.m[2] = _mm256_sub_ps(_mm256_load_ps(m[2]), _mm256_load_ps(Second.m[2]));
	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& sec) const {
	Matrix4x4 result;
	__m128 row0 = _mm_load_ps(sec.m[0]);
	__m128 row1 = _mm_load_ps(sec.m[1]);
	__m128 row2 = _mm_load_ps(sec.m[2]);
	__m128 row3 = _mm_load_ps(sec.m[3]);
	for (int i = 0; i < 4; i++) {
		__m128 brod0 = _mm_set1_ps(m[i][0]);
		__m128 brod1 = _mm_set1_ps(m[i][1]);
		__m128 brod2 = _mm_set1_ps(m[i][2]);
		__m128 brod3 = _mm_set1_ps(m[i][3]);
		__m128 row = _mm_add_ps(
		    _mm_add_ps(_mm_mul_ps(brod0, row0), _mm_mul_ps(brod1, row1)),
		    _mm_add_ps(_mm_mul_ps(brod2, row2), _mm_mul_ps(brod3, row3)));
		_mm_store_ps(result.m[i], row);
	}
	return result;
}

Matrix4x4 Matrix4x4::operator*(const float& scalar) const {
	Matrix4x4 result;
	__m256 value = _mm256_set1_ps(scalar);
	*(__m256*)&result.m[0] = _mm256_mul_ps(_mm256_load_ps(m[0]), value);
	*(__m256*)&result.m[2] = _mm256_mul_ps(_mm256_load_ps(m[2]), value);
	return result;
}

Matrix4x4 Matrix4x4::operator/(const float& scalar) const {
	Matrix4x4 result;
	__m256 value = _mm256_set1_ps(scalar);
	*(__m256*)&result.m[0] = _mm256_div_ps(_mm256_load_ps(m[0]), value);
	*(__m256*)&result.m[2] = _mm256_div_ps(_mm256_load_ps(m[2]), value);
	return result;
}

Matrix4x4 Matrix4x4::operator+=(const Matrix4x4& Second) { return *this = *this + Second; }

Matrix4x4 Matrix4x4::operator-=(const Matrix4x4& Second) { return *this = *this - Second; }

Matrix4x4 Matrix4x4::operator*=(const Matrix4x4& Second) { return *this = *this * Second; }

Matrix4x4 Matrix4x4::operator*=(const float& Second) { return *this = *this * Second; }

Matrix4x4 Matrix4x4::operator/=(const float& Second) { return *this = *this / Second; }

Vector4 operator*(const Vector4& fir, const Matrix4x4& sec) {
	Vector4 result;
	for (unsigned char i = 0u; i < 4u; i++) {
		__m128 vecB = _mm_set_ps(sec.m[3][i], sec.m[2][i], sec.m[1][i], sec.m[0][i]);
		((float*)&result)[i] = _mm_cvtss_f32(_mm_dp_ps(*(__m128*)&fir, vecB, 0xFF));
	}
	return result;
}
