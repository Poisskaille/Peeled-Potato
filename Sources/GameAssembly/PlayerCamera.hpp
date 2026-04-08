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
	//Termina::CameraComponent cam;

	//float Sensitivity = 0.1f;
	//float Pitch = 0.0f;
	//float Yaw = 0.0f;
    // Smooth side-look
	float m_CurrentYawOffset = 0.0f;
	float m_TargetYawOffset = 0.0f;
	float m_SideLookAngle = 20.0f; // degrees
	float m_LerpSpeed = 6.0f; // higher is faster
	glm::quat m_BaseRotation = glm::quat(1,0,0,0);
};