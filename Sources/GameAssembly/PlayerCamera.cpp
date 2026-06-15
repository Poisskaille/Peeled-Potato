#include "PlayerCamera.hpp"

void PlayerCamera::Serialize(nlohmann::json& out) const
{
}

void PlayerCamera::Deserialize(const nlohmann::json& in)
{
}

void PlayerCamera::Update(float dt)
{
    bool left = Input::IsKeyHeld(Termina::Key::A);
    bool right = Input::IsKeyHeld(Termina::Key::E);

    if (left && !right) m_TargetYawOffset = -m_SideLookAngle;
    else if (right && !left) m_TargetYawOffset = m_SideLookAngle;
    else m_TargetYawOffset = 0.0f;

    float t = 1.0f - std::exp(-m_LerpSpeed * dt);
    m_CurrentYawOffset = glm::mix(m_CurrentYawOffset, m_TargetYawOffset, t);

    glm::quat yawQuat = glm::angleAxis(glm::radians(-m_CurrentYawOffset), glm::vec3(0, 1, 0));
    m_Transform->SetRotation(yawQuat * m_BaseRotation);
}

void PlayerCamera::Start()
{
    m_BaseRotation = m_Transform->GetRotation();
}
