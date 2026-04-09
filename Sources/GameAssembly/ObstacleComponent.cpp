#include "ObstacleComponent.hpp"
#include "CharacterMovement.hpp"
#include "MenuComponent.h"
#include <cmath>
#include <algorithm>

std::vector<Termina::Actor*> ObstacleComponent::s_AllObstacles;

void ObstacleComponent::DestroyAllObstacles()
{
	std::vector<Termina::Actor*> copy = s_AllObstacles;
	for (auto* obs : copy) {
		if (obs && obs->HasComponent<ObstacleComponent>()) {
			obs->GetComponent<ObstacleComponent>().SelfDestroy();
		}
	}
	s_AllObstacles.clear();
}

void ObstacleComponent::Update(float dt)
{
	GameManager* gm = GameManager::Instance();

	if (gm && gm->IsPlaying() && m_generation != gm->GetGeneration()) {
		if (!m_destroyedFlag) {
			m_destroyedFlag = true;
			Destroy(m_Owner);
		}
		return;
	}

	if (gm && !gm->IsPlaying()) {
		// On met en pause, pas de destruction sinon l'éditeur crashe !
		return;
	}

	Move(dt);
	DeleteObstacle(dt);
}

void ObstacleComponent::Start()
{
	s_AllObstacles.push_back(m_Owner);

	if (!m_Transform && m_Owner) {
		if (m_Owner->HasComponent<Termina::Transform>())
			m_Transform = &m_Owner->GetComponent<Termina::Transform>();
	}
	if (!m_Transform) return;

	glm::vec3 pos = m_Transform->GetPosition();
	m_StartDistance = glm::length(pos);
}

void ObstacleComponent::Stop()
{
	auto it = std::find(s_AllObstacles.begin(), s_AllObstacles.end(), m_Owner);
	if (it != s_AllObstacles.end()) {
		s_AllObstacles.erase(it);
	}
}

void ObstacleComponent::OnTriggerEnter(Termina::Actor* other)
{
	if (other && other->HasComponent<CharacterMovement>()) {
		TN_INFO("Player hit an obstacle!");

		GameManager* gm = GameManager::Instance();
		if (gm) gm->TriggerGameOver();
	}
}

void ObstacleComponent::Move(float dt)
{
	if (!m_Transform && m_Owner) {
		if (m_Owner->HasComponent<Termina::Transform>())
			m_Transform = &m_Owner->GetComponent<Termina::Transform>();
	}
	if (!m_Transform) return;

	if (m_StartDistance <= 0.0f) {
		glm::vec3 pos = m_Transform->GetPosition();
		m_StartDistance = glm::length(pos);
	}

	switch (_type)
	{
    case ObstacleType::X:
            {
				GameManager* gm = GameManager::Instance();
				float speed = gm ? gm->GetXObstacleSpeed() : 5.0f;
				m_Transform->Translate(glm::vec3(-speed * dt, 0, 0));
			}
			break;
    case ObstacleType::Z:
		{
            GameManager* gm = GameManager::Instance();
			float baseSpeed = gm ? gm->GetXObstacleSpeed() : 5.0f;
			const float referenceDistance = 100.0f;
			float scale = 1.0f;
			if (m_StartDistance > 0.0001f)
				scale = m_StartDistance / referenceDistance;
			float speed = baseSpeed * scale;
			m_Transform->Translate(glm::vec3(0, 0, -speed * dt));
		}
		break;
	}
}

void ObstacleComponent::DeleteObstacle(float dt)
{
	glm::vec3 pos = m_Transform->GetPosition();
	bool toDestroy = false;

	if (_type == ObstacleType::X) {
		if (pos.x <= -60.0f)
			toDestroy = true;
	} else if (_type == ObstacleType::Z) {
		if (std::abs(pos.z) >= 80.0f)
			toDestroy = true;
	}

	if (toDestroy) {
		auto it = std::find(s_AllObstacles.begin(), s_AllObstacles.end(), m_Owner);
		if (it != s_AllObstacles.end()) s_AllObstacles.erase(it);
		Destroy(m_Owner);
	}
}

void ObstacleComponent::SetType(ObstacleType type) { _type = type; }
