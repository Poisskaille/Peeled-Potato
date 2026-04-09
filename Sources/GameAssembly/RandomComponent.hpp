#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class RandomComponent : public TerminaScript::ScriptableComponent
{
public:
    RandomComponent() = default;
    RandomComponent(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Awake() override;
    void Start() override {}
    void Update(float dt) override;

    // If true the component will start in Rotate state, otherwise Bob state.
    // This bool is read at Awake() to decide the initial behaviour.
    bool StartRotate = false;

    // Rotation speed in degrees per second when in Rotate state.
    float RotationSpeed = 90.0f;

    // Bob parameters (Y oscillation). Amplitude in world units and speed in cycles/sec.
    float BobAmplitude = 3.0f;
    float BobSpeed = 1.0f;

private:
    enum class State { Rotate, Bob };
    State m_State = State::Rotate;

    float m_StartY = 0.0f;
    float m_Time = 0.0f;
};
