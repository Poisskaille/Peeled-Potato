#include "CharacterMovement.h"

void CharacterMovement::Update(float dt)
{
	Move(dt);
}

void CharacterMovement::Start()
{
	rb = m_Owner->GetComponent<Termina::Rigidbody>();
	cam = m_Owner->GetComponent<Termina::CameraComponent>();
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
	if (Input::IsKeyHeld(Termina::Key::W)) impulse += camForward * 10.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::S)) impulse += camForward * -10.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::D)) impulse += camRight * 10.0f * dt;
	if (Input::IsKeyHeld(Termina::Key::A)) impulse += camRight * -10.0f * dt;
	if (Input::IsKeyPressed(Termina::Key::Space)) impulse.y += 5.0f;

	rb.AddImpulse(impulse);
}
