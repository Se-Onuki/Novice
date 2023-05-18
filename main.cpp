#include <Header/Math/Vector2.h>
#include <Header/Math/Vector3.h>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
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
	Vector3 v1{1.2f, -3.9f, 2.5f};
	Vector3 v2{2.8f, 0.4f, -1.3f};
	Vector3 cross = v1 ^ v2;

	Vector3 rotate{0.f, 0.f, 0.f};
	Vector3 translate{0, 0, 5.f};

	Vector3 cameraPosition{0.f, 0.f, 0.f};

	Vector3 kLocalVertices[3]{
	    {0,     0.75f, 0},
        {0.5f,  -0.5f, 0},
        {-0.5f, -0.5f, 0}
    };

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

		if (keys[DIK_A]) {
			translate.x -= 0.01f;
		}
		if (keys[DIK_D]) {
			translate.x += 0.01f;
		}
		if (keys[DIK_W]) {
			translate.z += 0.01f;
		}
		if (keys[DIK_S]) {
			translate.z -= 0.01f;
		}

		rotate.y += 0.05f;

		Matrix4x4 worldMatrix = Matrix4x4::Affine({1.f, 1.f, 1.f}, rotate, translate);
		Matrix4x4 cameraMatrix =
		    Matrix4x4::Affine({1.f, 1.f, 1.f}, {0.f, 0.f, 0.f}, cameraPosition);

		Matrix4x4 viewMatrix = cameraMatrix.Inverse();

		Matrix4x4 projectonMatrix =
		    Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f);

		Matrix4x4 worldViewProjectionMatrix = worldMatrix * (viewMatrix * projectonMatrix);

		Matrix4x4 viewportMatrix = Render::MakeViewportMatrix({0, 0}, 1280.f, 720.f, 0.f, 1.f);

		Vector3 screenVertices[3];
		for (uint32_t i = 0; i < 3; i++) {
			Vector3 ndcVertex = Transform(kLocalVertices[i], worldViewProjectionMatrix);
			screenVertices[i] = Transform(ndcVertex, viewportMatrix);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		screenVertices[0].Printf(0, 0);

		worldViewProjectionMatrix.Printf(0, 100);

		cross.Printf(0, 200);

		Novice::DrawTriangle(
		    static_cast<int>(screenVertices[0].x), static_cast<int>(screenVertices[0].y),
		    static_cast<int>(screenVertices[1].x), static_cast<int>(screenVertices[1].y),
		    static_cast<int>(screenVertices[2].x), static_cast<int>(screenVertices[2].y), RED,
		    kFillModeSolid);

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
