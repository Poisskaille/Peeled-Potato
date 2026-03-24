#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
// #include <Termina/Renderer/Components/MeshComponent.hpp>

using namespace TerminaScript;

class ElectricalPanelStaticComponent : public TerminaScript::ScriptableComponent
{


public:

	enum class PanelState {
		Off,
		On,
	};

	ElectricalPanelStaticComponent() = default;
	ElectricalPanelStaticComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float deltaTime) override;
	void Inspect() override;
	void OnCollisionEnter(Termina::Actor* other) override;

private:
	PanelState m_panelState = PanelState::Off;
	float m_timer = 0.f;
	float m_onDuration = 1.f;
	bool m_isRevealed = false;
	
	// Termina::Material* m_OffMaterial = nullptr;
	// Termina::Material* m_OnMaterial = nullptr;

};