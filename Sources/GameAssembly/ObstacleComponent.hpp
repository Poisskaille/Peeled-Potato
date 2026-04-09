#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include <Termina/Physics/Components/Rigidbody.hpp>
#include <Termina/Core/Logger.hpp>
#include "GameManager.hpp"
#include <Termina/Renderer/Components/CameraComponent.hpp>

using namespace TerminaScript;

enum class ObstacleType{X, Z};

class ObstacleComponent : public TerminaScript::ScriptableComponent
{
public:

	ObstacleComponent() = default;
	ObstacleComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	static std::vector<Termina::Actor*> s_AllObstacles;
	static void DestroyAllObstacles();
	void SelfDestroy() { Destroy(m_Owner); }

	void Update(float dt)override;
	void Start() override;
	void Stop() override;
	void OnTriggerEnter(Termina::Actor* other)override;

	void Move(float dt);
	void DeleteObstacle(float dt);
	
	void SetType(ObstacleType type);
	enum Side { Left = -1, Right = 1 };
	void SetSide(Side s) { m_Side = s; }

    void SetGeneration(int gen) { m_generation = gen; }

private:
	ObstacleType _type = ObstacleType::X;

	float m_StartDistance = 0.f;
    Side m_Side = Right;

    int m_generation = 0;
    bool m_destroyedFlag = false;

	Termina::Transform* m_Transform = nullptr;
};