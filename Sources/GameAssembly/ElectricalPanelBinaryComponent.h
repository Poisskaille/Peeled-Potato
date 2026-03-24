#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
// #include <Termina/Renderer/Components/MeshComponent.hpp>

using namespace TerminaScript;

class ElectricalPanelBinaryComponent : public TerminaScript::ScriptableComponent
{


public:

	enum class PanelState {
		Off,
		Blinking,
		On,
	};

	ElectricalPanelBinaryComponent() = default;
	ElectricalPanelBinaryComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float deltaTime) override;
	void Inspect() override;
	void OnCollisionEnter(Termina::Actor* other) override;

private:
	PanelState m_panelState = PanelState::Off;
	float m_timer = 0.f;
	float m_blinkTimer = 0.f;
	bool m_isBlinkingOn = false;

	float m_offDuration = 1.f;
	float m_blinkDuration = 0.5f;

	// Termina::Material* m_OffMaterial = nullptr;
	// Termina::Material* m_OnMaterial = nullptr;

};