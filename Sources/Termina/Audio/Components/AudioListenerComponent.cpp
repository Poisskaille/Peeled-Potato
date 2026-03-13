#include "AudioListenerComponent.hpp"

#include <Termina/World/Actor.hpp>
#include <Termina/World/World.hpp>

#include <ImGui/imgui.h>

namespace Termina {
    AudioListenerComponent::~AudioListenerComponent()
    {
        if (m_Primary)
            m_Owner->GetParentWorld()->SetAudioListener(nullptr);
    }

    void AudioListenerComponent::OnPreUpdate(float deltaTime)
    {
        if (m_Primary)
            m_Owner->GetParentWorld()->SetAudioListener(m_Owner);
    }

    void AudioListenerComponent::Inspect()
    {
        ImGui::Checkbox("Primary", &m_Primary);
        ImGui::InputFloat3("Velocity", &m_Velocity.x);
    }

    void AudioListenerComponent::Serialize(nlohmann::json& out) const
    {
        out["primary"] = m_Primary;
        out["velocity"] = { m_Velocity.x, m_Velocity.y, m_Velocity.z };
    }

    void AudioListenerComponent::Deserialize(const nlohmann::json& in)
    {
        m_Primary = in.value("primary", false);
        if (in.contains("velocity")) {
            m_Velocity = glm::vec3(in["velocity"][0], in["velocity"][1], in["velocity"][2]);
        }
    }
}
