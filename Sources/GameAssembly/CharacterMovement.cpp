#include "CharacterMovement.h"

void CharacterMovement::Update(float dt)
{
	Move(dt);
}

void CharacterMovement::Start()
{
	rb = m_Owner->GetComponent<Termina::Rigidbody>();
	auto allChildren = m_Owner->GetChildren();

	for (Termina::Actor* child : allChildren)
	{
		try
		{
			cam = child->GetComponent<Termina::CameraComponent>();
			break;
		}
		catch(...){}
	}
	
}

void CharacterMovement::Move(float dt)
{
	glm::vec3 camForward = cam.GetCamera().Direction;
	glm::vec3 camRight = glm::vec3(cam.GetCamera().InverseView[0]);

	camForward.y = 0.0f;
	camRight.y = 0.0f;

	if (glm::length(camForward) > 0.001f) camForward = glm::normalize(camForward);
	if (glm::length(camRight) > 0.001f) camRight = glm::normalize(camRight);

	glm::vec3 impulse(0,0,0);
	if (Input::IsKeyHeld(Termina::Key::W)) impulse += camForward * 20.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::S)) impulse += camForward * -20.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::D)) impulse += camRight * 20.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::A)) impulse += camRight * -20.0f * dt;
	if (Input::IsKeyPressed(Termina::Key::Space)) impulse.y += 5.0f;

	rb.AddImpulse(impulse);

	FixSlide(dt,impulse);
}


void CharacterMovement::FixSlide(float dt, glm::vec3 impulse)
{
	glm::vec3 velocity = rb.GetLinearVelocity(); 

	glm::vec3 horizontalVelocity = glm::vec3(velocity.x, 0.0f, velocity.z);

	if (glm::length(horizontalVelocity) > 0.001f)
	{
		float friction = (impulse == glm::vec3(0.0f)) ? 15.0f : 8.0f;
		glm::vec3 frictionForce = -glm::normalize(horizontalVelocity) * friction * dt;

		if (glm::length(frictionForce) > glm::length(horizontalVelocity))
			frictionForce = -horizontalVelocity;

		rb.AddImpulse(frictionForce);
	}
}
