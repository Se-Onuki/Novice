#include "Transform.h"
#include "Header/Math/Matrix4x4.h"
#include <Header/Math/Math.hpp>
#include <imgui.h>

Transform::Transform(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
    : scale(scale), rotate(rotate), translate(translate) {}

Matrix4x4 Transform::Affine() const { return Matrix4x4::Affine(scale, rotate, translate); }

Matrix4x4 Transform::AffineRT() const {
	return Matrix4x4::Affine(Vector3::one(), rotate, translate);
}

Matrix4x4 Transform::AffineInv() const { return Matrix4x4(); }

bool Transform::ImGuiWidget() {
	if (ImGui::TreeNode("Transform")) {
		bool isUsing = false;

		isUsing |= ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.001f, 100.f);

		isUsing |= ImGui::DragFloat3("Rotate", &rotate.x, Angle::Dig2Rad);

		isUsing |= ImGui::DragFloat3("Transform", &translate.x, 0.01f);

		ImGui::TreePop();
		return isUsing;
	}
	return false;
}

bool Transform::ImGuiWidget2D() {
	if (ImGui::TreeNode("Transform2D")) {
		bool isUsing = false;

		isUsing |= ImGui::DragFloat2("Scale", &scale.x, 0.01f, 0.001f, 100.f);

		isUsing |= ImGui::DragFloat("Rotate", &rotate.z, Angle::Dig2Rad);

		isUsing |= ImGui::DragFloat2("Transform", &translate.x, 0.01f, -100.f, 100.f);

		ImGui::TreePop();
		return isUsing;
	}
	return false;
}