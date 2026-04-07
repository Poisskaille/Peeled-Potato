#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>

using namespace TerminaScript;

class ProceduralGenerator : public TerminaScript::ScriptableComponent
{
public:
    ProceduralGenerator() = default;
    ProceduralGenerator(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Awake() override {}
    void Start() override {}
    void Update(float dt) override;

    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

    void Inspect() override;

    void AddPrefab(const Prefab& prefab);
    void AddPrefabY(const Prefab& prefab);
    void ClearPool();
    void ClearPoolY();

private:
    std::vector<Prefab> m_Pool;
    // Separate pool for Y-type obstacles (user requested a separate prefab)
    std::vector<Prefab> m_PoolY;

    float m_SpawnInterval = 1.0f;
    float m_Timer = 0.0f;
    int m_MaxSpawned = 50;

    float m_SpawnOffsetMinX = 8.0f;
    float m_SpawnOffsetMaxX = 12.0f;
    float m_SpawnOffsetMinY = -1.0f;
    float m_SpawnOffsetMaxY = 3.0f;

    int m_CurrentSpawned = 0;

    std::vector<Termina::Actor*> m_SpawnedActors;
    std::vector<Termina::Actor*> m_SpawnedActorsY;

    float m_DestroyX = -5.0f;

    float m_SpawnOffsetMinZ = -1.0f;
    float m_SpawnOffsetMaxZ = 1.0f;
    // For Y spawns: distance on X where they should appear (relative to world origin)
    float m_SpawnDistanceXForY = 10.0f;
    // Lateral offset magnitude for right/left spawn (Z axis)
    float m_SpawnSideOffsetZ = 4.0f;
};
