#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class LaserComponent : public TerminaScript::ScriptableComponent
{
public:

	LaserComponent() = default;
	LaserComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Update(float dt)override;


	void Inspect() override;

	float m_laserSize= 1.f;

	void OnCollisionEnter(Termina::Actor* other) override;
};