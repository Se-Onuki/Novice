#include <Header/Math/Vector2.hpp>
#include <Header/Math/Vector3.hpp>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Matrix4x4.hpp"

const char kWindowTitle[] = "LE2A_03_オヌキ_セイヤ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Matrix4x4 m1 = {3.2f, 0.7f, 9.6f, 4.4f, 5.5f, 1.3f, 7.8f, 2.1f,
	                6.9f, 8.f,  2.6f, 1.f,  0.5f, 7.2f, 5.1f, 3.3f};
	Matrix4x4 m2 = {4.1f, 6.5f, 3.3f, 2.2f, 8.8f, 0.6f, 9.9f, 7.7f,
	                1.1f, 5.5f, 6.6f, 0.0f, 3.3f, 9.9f, 8.8f, 2.2f};

	Matrix4x4 add = m1 + m2;
	Matrix4x4 subtract = m1 - m2;
	Matrix4x4 multiply = m1 * m2;

	Matrix4x4 inverseM1 = m1.Inverse();
	Matrix4x4 inverseM2 = m2.Inverse();

	Matrix4x4 transposeM1 = m1.Transpose();
	Matrix4x4 transposeM2 = m2.Transpose();

	Matrix4x4 identity = Matrix4x4::Identity();

	bool toggle = false;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			if (toggle) {
				toggle = false;
			} else {
				toggle = true;
			}
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		if (toggle) {
			add.Printf(0, 0);
			subtract.Printf(0, 100);
			multiply.Printf(0, 200);
			inverseM1.Printf(0, 300);
			inverseM2.Printf(0, 400);
		} else {
			transposeM1.Printf(600, 0);
			transposeM2.Printf(600, 100);
			identity.Printf(600, 200);
		}
		Novice::ScreenPrintf(600, 400, "Toggle Space");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
