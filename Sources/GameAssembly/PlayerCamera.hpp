#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp>
#include <GLM/gtc/quaternion.hpp>

using namespace TerminaScript;

class PlayerCamera : public TerminaScript::ScriptableComponent
{
public:

	PlayerCamera() = default;
	PlayerCamera(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Serialize(nlohmann::json& out) const override;
	void Deserialize(const nlohmann::json& in) override;

	void Update(float dt)override;
	void Start()override;

private:
	float m_CurrentYawOffset = 0.0f;
	float m_TargetYawOffset = 0.0f;
	float m_SideLookAngle = 90.0f;
	float m_LerpSpeed = 6.0f;
	glm::quat m_BaseRotation = glm::quat(1,0,0,0);
};