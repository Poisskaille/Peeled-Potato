#include "LaserComponent.h"

#include <ImGui/imgui.h>


void LaserComponent::Inspect()
{
	if (ImGui::DragFloat("Laser Size", &m_laserSize, 0.1f)) {
		if (m_Owner && m_Owner->HasComponent<Termina::Transform>()){
			auto& transform = m_Owner->GetComponent<Termina::Transform>();

			glm::vec3 scale = transform.GetScale();
			scale.z = m_laserSize;
			transform.SetScale(scale);

			glm::vec3 localPos = transform.GetLocalPosition();
			localPos.z = m_laserSize / 2.0f;
			transform.SetLocalPosition(localPos);
			}
		
	}
}

void LaserComponent::OnCollisionEnter(Termina::Actor* other)
{

	if (other->HasComponent<Termina::Transform>()) {
		other->GetComponent <Termina::Transform>().SetPosition(glm::vec3(0, 0, 0));
		TN_DEBUG("Laser Toucher");
	}
}


