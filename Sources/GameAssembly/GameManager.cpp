#include "GameManager.hpp"
#include "MenuComponent.h"
#include "ObstacleComponent.hpp"

GameManager* GameManager::instance = nullptr;

void GameManager::Start()
{
	// register singleton instance when component is started
	instance = this;
	_score = nullptr;
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
	if (m_requestGameOver) {
		m_requestGameOver = false;
		m_isPlaying = false;

		TN_INFO("Game Over requested! Destroying obstacles...");
		// Nettoyage absolu de la map
		ObstacleComponent::DestroyAllObstacles();

		// Enregistrer le meilleur score/temps
		if (_score && _score->GetScore() > m_bestScore) {
			m_bestScore = _score->GetScore();
		}

		TN_INFO("Trying to open menu...");
		MenuComponent* menu = MenuComponent::Instance();
		if (menu) {
			menu->OpenMenu();
			TN_INFO("Menu opened!");
		} else {
			TN_ERROR("MenuComponent instance not found!");
		}
		return;
	}

	if (!m_isPlaying)
		return;

	if (_score != nullptr)
		_score->Update(dt);

	UpgradeDifficulty(dt);
}

void GameManager::NewGame()
{
	if (_score) delete _score;
	_score = new Score();
	m_isPlaying = true;
	m_generation++;

	delay = 5.f;
	XobstacleSpeed = 5.f;
	YObstacleSpeed = 10.f;
}

float GameManager::GetPlayerScore(){ return _score ? _score->GetScore() : 0.0f; }

GameManager* GameManager::Instance()
{
	return instance;
}
