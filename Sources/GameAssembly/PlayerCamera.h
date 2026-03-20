#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

class PlayerCamera : public TerminaScript::ScriptableComponent
{
public:

	PlayerCamera() = default;
	PlayerCamera(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Start()override;

private:
	Termina::CameraComponent cam;

	float Sensitivity = 0.1f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
};