#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class ObstacleComponent : public TerminaScript::ScriptableComponent
{
public:

	ObstacleComponent() = default;
	ObstacleComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;
	void Serialize(nlohmann::json& out) const override;
	void Deserialize(const nlohmann::json& in) override;

	float _speed = 0.5f;

};