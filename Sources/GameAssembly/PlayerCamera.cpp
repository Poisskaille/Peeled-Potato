#include "PlayerCamera.h"

void PlayerCamera::Update(float dt)
{
    glm::vec2 mouseDelta = Input::GetMouseDelta();

    Yaw += mouseDelta.x * Sensitivity;
    Pitch += mouseDelta.y * Sensitivity;

    Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

    glm::vec3 direction;
    direction.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    direction.y = -sin(glm::radians(Pitch));
    direction.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

    cam.GetCamera().Direction = glm::normalize(direction);
    
}

void PlayerCamera::Start()
{
	cam = m_Owner->GetComponent<Termina::CameraComponent>();
	Input::SetCursorLocked(true);
	Input::SetCursorVisible(false);
}