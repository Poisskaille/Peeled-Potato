#include "ForwardComponent.hpp"

#include <ImGui/imgui.h>

void ForwardComponent::Update(float dt)
{
	glm::vec3 pos = m_Transform->GetPosition();
	pos.x += 1.0f * dt;
	m_Transform->SetPosition({ pos.x , pos.y, pos.z });
}


