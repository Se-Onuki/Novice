#include <Header/Math/Vector2.hpp>
#include <Header/Math/Vector3.hpp>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Matrix4x4.hpp"
#include "Header/Math/Math.hpp"

const char kWindowTitle[] = "LE2A_03_オヌキ_セイヤ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 変数
	Vector3 position = {2.3f, 3.8f, 1.4f};
	Vector3 translate= {4.1f, 2.6f, 0.8f};
	Vector3 scale = {1.5f, 5.2f, 7.3f};
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 transformMatrix = {
		1.f, 2.f, 3.f, 4.f,
		3.f, 1.f, 1.f, 2.f,
		1.f, 4.f, 2.f, 3.f,
		2.f, 2.f, 1.f, 3.f
	};
	Vector3 transformed = position * transformMatrix;


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

		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		
		transformed.Printf(0, 0);
		translateMatrix.Printf(0, 60);
		scaleMatrix.Printf(0, 160);

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
