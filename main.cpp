#include <Header/Math/Vector2.h>
#include <Header/Math/Vector3.h>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Object/Transform.h"
#include <cmath>
#include <numbers>

#include "Header/Object/Object.h"
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

	Transform triangleTrans{
	    {1.f, 1.f, 1.f},
        {0.f, 0.f, 0.f},
        {0,   0,   5.f}
    };
	Transform cameraTrans{
	    {1.f, 1.f, 1.f},
        {0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f}
    };

	Vector3 kLocalVertices[3]{
	    {0,     0.75f, 0},
        {0.5f,  -0.5f, 0},
        {-0.5f, -0.5f, 0}
    };

	Triangle surface{kLocalVertices};
	ModelClass model;
	model.AddSurface(surface);
	Object3d object{triangleTrans, &model};
	Render render;
	render.SetViewportMatrix(Render::MakeViewportMatrix(0, 0, 1280.f, 720.f, 0.f, 1.f));
	Camera camera{Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f)};

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

		Transform& transform = object.transform_;

		if (keys[DIK_A]) {
			transform.translate.x -= 0.01f;
		}
		if (keys[DIK_D]) {
			transform.translate.x += 0.01f;
		}
		if (keys[DIK_W]) {
			transform.translate.z += 0.01f;
		}
		if (keys[DIK_S]) {
			transform.translate.z -= 0.01f;
		}

		transform.rotate.y += 0.05f;

		if (keys[DIK_SPACE]) {
			transform.rotate.y = 0.f;
		}

		object.UpdateMatrix();
		camera.UpdateMatrix();

		camera.CreateNDC(object,&render);
		render.UpdateSurface();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		render.Draw();

		cross.Printf(0, 0);
		transform.rotate.Printf(0, 70);

		Novice::ScreenPrintf(
		    0, 140, "Push Space || object.rotate.y == digree( %.2f )",
		    (transform.rotate.y) / std::numbers::pi * 180.f);

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
