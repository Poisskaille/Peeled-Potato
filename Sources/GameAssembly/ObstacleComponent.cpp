#include "ObstacleComponent.hpp"

void ObstacleComponent::Update(float dt)
{
	glm::vec3 pos = m_Transform->GetPosition();
	pos.x -= _speed * dt;
	m_Transform->SetPosition(pos);
}

void ObstacleComponent::OnCollisionEnter(Termina::Actor* other)
{
	TN_INFO("Collide");
	Destroy(other);
}
