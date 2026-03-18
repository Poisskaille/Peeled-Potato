#include "CharacterMovement.h"

void CharacterMovement::Update(float dt)
{
	Move();
}

void CharacterMovement::Start()
{
	rb = m_Owner->GetComponent(Termina::Rigidbody);
}

void CharacterMovement::Move()
{
	if (Input::IsKeyPressed(Termina::Key::Space))
		rb.AddImpulse(glm::vec3(0,100,0);
}
