#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class ForwardComponent : public TerminaScript::ScriptableComponent
{
public:

	ForwardComponent() = default;
	ForwardComponent(Termina::Actor* owner): TerminaScript::ScriptableComponent(owner){}

	void Update(float dt)override;


};