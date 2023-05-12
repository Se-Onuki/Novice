#include <Header/Math/Vector2.h>
#include <Header/Math/Vector3.h>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Matrix4x4.h"
#include "Header/Math/Math.hpp"
#include <cmath>
#include <numbers>

#include "Header/Render/Render.hpp"

const char kWindowTitle[] = "LE2A_03_オヌキ_セイヤ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 変数
	Matrix4x4 orthograficMatrix =
	    Render::MakeOrthographicMatrix({-160.f, 160.f}, {200.f, 300.f}, 0.f, 1000.f);
	Matrix4x4 perspectiveFovMatrix = Render::MakePerspectiveFovMatrix(0.63f, 1.33f, 0.1f, 1000.f);
	Matrix4x4 viewportMatrix = Render::MakeViewportMarix({100.f, 200.f}, 600.f, 300.f, 0.f, 1.f);

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
		
		orthograficMatrix.Printf(0, 0);
		perspectiveFovMatrix.Printf(0, 100);
		viewportMatrix.Printf(0, 200);

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
