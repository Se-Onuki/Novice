#pragma once

#include"Vector2.hpp"
#include "PolarCoordinates.hpp"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

/// <summary>
/// �񎟌����ϊ֐�
/// </summary>
/// <param name="startA">����A�̎n�_</param>
/// <param name="endA">����A�̏I�_</param>
/// <param name="startB">����B�̎n�_</param>
/// <param name="endB">����B�̏I�_</param>
/// <param name="normalizeFlagA"></param>
/// <param name="normalizeFlagB"></param>
/// <returns>���όv�Z�̌���</returns>
float DotProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB);

/// <summary>
/// �񎟌����ϊ֐�
/// </summary>
/// <param name="VectorA">�x�N�g��A</param>
/// <param name="VectorB">�x�N�g��B</param>
/// <returns>���όv�Z�̌���</returns>
float DotProduct(Vector2 VectorA, Vector2 VectorB);


/// <summary>
/// �񎟌��O�ϊ֐�
/// </summary>
/// <param name="startA">����A�̎n�_</param>
/// <param name="endA">����A�̏I�_</param>
/// <param name="startB">����B�̎n�_</param>
/// <param name="endB">����B�̏I�_</param>
/// <param name="normalizeFlagA"></param>
/// <param name="normalizeFlagB"></param>
/// <returns>���όv�Z�̌���</returns>
float CrossProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB);


/// <summary>
/// �񎟌��O�ϊ֐�
/// </summary>
/// <param name="VectorA">�x�N�g��A</param>
/// <param name="VectorB">�x�N�g��B</param>
/// <returns>���όv�Z�̌���</returns>
float CrossProduct(Vector2 VectorA, Vector2 VectorB);



/// <summary>
/// �ɍ��W�n���x�N�g���ɕϊ�����B
/// </summary>
/// <param name="Polar">�ɍ��W</param>
/// <returns>�x�N�g��</returns>
Vector2 Polar2Rectangular(Polar Polar);

/// <summary>
/// �x�N�g�����ɍ��W�n�ɕϊ�����B
/// </summary>
/// <param name="Vector">�x�N�g��</param>
/// <returns>�ɍ��W�N���X</returns>
Polar Rectangular2Polar(Vector2 Vector);


/// <summary>
/// ��]�s��쐬�֐�
/// </summary>
/// <param name="theta">radian�p�x</param>
/// <returns>2x2�̉�]�s��</returns>
Matrix2x2 MakeRotateMatrix(const float &theta);


// <summary>
/// �g�k�s��쐬�֐�
/// </summary>
/// <param name="scale">�g�k�̔{��</param>
/// <returns>3x3�̊g�k�s��</returns>
Matrix3x3 MakeScaleMatrix3x3(const Vector2 &scale);

// <summary>
/// ��]�s��쐬�֐�
/// </summary>
/// <param name="theta">radian�p�x</param>
/// <returns>3x3�̉�]�s��</returns>
Matrix3x3 MakeRotateMatrix3x3(const float &theta);


/// <summary>
/// ���s�ړ��s��̍쐬�֐�
/// </summary>
/// <param name="translate">�ϊ����̃x�N�g��</param>
/// <returns>�������W�n</returns>
Matrix3x3 MakeTranslateMatrix(const Vector2 &translate);


/// <summary>
/// �A�t�B���s��̍쐬�֐�
/// </summary>
/// <param name="scale">�g�k�̔{��</param>
/// <param name="theta">radian�p�x</param>
/// <param name="translate">�ϊ����̃x�N�g��</param>
/// <returns>�������W�n</returns>
Matrix3x3 MakeAffineMatrix(const Vector2 &scale, const float &theta, const Vector2 &translate);

/// <summary>
/// �x�N�g���Ɠ������W�n�̍���
/// </summary>
/// <param name="vector">�����O�̃x�N�g��</param>
/// <param name="matrix">�������W�n�̕��s�ړ��s��</param>
/// <returns>���������x�N�g��</returns>
Vector2 Transform(const Vector2 &vector, const Matrix3x3 &matrix);

/// <summary>
/// ���ˉe�s��
/// </summary>
/// <param name="LeftTop">����</param>
/// <param name="RightBottom">�E��</param>
/// <returns>���ˉe�s��</returns>
Matrix3x3 MakeOrthographicMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom);

/// <summary>
/// �r���[�|�[�g�s��
/// </summary>
/// <param name="LeftTop">����</param>
/// <param name="RightBottom">�E��</param>
/// <returns>�r���[�|�[�g�s��</returns>
Matrix3x3 MakeViewportMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom);
