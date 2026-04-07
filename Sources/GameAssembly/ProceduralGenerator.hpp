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

    // Runtime API
    void AddPrefab(const Prefab& prefab);
    void ClearPool();

private:
    std::vector<Prefab> m_Pool;

    // Spawning configuration
    float m_SpawnInterval = 1.0f; // seconds between spawns
    float m_Timer = 0.0f;
    int m_MaxSpawned = 50;

    // Simple positional randomization around the generator's transform
    float m_SpawnOffsetMinX = 8.0f;
    float m_SpawnOffsetMaxX = 12.0f;
    float m_SpawnOffsetMinY = -1.0f;
    float m_SpawnOffsetMaxY = 3.0f;

    // Track spawned actors count (optional, cleaned when actor is destroyed externally)
    int m_CurrentSpawned = 0;

    // Active spawned actors (used to cleanup/destroy when they pass the player)
    std::vector<Termina::Actor*> m_SpawnedActors;

    // When spawned obstacles pass this X (world) position they will be destroyed
    float m_DestroyX = -5.0f;

    // Optional Z variance for 3D placement (player is at Z=0)
    float m_SpawnOffsetMinZ = -1.0f;
    float m_SpawnOffsetMaxZ = 1.0f;
};
