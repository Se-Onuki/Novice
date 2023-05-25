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

	Transform cameraTrans{
	    {1.f,   1.f, 1.f  },
        {0.25f, 0.f, 0.f  },
        {0.f,   5.f, -15.f}
    };

	Sphere sphere{
	    {0.f, 2.f, 0.f},
        2
    };

	float sphereSubdivision = 8;
	float gridSubdivision = 10;

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
			cameraTrans.translate.x -= 0.01f;
		}
		if (keys[DIK_D]) {
			cameraTrans.translate.x += 0.01f;
		}
		if (keys[DIK_W]) {
			cameraTrans.translate.z += 0.01f;
		}
		if (keys[DIK_S]) {
			cameraTrans.translate.z -= 0.01f;
		}

		if (keys[DIK_SPACE]) {
			cameraTrans.translate.y += 0.01f;
		}
		if (keys[DIK_LSHIFT]) {
			cameraTrans.translate.y -= 0.01f;
		}

		Matrix4x4 cameraMatrix = cameraTrans.Affine();

		Matrix4x4 viewMatrix = cameraMatrix.Inverse();

		Matrix4x4 projectonMatrix =
		    Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f);
		Matrix4x4 viewProjectonMatrix = viewMatrix * projectonMatrix;

		Matrix4x4 viewportMatrix = Render::MakeViewportMatrix(0, 0, 1280.f, 720.f, 0.f, 1.f);

		ImGui::Begin("window");
		ImGui::DragFloat3("CameraPosition", &cameraTrans.translate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraTrans.rotate.x, 0.01f);
		ImGui::DragFloat3("SpherePosition", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);
		ImGui::InputFloat("SphereSubdivision", &sphereSubdivision, 1.f);
		ImGui::InputFloat("GridSubdivision", &gridSubdivision, 1.f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		/// worldViewProjectionMatrix.Printf(0, 100);

		Render::DrawGrid(
		    viewProjectonMatrix, viewportMatrix, 2.f, static_cast<int>(gridSubdivision));
		Render::DrawSphere(
		    sphere, viewProjectonMatrix, viewportMatrix, static_cast<int>(sphereSubdivision), RED);

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
