#include "ElectricalPanelStaticComponent.h"

#include <ImGui/imgui.h>




void ElectricalPanelStaticComponent::Update(float deltaTime)
{
	if (m_isRevealed) {
		m_timer += deltaTime;

		if (m_timer >= m_onDuration) {
		
			m_isRevealed = false;
			m_timer = 0.f;
			TN_DEBUG("Panneau electrique statique : redevient cache");

			// Remettre le matérial de base (Off) ici
			// if (GetOwner()->HasComponent<Termina::MeshComponent>()) {
			//     auto& mesh = GetOwner()->GetComponent<Termina::MeshComponent>();
			//     mesh.SetMaterial(m_OffMaterial);
			// }
		}
	}
}

void ElectricalPanelStaticComponent::Inspect()
{
	ImGui::Text("État actuel : %d", (int)m_panelState);
	ImGui::DragFloat("Duree d'illumination", &m_onDuration, 0.1f, 0.1f, 5.0f);

}

void ElectricalPanelStaticComponent::OnCollisionEnter(Termina::Actor* other)
{
	if (m_panelState == PanelState::On) {
		if (other->HasComponent<Termina::Transform>()) {
			other->GetComponent <Termina::Transform>().SetPosition(glm::vec3(0, 0, 0));
			TN_DEBUG("Panneau electrique static Toucher et actif");

			if (!m_isRevealed) {
				m_isRevealed = true;
				m_timer = 0.f;

				//metre le swicth de material ici
				//if (GetOwner()->HasComponent<Termina::MeshComponent>()) {
				//     auto& mesh = GetOwner()->GetComponent<Termina::MeshComponent>();
				//     mesh.SetMaterial(m_OnMaterial);
				//}
			}
		}
	}
	else {
		TN_DEBUG("Panneau electrique static Toucher mais eteint");
	}
}


