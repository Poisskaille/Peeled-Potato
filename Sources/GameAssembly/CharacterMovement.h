#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "Components/Rigidbody.hpp"

using namespace TerminaScript;

class CharacterMovement : public TerminaScript::ScriptableComponent
{
public:

	CharacterMovement() = default;
	CharacterMovement(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Start()override;

	void Move();

private:
	Termina::Rigidbody rb;
};