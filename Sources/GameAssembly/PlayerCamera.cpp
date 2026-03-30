#include "PlayerCamera.hpp"

void PlayerCamera::Serialize(nlohmann::json& out) const
{
    //out["Sensitivity"] = Sensitivity;
    //out["Pitch"] = Pitch;
    //out["Yaw"] = Yaw;
}

void PlayerCamera::Deserialize(const nlohmann::json& in)
{
    //if (in.contains("Sensitivity")) Sensitivity = in["Sensitivity"];
    //if (in.contains("Pitch"))       Pitch = in["Pitch"];
    //if (in.contains("Yaw"))         Yaw = in["Yaw"];
}

void PlayerCamera::Update(float dt)
{
    //glm::vec2 mouseDelta = Input::GetMouseDelta();

    //Yaw += mouseDelta.x * Sensitivity;
    //Pitch += mouseDelta.y * Sensitivity;

    //Pitch = glm::clamp(Pitch, -89.0f, 89.0f);

    //glm::quat yawQuat = glm::angleAxis(glm::radians(-Yaw), glm::vec3(0, 1, 0));
    //glm::quat pitchQuat = glm::angleAxis(glm::radians(Pitch), glm::vec3(1, 0, 0));

    //m_Transform->SetRotation(yawQuat * pitchQuat);
}

void PlayerCamera::Start()
{
/*	cam = m_Owner->GetComponent<Termina::CameraComponent>();
	Input::SetCursorLocked(true);
	Input::SetCursorVisible(false)*/;
}