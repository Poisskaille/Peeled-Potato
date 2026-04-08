#include "ObstacleComponent.hpp"
#include <cmath>

void ObstacleComponent::Update(float dt)
{
	Move(dt);
	DeleteObstacle(dt);
}

void ObstacleComponent::Start()
{
    if (!m_Transform && m_Owner) {
		if (m_Owner->HasComponent<Termina::Transform>())
			m_Transform = &m_Owner->GetComponent<Termina::Transform>();
	}
	if (!m_Transform) return;

	glm::vec3 pos = m_Transform->GetPosition();
	m_StartDistance = glm::length(pos);
}

void ObstacleComponent::OnCollisionEnter(Termina::Actor* other)
{
	TN_INFO("Collide");
	Destroy(other);
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
	if (_type == ObstacleType::X) {
		if (std::abs(pos.x) <= -5.0f)
			Destroy(m_Owner);
	} else if (_type == ObstacleType::Z) {
		if (std::abs(pos.z) <= 5.0f)
			Destroy(m_Owner);
	}
}

void ObstacleComponent::SetType(ObstacleType type) { _type = type; }
