#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class FlyCamComponent : public TerminaScript::ScriptableComponent
{
public:
    FlyCamComponent() = default;
    FlyCamComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Update(float deltaTime) override;

    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    float m_MoveSpeed      = 5.0f;
    float m_FastMultiplier = 4.0f;
    float m_Sensitivity    = 0.15f;
    float m_Acceleration   = 20.0f;

private:
    float     m_Yaw         = 0.0f;
    float     m_Pitch       = 0.0f;
    glm::vec3 m_Velocity    = glm::vec3(0.0f);
    bool      m_WasRotating = false;
};
