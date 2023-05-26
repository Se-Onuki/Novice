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

	Segment segment;
	segment.origin = {-2.f, -1.f, 0.f};
	segment.diff = {3.f, 2.f, 2.f};

	Sphere point{
	    {-1.5f, 0.6f, 0.6f},
        -0.01f
    };

	Render render;
	render.SetViewportMatrix(Render::MakeViewportMatrix(0, 0, 1280.f, 720.f, 0.f, 1.f));

	Camera camera{Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f)};
	camera.SetTransform({
	    {1.f,   1.f, 1.f  },
        {0.25f, 0.f, 0.f  },
        {0.f,   5.f, -15.f}
    });

	Vector3 project{0, 0, 0};
	Sphere closestPoint{
	    {0, 0, 0},
        0.01f
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

		Transform transform = camera.GetTransform();

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

		if (keys[DIK_SPACE]) {
			transform.translate.y += 0.01f;
		}
		if (keys[DIK_LSHIFT]) {
			transform.translate.y -= 0.01f;
		}

		camera.SetTransform(transform);

		camera.UpdateMatrix();

		render.UpdateSurface();

		project = segment.Project(point.center);
		closestPoint.center = segment.ClosestPoint(point.center);

		ImGui::Begin("window");
		ImGui::DragFloat3("point", &point.center.x, 0.1f);
		ImGui::DragFloat3("segmentOrigin", &segment.origin.x, 0.1f);
		ImGui::DragFloat3("segmentDiff", &segment.diff.x, 0.1f);
		ImGui::DragFloat3("Project", &segment.diff.x, 0.1f);
		ImGui::DragFloat3("ClosestPoint", &closestPoint.center.x, 0.1f);
		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		render.Draw();

		render.DrawLine(camera.GetViewProjection(), segment, GREEN);
		render.DrawSphere(camera.GetViewProjection(), point, RED, 4);
		render.DrawSphere(camera.GetViewProjection(), closestPoint, BLACK, 4);
		render.DrawGrid(camera.GetViewProjection());

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
