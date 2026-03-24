#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class TrapdoorComponent : public TerminaScript::ScriptableComponent
{
public:

	TrapdoorComponent() = default;
	TrapdoorComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}


	void OnCollisionEnter(Termina::Actor* other) override;
};