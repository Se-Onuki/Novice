#include <Header/Math/Vector2.hpp>
#include <Header/Math/Vector3.hpp>
#include <Novice.h>

#include <imgui.h>

const char kWindowTitle[] = "LE2A_03_オヌキ_セイヤ_MT3";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// 変数
	Vector3 v1{1.f, 3.f, -5.f};
	Vector3 v2{4.f, -1.f, 2.f};
	float k{4.f};

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

		ImGui::Begin("config");
		ImGui::Text("vector1");
		ImGui::InputFloat("v1.x", &v1.x);
		ImGui::InputFloat("v1.y", &v1.y);
		ImGui::InputFloat("v1.z", &v1.z);
		ImGui::Text("vector2");
		ImGui::InputFloat("v2.x", &v2.x);
		ImGui::InputFloat("v2.y", &v2.y);
		ImGui::InputFloat("v2.z", &v2.z);
		ImGui::Text("k");
		ImGui::InputFloat("k", &k);
		ImGui::End();

		Vector3 add = v1 + v2;
		Vector3 subtract = v1 - v2;

		Vector3 multiply = v1 * k;

		float dot = v1 * v2;
		float length = v1.Length();

		Vector3 nomalize = v2.Nomalize();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		add.Printf(0, 0);
		subtract.Printf(0, 70);
		multiply.Printf(0, 140);
		Novice::ScreenPrintf(0, 220, "%.3f", dot);
		Novice::ScreenPrintf(0, 240, "%.3f", length);
		nomalize.Printf(0, 260);

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
