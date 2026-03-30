#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class LethalComponent : public TerminaScript::ScriptableComponent
{
public:
	LethalComponent() = default;
	LethalComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void OnCollisionEnter(Termina::Actor* other) override;
};