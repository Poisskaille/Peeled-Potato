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
	float GetBestScore() const { return m_bestScore; }

	static GameManager* Instance();

	float GetXObstacleSpeed() const { return XobstacleSpeed; }
	float GetYObstacleSpeed() const { return YObstacleSpeed; }

	bool IsPlaying() const { return m_isPlaying; }
	void SetPlaying(bool playing) { m_isPlaying = playing; }

	int GetGeneration() const { return m_generation; }
	void TriggerGameOver() { m_requestGameOver = true; }

private:
	Score* _score;
	static GameManager* instance;

	bool m_isPlaying = false;
	int m_generation = 0;
	bool m_requestGameOver = false;
	float m_bestScore = 0.0f;

	float delay = 5.f;
	float XobstacleSpeed = 5.f;
	float YObstacleSpeed = 10.f;
};