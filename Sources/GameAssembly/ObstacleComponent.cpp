#include "RotateComponent.hpp"

#include <ImGui/imgui.h>

void ObstacleComponent::Update(float dt)
{
	glm::vec3 pos = m_Transform->GetRotation();
	pos.x += _speed;
	m_Transform->SetRotation(pos);
}

void ObstacleComponent::Serialize(nlohmann::json& out) const
{
	out["Rotate Speed"] = m_rotateSpeed;
}

void ObstacleComponent::Deserialize(const nlohmann::json& in)
{
	if (in.contains("Rotate Speed")) m_rotateSpeed = in["Rotate Speed"];
}