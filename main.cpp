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

	const float deltaTime = 1.f / 60.f;
	const Vector3 kGravity{0.f, -9.8f, 0.f};
	// const Vector3 defaultPos{0.8f, 0.2f, 0.f};

	Pendulum pendulum{};
	pendulum.anchor = {0.f, 1.f, 0.f};
	pendulum.length = 0.8f;
	pendulum.angle = 0.7f;
	pendulum.angularVelocity = 0.f;
	pendulum.angularAcceleration = 0.f;

	// bool startFlag = false;

	LineBase line{};
	Sphere sphere{.radius = 0.15f};

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

#pragma region 振り子

		pendulum.angularAcceleration = kGravity.y / pendulum.length * std::sin(pendulum.angle);
		pendulum.angularVelocity += pendulum.angularAcceleration * deltaTime;
		pendulum.angle += pendulum.angularVelocity * deltaTime;

		pendulum.angularAcceleration = 0.f;

#pragma region 見た目

		sphere.centor =
		    pendulum.anchor +
		    Vector3{std::sin(pendulum.angle), -std::cos(pendulum.angle), 0.f} * pendulum.length;

		line.origin = pendulum.anchor;
		line.diff = sphere.centor - line.origin;

#pragma endregion

#pragma endregion

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

		render.DrawSphere(camera.GetViewProjection(), sphere);
		render.DrawLine(camera.GetViewProjection(), line, RED);
		//  render.DrawCurve(camera.GetViewProjection(), catmull, WHITE, 20u);

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
