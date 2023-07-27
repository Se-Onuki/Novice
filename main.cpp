#include <Header/Math/Vector2.h>
#include <Header/Math/Vector3.h>
#include <Novice.h>

#include <imgui.h>

#include "Header/Math/Lerp.h"
#include "Header/Math/Math.hpp"
#include "Header/Math/Matrix4x4.h"
#include "Header/Object/Transform.h"
#include <cmath>
#include <numbers>

#include "Header/Object/Object.h"
#include "Header/Render/Render.hpp"
#include <algorithm>

// #include <DirectXMath.h >

const char kWindowTitle[] = "LE2A_03_オヌキ_セイヤ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	IntVector2 mouse{};
	IntVector2 preMouse{};
	Vector2 mouseDiff{};

	// 変数

	Render render;
	render.SetViewportMatrix(Render::MakeViewportMatrix(0, 0, 1280.f, 720.f, 0.f, 1.f));

	Camera camera{Render::MakePerspectiveFovMatrix(0.45f, 1280.f / 720.f, 0.1f, 100.f)};

	Vector3 cameraOrigin{0, 0, 0};
	Vector3 cameraEuler{20.f * Angle::Dig2Rad, 0, 0};

	float cameraRadius = 15.f;

	Vector3 a{0.2f, 1.f, 0.f};
	Vector3 b{2.4f, 3.1f, 1.2f};

	Vector3 c = a + b;
	Vector3 d = a - b;
	Vector3 e = a * 2.4f;

	Vector3 rotate{.4f, 1.43f, -.8f};
	Matrix4x4 xRot = Matrix4x4::EulerRotate(Matrix4x4::Pitch, rotate.x);
	Matrix4x4 yRot = Matrix4x4::EulerRotate(Matrix4x4::Yaw, rotate.y);
	Matrix4x4 zRot = Matrix4x4::EulerRotate(Matrix4x4::Roll, rotate.z);

	Matrix4x4 matRot = xRot * yRot * zRot;

	// uint32_t sphereColor = WHITE;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		preMouse = mouse;
		Novice::GetMousePosition(&mouse.x, &mouse.y);
		mouseDiff.x = (float)mouse.x - preMouse.x;
		mouseDiff.y = (float)mouse.y - preMouse.y;

		///
		/// ↓更新処理ここから
		///

		Vector3 transform = Vector3::zero();

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

		cameraRadius -= Novice::GetWheel() / 120.f;
		if (cameraRadius <= 0.f) {
			cameraRadius = 0.1f;
		}

		Vector3 cameraPos = Vector3::back() * cameraRadius;
		if (Novice::IsPressMouse(2)) {
			cameraEuler.x += mouseDiff.y * Angle::Dig2Rad * 0.1f;
			cameraEuler.x =
			    std::clamp(cameraEuler.x, -(Angle::PI / 2 - 0.1f), (Angle::PI / 2 - 0.1f));
			cameraEuler.y += mouseDiff.x * Angle::Dig2Rad * 0.1f;
		}

		cameraPos *= Matrix4x4::EulerRotate(cameraEuler);

		cameraPos = cameraPos.Nomalize() * cameraRadius;
		Vector3 cameraRotate = (-cameraPos).Direction2Euler();
		transform *= Matrix4x4::EulerRotate(Matrix4x4::Yaw, cameraRotate.y);
		cameraOrigin += transform;
		camera.SetTransform({
		    {1.f, 1.f, 1.f},
            cameraRotate, cameraPos + cameraOrigin
        });

		// if (Collision::IsHit(line, triangle))
		//	sphereColor = RED;
		// else
		//	sphereColor = WHITE;

		camera.UpdateMatrix();
		render.UpdateSurface();

		ImGui::Begin("Math");
		ImGui::Text("c : %.4f %.4f %.4f", c.x, c.y, c.z);
		ImGui::Text("d : %.4f %.4f %.4f", d.x, d.y, d.z);
		ImGui::Text("e : %.4f %.4f %.4f", e.x, e.y, e.z);

		ImGui::Text("matRot :"
			"\n%.4f %.4f %.4f %.4f"
			"\n%.4f %.4f %.4f %.4f"
			"\n%.4f %.4f %.4f %.4f"
			"\n%.4f %.4f %.4f %.4f",
		    matRot.m[0][0],matRot.m[0][1],matRot.m[0][2],matRot.m[0][3],
		    matRot.m[1][0],matRot.m[1][1],matRot.m[1][2],matRot.m[1][3],
		    matRot.m[2][0],matRot.m[2][1],matRot.m[2][2],matRot.m[2][3],
		    matRot.m[3][0],matRot.m[3][1],matRot.m[3][2],matRot.m[3][3]
			);
		ImGui::End();

		ImGui::Begin("window");

		ImGui::Text("While Click	  : CameraRotate");
		ImGui::Text("While Rotate	  : ZoomIn / Out");
		ImGui::Text("WASD Space Shift : CameraMove");
		ImGui::DragFloat3("angle", &cameraEuler.x);
		ImGui::SameLine();
		if (ImGui::Button("reset")) {
			cameraEuler = Vector3{20 * Angle::Dig2Rad, 0, 0};
			cameraRadius = 15.f;
		}

		// catmull.ImGuiDebug("catmull");

		ImGui::End();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		render.Draw();
		render.DrawGrid(camera.GetViewProjection(), 5);
		// render.DrawCurve(camera.GetViewProjection(), catmull, WHITE, 20u);

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
