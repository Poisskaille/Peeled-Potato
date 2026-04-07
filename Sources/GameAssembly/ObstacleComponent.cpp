#include "ObstacleComponent.hpp"

void ObstacleComponent::Update(float dt)
{
	Move();
	DeleteObstacle(dt);
}

void ObstacleComponent::OnCollisionEnter(Termina::Actor* other)
{
	TN_INFO("Collide");
	Destroy(other);
}

void ObstacleComponent::Move()
{
	switch (_type)
	{
		case ObstacleType::X:
			m_Transform->Translate(glm::vec3(-GameManager::Instance()->GetXObstacleSpeed() * dt, 0, 0));
			break;
		case ObstacleType::Y:
			m_Transform->Translate(glm::vec3(0, -GameManager::Instance()->GetYObstacleSpeed() * dt, 0));
			break;
	}
}

void ObstacleComponent::DeleteObstacle(float dt)
{
	if (m_Transform->GetPosition().x < -10.f && _type == ObstacleType::X)
		Destroy(m_Owner);
	else 
	{
		lifeSpanY += dt;
		if (lifeSpanY >= 5.f)
			Destroy(m_Owner);
	}
}

void ObstacleComponent::SetType(ObstacleType type) { _type = type; }
