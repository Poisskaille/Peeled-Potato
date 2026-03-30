#include "CharacterMovement.hpp"

void CharacterMovement::Update(float dt)
{
	Move(dt);

	if (cam != nullptr)
		TN_DEBUG("CACA PAS NUL");

	if (cam == nullptr)
		TN_DEBUG("CACA NUL");

	TN_DEBUG("CACA");
}

void CharacterMovement::Start()
{
	rb = m_Owner->GetComponent<Termina::Rigidbody>();
}

void CharacterMovement::Move(float dt)
{
	glm::vec3 impulse(0,0,-1);

	if (Input::IsKeyPressed(Termina::Key::Space)) impulse.y += 5.0f;

	rb.AddImpulse(impulse);

	glm::vec3 vel = rb.GetLinearVelocity();
	vel.z = -1;
	rb.SetLinearVelocity(vel);


}
