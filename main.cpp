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

	Transform shoulder{
	    Vector3::one(), Vector3{0.f,  0.f,  -6.8f},
         Vector3{0.2f, 0.1f, 0.f  }
    };

	Transform elbow{
	    Vector3{3.f,  1.f, 1.f  },
        Vector3{0.f,  0.f, -1.4f},
        Vector3{0.4f, 0.f, 0.f  }
    };
	Transform hand{
	    Vector3::one(), Vector3::zero(), Vector3{0.3f, 0.f, 0.f}
    };

	std::array<Matrix4x4, 3u> worldMatrix = {};
	std::array<Sphere, 3u> sphere = {};
	std::array<LineBase, 2u> line = {};

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

		ImGui::Begin("Tree");
		;
		if (ImGui::TreeNode("Shoulder")) {
			shoulder.ImGuiWidget();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Elbow")) {
			elbow.ImGuiWidget();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Hand")) {
			hand.ImGuiWidget();
			ImGui::TreePop();
		}

		ImGui::End();

		worldMatrix[0] = shoulder.Affine();
		worldMatrix[1] = elbow.Affine() * worldMatrix[0];
		worldMatrix[2] = hand.Affine() * worldMatrix[1];

		for (uint32_t i = 0u; i < 3u; i++) {
			sphere[i].centor = *(Vector3*)worldMatrix[i].m[3];
			sphere[i].radius = 0.2f;
		}
		for (uint32_t i = 0; i < 2u; i++) {
			line[i].origin = sphere[i].centor;
			line[i].diff = sphere[i + 1u].centor - sphere[i].centor;
		}

		// if (Collision::IsHit(line, triangle))
		//	sphereColor = RED;
		// else
		//	sphereColor = WHITE;

		camera.UpdateMatrix();
		render.UpdateSurface();

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

		for (uint32_t i = 0; i < 3u; i++) {
			render.DrawSphere(camera.GetViewProjection(), sphere[i]);
		}
		render.DrawLine(camera.GetViewProjection(), line[0]);
		render.DrawLine(camera.GetViewProjection(), line[1]);

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
