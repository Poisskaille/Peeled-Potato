#include "ProceduralGenerator.hpp"
#include "ObstacleComponent.hpp"
#include "GameManager.hpp"
#include <random>
#include <Termina/Core/Logger.hpp>
#include <Termina/Renderer/UIUtils.hpp>

static std::mt19937& rng()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

void ProceduralGenerator::Serialize(nlohmann::json& out) const
{
    out["spawnIntervalX"] = SpawnIntervalX;
    out["spawnIntervalZ"] = SpawnIntervalZ;
    out["spawnDistance"] = SpawnDistance;
    out["spread"] = Spread;

    nlohmann::json xlist = nlohmann::json::array();
    for (const auto& p : PrefabsX)
        xlist.push_back(p.Path);
    out["prefabsX"] = std::move(xlist);

    nlohmann::json zlist = nlohmann::json::array();
    for (const auto& p : PrefabsZ)
        zlist.push_back(p.Path);
    out["prefabsZ"] = std::move(zlist);
}

void ProceduralGenerator::Deserialize(const nlohmann::json& in)
{
    SpawnIntervalX = in.value("spawnIntervalX", SpawnIntervalX);
    SpawnIntervalZ = in.value("spawnIntervalZ", SpawnIntervalZ);
    SpawnDistance = in.value("spawnDistance", SpawnDistance);
    Spread = in.value("spread", Spread);

    PrefabsX.clear();
    if (in.contains("prefabsX") && in["prefabsX"].is_array()) {
        for (const auto& v : in["prefabsX"]) {
            std::string path = v.get<std::string>();
            PrefabsX.emplace_back(Prefab(path));
        }
    }

    PrefabsZ.clear();
    if (in.contains("prefabsZ") && in["prefabsZ"].is_array()) {
        for (const auto& v : in["prefabsZ"]) {
            std::string path = v.get<std::string>();
            PrefabsZ.emplace_back(Prefab(path));
        }
    }
}

void ProceduralGenerator::Awake()
{
    // nothing
}

void ProceduralGenerator::Start()
{
}

void ProceduralGenerator::Update(float dt)
{
	GameManager* gm = GameManager::Instance();
	if (gm && !gm->IsPlaying())
		return;

	SpawnObstacleX(dt);
	SpawnObstacleZ(dt);
}

void ProceduralGenerator::Inspect()
{
    ImGui::Text("Prefabs X (%zu):", PrefabsX.size());
    for (size_t i = 0; i < PrefabsX.size(); ++i) {
        ImGui::PushID((int)i);
        bool changed = Termina::UIUtils::PrefabPicker(PrefabsX[i].Path);
        ImGui::SameLine();
        if (ImGui::SmallButton("Remove")) {
            PrefabsX.erase(PrefabsX.begin() + i);
            ImGui::PopID();
            break;
        }
        ImGui::PopID();
    }
    if (ImGui::SmallButton("Add Prefab to X")) {
        PrefabsX.emplace_back(Prefab());
    }

    ImGui::Separator();

    ImGui::Text("Prefabs Z (%zu):", PrefabsZ.size());
    for (size_t i = 0; i < PrefabsZ.size(); ++i) {
        ImGui::PushID((int)i + 1000);
        bool changed = Termina::UIUtils::PrefabPicker(PrefabsZ[i].Path);
        ImGui::SameLine();
        if (ImGui::SmallButton("Remove")) {
            PrefabsZ.erase(PrefabsZ.begin() + i);
            ImGui::PopID();
            break;
        }
        ImGui::PopID();
    }
    if (ImGui::SmallButton("Add Prefab to Z")) {
        PrefabsZ.emplace_back(Prefab());
    }
}

void ProceduralGenerator::SpawnObstacleX(float dt)
{
    static float accumX = 0.f;
    static float accumZ = 0.f;

    accumX += dt;
    accumZ += dt;

    glm::vec3 playerPos(0.0f);

    if (accumX >= SpawnIntervalX) {
        accumX = 0.f;
        if (!PrefabsX.empty()) {
            std::uniform_int_distribution<size_t> pick(0, PrefabsX.size() - 1);
            size_t idx = pick(rng());
            Termina::Actor* a = Instantiate(PrefabsX[idx]);
            if (a) {
                if (a->HasComponent<Termina::Transform>()) {
                    std::uniform_real_distribution<float> ydist(-3.0f, 3.0f);
                    float y = ydist(rng());
                    a->GetComponent<Termina::Transform>().SetPosition(glm::vec3(playerPos.x + SpawnDistance, y, 0.0f));
                    a->GetComponent<Termina::Transform>().SetEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
                }
                if (a->HasComponent<ObstacleComponent>()) {
                    a->GetComponent<ObstacleComponent>().SetType(ObstacleType::X);
                    if(GameManager::Instance()) {
                        a->GetComponent<ObstacleComponent>().SetGeneration(GameManager::Instance()->GetGeneration());
                    }
                }
            }
        }
    }
}

void ProceduralGenerator::SpawnObstacleZ(float dt)
{
    static float accumX = 0.f;
    static float accumZ = 0.f;

    accumX += dt;
    accumZ += dt;

    glm::vec3 playerPos(0.0f);

    if (accumZ >= SpawnIntervalZ) {
        accumZ = 0.f;
        if (!PrefabsZ.empty()) {
            std::uniform_int_distribution<size_t> pick(0, PrefabsZ.size() - 1);
            size_t idx = pick(rng());
            Termina::Actor* a = Instantiate(PrefabsZ[idx]);
            if (a) {
                std::uniform_int_distribution<int> sideDist(0, 1);
                bool isRight = (sideDist(rng()) == 0);
                float sideX = isRight ? SpawnDistance : -SpawnDistance;
                if (a->HasComponent<Termina::Transform>()) {
                    a->GetComponent<Termina::Transform>().SetPosition(glm::vec3(0.0f, 0.0f, playerPos.z + sideX));
                    a->GetComponent<Termina::Transform>().SetEulerAngles(glm::vec3(0.0f, 90.0f, 0.0f));
                }
                if (a->HasComponent<ObstacleComponent>()) {
                    auto& oc = a->GetComponent<ObstacleComponent>();
                    oc.SetType(ObstacleType::Z);
                    oc.SetSide(isRight ? ObstacleComponent::Side::Right : ObstacleComponent::Side::Left);

                    if(GameManager::Instance()) {
                        oc.SetGeneration(GameManager::Instance()->GetGeneration());
                    }
                }
            }
        }
    }
}
