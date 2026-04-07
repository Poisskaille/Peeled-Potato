#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>
#include <Termina/Core/Logger.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

class ObstacleComponent : public TerminaScript::ScriptableComponent
{
public:

	ObstacleComponent() = default;
	ObstacleComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void OnCollisionEnter(Termina::Actor* other)override;


private:
	float _speed = 5.f;
};