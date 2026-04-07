#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>
#include <Termina/Core/Logger.hpp>
#include "GameManager.hpp"
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

enum class ObstacleType{X, Y,};

class ObstacleComponent : public TerminaScript::ScriptableComponent
{
public:

	ObstacleComponent() = default;
	ObstacleComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void OnCollisionEnter(Termina::Actor* other)override;

	void Move();
	void DeleteObstacle(float dt);
	
	void SetType(ObstacleType type);

private:
	ObstacleType _type = ObstacleType::X;

	float lifeSpanY = 0.f;
};