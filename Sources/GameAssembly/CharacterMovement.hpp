#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

enum class PlayerShieldState { None, Left, Right };

class CharacterMovement : public TerminaScript::ScriptableComponent
{
public:

	CharacterMovement() = default;
	CharacterMovement(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Start()override;

	void Jump(float dt);
    void Shield(float dt);

	PlayerShieldState GetShieldState();

private:
	Termina::Rigidbody* rb;
	PlayerShieldState shieldState = PlayerShieldState::None;

	float delay = 0.f;
};