#include "GameManager.hpp"

void GameManager::Start()
{
	NewGame();
}

void GameManager::Update(float dt)
{
	if (_score != nullptr)
		_score->Update(dt);
}

void GameManager::NewGame()
{
	_score = new Score();
}

GameManager* GameManager::Instance()
{
	if (instance == nullptr)
		instance = new GameManager();

	return instance;
}
