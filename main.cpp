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

	Render render;
	render.SetViewportMatrix(Render::MakeViewportMatrix(0, 0, 1280.f, 720.f, 0.f, 1.f));

	Camera camera{Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f)};
	camera.SetTransform({
	    {1.f,   1.f, 1.f  },
        {0.25f, 0.f, 0.f  },
        {0.f,   5.f, -15.f}
    });

	Segment segment{
	    {0, 1, 0},
        {0, 1, 1}
    };

	// Plane plane = Plane::Create(Vector3{1, 0, 0}, Vector3{2.1f, 0, 0});
	Triangle triangle{
	    Vector3{-1, 1, 0},
        Vector3{0,  2, 0},
        Vector3{1,  1, 0}
    };

	uint32_t sphereColor = WHITE;

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

		Vector3& transform = segment.origin;

		if (keys[DIK_A]) {
			transform.x -= 0.01f;
		}
		if (keys[DIK_D]) {
			transform.x += 0.01f;
		}
		if (keys[DIK_W]) {
			transform.z += 0.01f;
		}
		if (keys[DIK_S]) {
			transform.z -= 0.01f;
		}

		if (keys[DIK_SPACE]) {
			transform.y += 0.01f;
		}
		if (keys[DIK_LSHIFT]) {
			transform.y -= 0.01f;
		}

		if (Collision::IsHit(segment, triangle))
			sphereColor = RED;
		else
			sphereColor = WHITE;

		camera.UpdateMatrix();
		render.UpdateSurface();

		ImGui::Begin("window");
		segment.ImGuiDebug();
		triangle.ImGuiDebug();
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		render.Draw();
		render.DrawGrid(camera.GetViewProjection());
		render.DrawLine(camera.GetViewProjection(), segment, sphereColor);
		render.DrawTriangle(camera.GetViewProjection(), triangle, WHITE, kFillModeWireFrame);

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
