#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "Score.hpp"

using namespace TerminaScript;

class GameManager : public TerminaScript::ScriptableComponent
{
public:

	GameManager() = default;
	GameManager(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

	void Start()override;
	void Update(float dt)override;
	void UpgradeDifficulty(float dt);

	void NewGame();
	float GetPlayerScore();

	static GameManager* Instance();

	float GetXObstacleSpeed() const { return XobstacleSpeed; }
	float GetYObstacleSpeed() const { return YObstacleSpeed; }

private:
	Score* _score;
	static GameManager* instance;

	float delay = 5.f;
	float XobstacleSpeed = 5.f;
	float YObstacleSpeed = 10.f;
};