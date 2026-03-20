#include "LaserComponent.h"

#include <ImGui/imgui.h>

void LaserComponent::Update(float dt)
{
	
}


void LaserComponent::Inspect()
{
	ImGui::DragFloat("Laser Size", &m_laserSize, 0.1f);
}

void LaserComponent::OnCollisionEnter(Termina::Actor* other)
{
	if (other) {
		TN_INFO("Collision detectee avec");
	}
}


