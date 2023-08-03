#pragma once

#include "Matrix2x2.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Polar.h"
#include "Vector2.h"
#include <numbers>

static const Vector2 TextSize{50, 20};

/// @brief 極座標系をベクトルに変換する。
/// @param Polar 極座標
/// @return ベクトル
Vector2 operator<<(Vector2& vec2, const Polar& Polar);

/// @brief ベクトルを極座標系に変換する。
/// @param Vector ベクトル
/// @return 極座標クラス
Polar operator<<(Polar& Polar, const Vector2& vec2);

/// @brief ベクトルの向きのみの回転
/// @param v 元ベクトル
/// @param m ワールド行列
/// @return 回転したベクトル
_NODISCARD Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

/// <summary>
/// 回転行列作成関数
/// </summary>
/// <param name="theta">radian角度</param>
/// <returns>2x2の回転行列</returns>
Matrix2x2 MakeRotateMatrix(const float& theta);

// <summary>
/// 拡縮行列作成関数
/// </summary>
/// <param name="scale">拡縮の倍率</param>
/// <returns>3x3の拡縮行列</returns>
Matrix3x3 MakeScaleMatrix(const Vector2& scale);

// <summary>
/// 拡縮行列作成関数
/// </summary>
/// <param name="scale">拡縮の倍率</param>
/// <returns>4x4の拡縮行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// <summary>
/// 回転行列作成関数
/// </summary>
/// <param name="theta">radian角度</param>
/// <returns>3x3の回転行列</returns>
Matrix3x3 MakeRotateMatrix3x3(const float& theta);

/// <summary>
/// 平行移動行列の作成関数
/// </summary>
/// <param name="translate">変換元のベクトル</param>
/// <returns>同次座標系</returns>
Matrix3x3 MakeTranslateMatrix(const Vector2& translate);

/// <summary>
/// 平行移動行列の作成関数
/// </summary>
/// <param name="translate">変換元のベクトル</param>
/// <returns>同次座標系</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// アフィン行列の作成関数
/// </summary>
/// <param name="scale">拡縮の倍率</param>
/// <param name="theta">radian角度</param>
/// <param name="translate">変換元のベクトル</param>
/// <returns>同次座標系</returns>
Matrix3x3 MakeAffineMatrix(const Vector2& scale, const float& theta, const Vector2& translate);

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="LeftTop">左上</param>
/// <param name="RightBottom">右下</param>
/// <returns>正射影行列</returns>
Matrix3x3 MakeOrthographicMatrix(const Vector2& LeftTop, const Vector2& RightBottom);

/// <summary>
/// ビューポート行列
/// </summary>
/// <param name="LeftTop">左上</param>
/// <param name="RightBottom">右下</param>
/// <returns>ビューポート行列</returns>
Matrix3x3 MakeViewportMatrix(const Vector2& LeftTop, const Vector2& RightBottom);

namespace Angle {

struct Digree {
	float digree;
	float ToRadian();
};

struct Radian {
	float radian;
	float ToDigree();
};
constexpr float PI = static_cast<float>(std::numbers::pi);
constexpr float PI_2 = static_cast<float>(std::numbers::pi) * 2;

constexpr float Dig2Rad = Angle::PI / 180.f;
constexpr float Rad2Dig = 180.f / Angle::PI;

} // namespace Angle

Vector3 ClosestPoint(const Vector3& v1, const Vector3& v2);