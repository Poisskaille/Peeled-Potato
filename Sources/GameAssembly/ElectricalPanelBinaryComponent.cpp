#include "ElectricalPanelBinaryComponent.h"

#include <ImGui/imgui.h>


void ElectricalPanelBinaryComponent::Update(float deltaTime)
{
	m_timer += deltaTime;
	switch (m_panelState) {
	case PanelState::Off:
		if (m_timer >= m_offDuration) {
			m_panelState = PanelState::Blinking;
			m_timer = 0.f;
			TN_DEBUG("Panneau electrique : Début clignotement");
		}
		break;
	case PanelState::Blinking:
		m_blinkTimer += deltaTime;
		if (m_blinkTimer >= m_blinkDuration) {
			m_isBlinkingOn = !m_isBlinkingOn;
			m_blinkTimer = 0.f;
			TN_DEBUG("Panneau electrique : Clignotement ");

			//metre le swicth de material ici
			//if (GetOwner()->HasComponent<Termina::MeshComponent>()) {
			//     auto& mesh = GetOwner()->GetComponent<Termina::MeshComponent>();
			//     mesh.SetMaterial(m_IsBlinkMatActive ? m_OnMaterial : m_OffMaterial);
			// }
		}
		if (m_timer >= 5 * m_blinkDuration) { 
			m_panelState = PanelState::On;
			m_timer = 0.f;
			TN_DEBUG("Panneau electrique : Allumé");
		}
		break;
	case PanelState::On:
		if (m_timer >= m_offDuration) {
			m_panelState = PanelState::Off;
			m_timer = 0.f;
			TN_DEBUG("Panneau electrique : viens de s'eteindre");
		}
		break;
	}
}

	void ElectricalPanelBinaryComponent::Inspect()
{
	ImGui::Text("État actuel : %d", (int)m_panelState);
	ImGui::Text("Timer : %.2f s", m_timer);

	ImGui::Separator(); 

	ImGui::DragFloat("Durée état Off", &m_offDuration, 0.1f, 0.0f, 10.0f);
	ImGui::DragFloat("Durée Clignotement", &m_blinkDuration, 0.1f, 0.0f, 10.0f);

}

void ElectricalPanelBinaryComponent::OnCollisionEnter(Termina::Actor* other)
{
	if (m_panelState == PanelState::On) {
		if (other->HasComponent<Termina::Transform>()) {
			other->GetComponent <Termina::Transform>().SetPosition(glm::vec3(0, 0, 0));
			TN_DEBUG("Panneau electrique binaire Toucher et actif");
		}
	}
	else {
		TN_DEBUG("Panneau electrique binaire Toucher mais eteint");
	}
}


