#include "GameManager.hpp"

GameManager* GameManager::instance = nullptr;

void GameManager::Start()
{
	// register singleton instance when component is started
	instance = this;
	_score = nullptr;
	NewGame();
}

void GameManager::UpgradeDifficulty(float dt)
{
	delay += dt;
	if (delay >= 5.f)
	{
		delay = 0.f;
		XobstacleSpeed += 5.5f;
		YObstacleSpeed += 2.f;
	}
}
void GameManager::Update(float dt)
{
	if (_score != nullptr)
		_score->Update(dt);

	UpgradeDifficulty(dt);
}

void GameManager::NewGame()
{
	if (_score) delete _score;
	_score = new Score();
}

float GameManager::GetPlayerScore(){ return _score ? _score->GetScore() : 0.0f; }

GameManager* GameManager::Instance()
{
	return instance;
}
