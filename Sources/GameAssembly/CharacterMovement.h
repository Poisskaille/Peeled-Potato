#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

class CharacterMovement : public TerminaScript::ScriptableComponent
{
public:

	CharacterMovement() = default;
	CharacterMovement(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Start()override;

	void Move(float dt);

private:
	Termina::Rigidbody rb;
	Termina::CameraComponent cam;
};