#pragma once

#include <Termina/Scripting/API/ScriptingAPI.hpp>
#include "RandomComponent.hpp"
#include <vector>

using namespace TerminaScript;

class ProceduralGenerator : public TerminaScript::ScriptableComponent
{
public:
    ProceduralGenerator() = default;
    ProceduralGenerator(Termina::Actor* owner) : TerminaScript::ScriptableComponent(owner) {}

    void Awake() override;
    void Start() override;
    void Update(float dt) override;
    void Inspect() override;
    void Serialize(nlohmann::json& out) const override;
    void Deserialize(const nlohmann::json& in) override;

	void SpawnObstacleX(float dt);
	void SpawnObstacleZ(float dt);

private:
    std::vector<Prefab> PrefabsX; 
    std::vector<Prefab> PrefabsZ; 

    float SpawnIntervalX = 2.0f;
    float SpawnIntervalZ = 5.0f;

    float SpawnDistance = 25.0f;
    float Spread = 20.0f;
};
