#pragma once

#include"Vector2.hpp"
#include "PolarCoordinates.hpp"
#include "Matrix2x2.hpp"
#include "Matrix3x3.hpp"

/// <summary>
/// 二次元内積関数
/// </summary>
/// <param name="startA">線分Aの始点</param>
/// <param name="endA">線分Aの終点</param>
/// <param name="startB">線分Bの始点</param>
/// <param name="endB">線分Bの終点</param>
/// <param name="normalizeFlagA"></param>
/// <param name="normalizeFlagB"></param>
/// <returns>内積計算の結果</returns>
float DotProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB);

/// <summary>
/// 二次元内積関数
/// </summary>
/// <param name="VectorA">ベクトルA</param>
/// <param name="VectorB">ベクトルB</param>
/// <returns>内積計算の結果</returns>
float DotProduct(Vector2 VectorA, Vector2 VectorB);


/// <summary>
/// 二次元外積関数
/// </summary>
/// <param name="startA">線分Aの始点</param>
/// <param name="endA">線分Aの終点</param>
/// <param name="startB">線分Bの始点</param>
/// <param name="endB">線分Bの終点</param>
/// <param name="normalizeFlagA"></param>
/// <param name="normalizeFlagB"></param>
/// <returns>内積計算の結果</returns>
float CrossProduct(Vector2 startA, Vector2 endA, Vector2 startB, Vector2 endB);


/// <summary>
/// 二次元外積関数
/// </summary>
/// <param name="VectorA">ベクトルA</param>
/// <param name="VectorB">ベクトルB</param>
/// <returns>内積計算の結果</returns>
float CrossProduct(Vector2 VectorA, Vector2 VectorB);



/// <summary>
/// 極座標系をベクトルに変換する。
/// </summary>
/// <param name="Polar">極座標</param>
/// <returns>ベクトル</returns>
Vector2 Polar2Rectangular(Polar Polar);

/// <summary>
/// ベクトルを極座標系に変換する。
/// </summary>
/// <param name="Vector">ベクトル</param>
/// <returns>極座標クラス</returns>
Polar Rectangular2Polar(Vector2 Vector);


/// <summary>
/// 回転行列作成関数
/// </summary>
/// <param name="theta">radian角度</param>
/// <returns>2x2の回転行列</returns>
Matrix2x2 MakeRotateMatrix(const float &theta);


// <summary>
/// 拡縮行列作成関数
/// </summary>
/// <param name="scale">拡縮の倍率</param>
/// <returns>3x3の拡縮行列</returns>
Matrix3x3 MakeScaleMatrix3x3(const Vector2 &scale);

// <summary>
/// 回転行列作成関数
/// </summary>
/// <param name="theta">radian角度</param>
/// <returns>3x3の回転行列</returns>
Matrix3x3 MakeRotateMatrix3x3(const float &theta);


/// <summary>
/// 平行移動行列の作成関数
/// </summary>
/// <param name="translate">変換元のベクトル</param>
/// <returns>同次座標系</returns>
Matrix3x3 MakeTranslateMatrix(const Vector2 &translate);


/// <summary>
/// アフィン行列の作成関数
/// </summary>
/// <param name="scale">拡縮の倍率</param>
/// <param name="theta">radian角度</param>
/// <param name="translate">変換元のベクトル</param>
/// <returns>同次座標系</returns>
Matrix3x3 MakeAffineMatrix(const Vector2 &scale, const float &theta, const Vector2 &translate);

/// <summary>
/// ベクトルと同次座標系の合成
/// </summary>
/// <param name="vector">合成前のベクトル</param>
/// <param name="matrix">同次座標系の平行移動行列</param>
/// <returns>合成したベクトル</returns>
Vector2 Transform(const Vector2 &vector, const Matrix3x3 &matrix);

/// <summary>
/// 正射影行列
/// </summary>
/// <param name="LeftTop">左上</param>
/// <param name="RightBottom">右下</param>
/// <returns>正射影行列</returns>
Matrix3x3 MakeOrthographicMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom);

/// <summary>
/// ビューポート行列
/// </summary>
/// <param name="LeftTop">左上</param>
/// <param name="RightBottom">右下</param>
/// <returns>ビューポート行列</returns>
Matrix3x3 MakeViewportMatrix(const Vector2 &LeftTop, const Vector2 &RightBottom);
