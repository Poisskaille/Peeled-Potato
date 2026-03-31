#include "CharacterMovement.hpp"

void CharacterMovement::Update(float dt)
{
	Jump(dt);
}

void CharacterMovement::Start()
{
	rb = &m_Owner->GetComponent<Termina::Rigidbody>();
}

void CharacterMovement::Jump(float dt)
{
	if (Input::IsKeyPressed(Termina::Key::Space)) rb->AddImpulse(glm::vec3(0,5,0));
}
