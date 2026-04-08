#include "CharacterMovement.hpp"

void CharacterMovement::Update(float dt)
{
    Jump(dt);
	Shield(dt);
}

void CharacterMovement::Start()
{
	rb = &m_Owner->GetComponent<Termina::Rigidbody>();
}

void CharacterMovement::Jump(float dt)
{
	if (Input::IsKeyPressed(Termina::Key::Space)) rb->AddImpulse(glm::vec3(0,5,0));
}

void CharacterMovement::Shield(float dt)
{
	if (shieldState != PlayerShieldState::None)
	{
		delay += dt;
		if (delay >= 0.5f)
		{
			shieldState = PlayerShieldState::None;
			delay = 0.f;
		}
	}
    else {
		if (Input::IsMouseButtonPressed(Termina::MouseButton::Left)) shieldState = PlayerShieldState::Left;
		else if (Input::IsMouseButtonPressed(Termina::MouseButton::Right)) shieldState = PlayerShieldState::Right;
	}
}

PlayerShieldState CharacterMovement::GetShieldState(){ return shieldState; }
