#pragma once
#include "../Math/Vector3.h"
#include <vector>
template<typename T> T Lerp(const T& Start, const T& End, float t) {
	return Start + (End - Start) * t;
}

Vector3 Slerp(const Vector3& Start, const Vector3& End, float t);

class Curve {
protected:
	std::vector<Vector3> pointList_;

public:
	// virtual T GetPosition(const float&) = 0;
	Curve() {}
	Curve(const Vector3& pos) { AddPoint(pos); }
	Curve(const std::vector<Vector3>& pos) { AddPoint(pos); }
	void AddPoint(const Vector3& pos) { pointList_.push_back(pos); }
	void AddPoint(const std::vector<Vector3>& pos) {
		pointList_.insert(pointList_.end(), pos.begin(), pos.end());
	}

	inline const std::vector<Vector3>& GetPointList() const { return pointList_; }
	void ImGuiDebug(const char* treeName);
};

class Bezier : public Curve {
public:
	using Curve::Curve;
	inline Vector3 GetPosition(const float& t) const {
		/*
		    // 仮に、4つの制御点を持つ、3次ベジエ曲線と仮定する。
		    // 0 -> 1 -> 2 -> 3と、各制御点の間で係数を使った線形補間を行う。
		    // 上記の操作により、3点の座標を取得する。
		    // 3点の座標を再度制御点として、2次ベジエ曲線とする。

		    // 3つの制御点を持つ 2次ベジエ曲線から、再起関数にて2つの制御点を取得する。

		    // 制御点が2つのベジエ曲線の場合、通常の線形補間と同じになる。
		    // 線形補間の返り値として、1点の座標を返す。

		    // 再起関数の引数が1つとなった場合、再帰の終端として、値をそのまま返す。
		*/

		// 引数が 1つの場合、再起関数の終端としてその座標を返す。
		if (this->pointList_.size() == 1)
			return this->pointList_[0];
		// - 引数から1つ制御点を減らしたベジエ曲線を作成。
		std::vector<Vector3> newPoints;
		for (int i = 0; i < this->pointList_.size() - 1; i++) {
			// 各制御点の間を線形補間して、新しい制御点を作成する。
			newPoints.push_back(Lerp(this->pointList_[i], this->pointList_[i + 1], t));
		}
		return Bezier(newPoints).GetPosition(t);
	}
};

class Catmull : public Curve {

public:
	using Curve::Curve;

	/// @brief Catmull-Rom スプライン曲線上の座標
	/// @param t 線形補間係数(整数は制御点リストの添え字と同義)
	/// @return 曲線上の座標
	inline Vector3 GetStructFacing(const float& t) const { return GetStructFacing(pointList_, t); }

	/// @brief Catmull-Rom スプライン曲線上の座標
	/// @param points 制御点リスト
	/// @param t 線形補間係数(整数は制御点リストの添え字と同義)
	/// @return 曲線上の座標
	static inline Vector3 GetStructFacing(const std::vector<Vector3>& points, const float& t) {
		if (points.size() < 4)
			return points[0];
		int index = static_cast<int>(std::floorf(t));
		if (index == 0) {
			return GetStructFacing(points[0], points[0], points[1], points[2], t);
		}
		if (index == points.size() - 2) {
			return GetStructFacing(
			    points[index - 1], points[index], points[index + 1], points[index + 1], t - index);
		}
		if (index == points.size() - 1) {
			return points[index];
		}
		return GetStructFacing(
		    points[index - 1], points[index], points[index + 1], points[index + 2], t - index);
	}

	/// @brief Catmull-Rom スプライン曲線上の座標
	/// @param t 線形補間係数(整数は制御点リストの添え字と同義)
	/// @return 曲線上の座標
	inline Vector3 GetStructPosition(const float& t) const {
		return GetStructPosition(pointList_, t);
	}

	/// @brief Catmull-Rom スプライン曲線上の座標
	/// @param points 制御点リスト
	/// @param t 線形補間係数(整数は制御点リストの添え字と同義)
	/// @return 曲線上の座標
	static inline Vector3 GetStructPosition(const std::vector<Vector3>& points, const float& t) {
		if (points.size() < 4)
			return points[0];
		int index = static_cast<int>(std::floorf(t));
		if (index == 0) {
			return GetStructPosition(points[0], points[0], points[1], points[2], t);
		} else if (index == points.size() - 2) {
			return GetStructPosition(
			    points[index - 1], points[index], points[index + 1], points[index + 1], t - index);
		} else if (index == points.size() - 1) {
			return points[index];
		} else if (index == points.size()) {
			return points.back();
		}
		return GetStructPosition(
		    points[index - 1], points[index], points[index + 1], points[index + 2], t - index);
	}

	/// @brief Catmull-Rom スプライン曲線上の座標
	/// @param p0 制御点0
	/// @param p1 制御点1(t = 0.0f)
	/// @param p2 制御点2(t = 1.0f)
	/// @param p3 制御点3
	/// @param t 線形補間係数
	/// @return p1 ~ p2の間の曲線上の座標
	static inline Vector3 GetStructPosition(
	    const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3,
	    const float& t) {
		return ((-p0 + 3 * p1 - 3 * p2 + p3) * powf(t, 3) +
		        (2 * p0 - 5 * p1 + 4 * p2 - p3) * powf(t, 2) + (-p0 + p2) * t + 2 * p1) /
		       2;
	}
	// Catmull-Romスプライン曲線の微分
	static inline Vector3 GetStructFacing(
	    const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
		return ((-p0 + 3 * p1 - 3 * p2 + p3) * 3.0f * powf(t, 2) +
		        (2 * p0 - 5 * p1 + 4 * p2 - p3) * 2.0f * t + (-p0 + p2)) /
		       2;
	}

	/*inline Vector3 GetPosition(const float& t) const {

	};*/
};