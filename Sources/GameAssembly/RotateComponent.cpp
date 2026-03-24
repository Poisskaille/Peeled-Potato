#include "RotateComponent.hpp"

#include <ImGui/imgui.h>

void RotateComponent::Update(float dt)
{
	glm::quat rot = m_Transform->GetRotation();
	rot = glm::rotate(rot, glm::radians(m_rotateSpeed), glm::vec3(1.0f, 0.0f, 0.0f));
	m_Transform->SetRotation(rot);
}

void RotateComponent::Serialize(nlohmann::json& out) const
{
	out["Rotate Speed"] = m_rotateSpeed;
}

void RotateComponent::Deserialize(const nlohmann::json& in)
{
	if (in.contains("Rotate Speed")) m_rotateSpeed = in["Rotate Speed"];
}

void RotateComponent::Inspect()
{
	ImGui::DragFloat("Rotate Caca", &m_rotateSpeed, 0.1f);
}
