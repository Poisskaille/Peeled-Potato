#include "RandomComponent.hpp"
#include <GLM/glm.hpp>
#include <GLM/gtc/quaternion.hpp>
#include <GLM/gtx/euler_angles.hpp>
#include <cmath>

void RandomComponent::Awake()
{
    // decide state from StartRotate flag
    m_State = StartRotate ? State::Rotate : State::Bob;
    if (m_Owner && m_Owner->HasComponent<Termina::Transform>()) {
        m_StartY = m_Owner->GetComponent<Termina::Transform>().GetPosition().y;
    } else if (m_Transform) {
        m_StartY = m_Transform->GetPosition().y;
    }
}

void RandomComponent::Update(float dt)
{
    // ensure transform pointer
    if (!m_Transform && m_Owner) {
        if (m_Owner->HasComponent<Termina::Transform>())
            m_Transform = &m_Owner->GetComponent<Termina::Transform>();
    }
    if (!m_Transform) return;

    m_Time += dt;
    switch (m_State) {
    case State::Rotate: {
        // rotate around Z axis
        float angle = RotationSpeed * dt; // degrees
        m_Transform->RotateEuler(glm::vec3(0.0f, 0.0f, angle));
        break;
    }
    case State::Bob: {
        float y = m_StartY + BobAmplitude * std::sin(2.0f * 3.14159265f * BobSpeed * m_Time);
        glm::vec3 pos = m_Transform->GetPosition();
        m_Transform->SetPosition(glm::vec3(pos.x, y, pos.z));
        break;
    }
    }
}
