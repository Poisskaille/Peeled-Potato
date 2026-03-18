#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class RotateComponent : public TerminaScript::ScriptableComponent
{
public:

	RotateComponent() = default;
	RotateComponent(Termina::Actor* owner): TerminaScript::ScriptableComponent(owner){}

	void Update(float dt)override;
	void Serialize(nlohmann::json& out) const override;
	void Deserialize(const nlohmann::json& in) override;

	void Inspect() override;

	float m_rotateSpeed = 1.f;

};