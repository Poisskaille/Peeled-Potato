#include "ProceduralGenerator.hpp"

#include <ImGui/imgui.h>
#include <random>
#include "ObstacleComponent.hpp"

static std::mt19937& GetRng()
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

void ProceduralGenerator::Update(float dt)
{
    for (size_t i = 0; i < m_SpawnedActors.size(); ++i) {
        Termina::Actor* a = m_SpawnedActors[i];
        if (!a) {
            m_SpawnedActors.erase(m_SpawnedActors.begin() + i);
            --i;
            continue;
        }
        float ax = a->GetComponent<Termina::Transform>().GetPosition().x;
        if (ax <= m_DestroyX) {
            Destroy(a);
            m_SpawnedActors.erase(m_SpawnedActors.begin() + i);
            --i;
            if (m_CurrentSpawned > 0) --m_CurrentSpawned;
            continue;
        }
    }

    for (size_t i = 0; i < m_SpawnedActorsY.size(); ++i) {
        Termina::Actor* a = m_SpawnedActorsY[i];
        if (!a) {
            m_SpawnedActorsY.erase(m_SpawnedActorsY.begin() + i);
            --i;
            continue;
        }
        float ay = a->GetComponent<Termina::Transform>().GetPosition().y;
        if (ay <= -10.0f) {
            Destroy(a);
            m_SpawnedActorsY.erase(m_SpawnedActorsY.begin() + i);
            --i;
            if (m_CurrentSpawnedY > 0) --m_CurrentSpawnedY;
            continue;
        }
    }

    if (!m_Pool.empty()) {
        m_Timer += dt;
        if (m_Timer >= m_SpawnInterval) {
            m_Timer = 0.0f;
            if (m_CurrentSpawned < m_MaxSpawned) {
                std::uniform_int_distribution<size_t> idxDist(0, m_Pool.size() - 1);
                size_t idx = idxDist(GetRng());
                const Prefab& prefab = m_Pool[idx];
                if (prefab.IsValid()) {
                    glm::vec3 pos = m_Transform->GetPosition();
                    std::uniform_real_distribution<float> xDist(m_SpawnOffsetMinX, m_SpawnOffsetMaxX);
                    std::uniform_real_distribution<float> yDist(m_SpawnOffsetMinY, m_SpawnOffsetMaxY);
                    std::uniform_real_distribution<float> zDist(m_SpawnOffsetMinZ, m_SpawnOffsetMaxZ);
                    pos.x += xDist(GetRng());
                    pos.y += yDist(GetRng());
                    pos.z += zDist(GetRng());

                    Termina::Actor* actor = Instantiate(prefab);
                    if (actor) {
                        actor->GetComponent<Termina::Transform>().SetPosition(pos);
                        if (actor->HasComponent<ObstacleComponent>())
                            actor->GetComponent<ObstacleComponent>().SetType(ObstacleType::X);
                        m_SpawnedActors.push_back(actor);
                        ++m_CurrentSpawned;
                    }
                }
            }
        }
    }

    if (!m_PoolY.empty()) {
        m_TimerY += dt;
        if (m_TimerY >= m_SpawnIntervalY) {
            m_TimerY = 0.0f;
            if (m_CurrentSpawnedY < m_MaxSpawnedY) {
                std::uniform_int_distribution<size_t> idxDistY(0, m_PoolY.size() - 1);
                size_t idxY = idxDistY(GetRng());
                const Prefab& prefabY = m_PoolY[idxY];
                if (prefabY.IsValid()) {
                    glm::vec3 pos(0.0f);
                    std::uniform_int_distribution<int> sideDist(0, 1);
                    int side = sideDist(GetRng()) == 0 ? -1 : 1;
                    pos.x = (side > 0) ? m_SpawnDistanceXForY : -m_SpawnDistanceXForY;
                    std::uniform_real_distribution<float> yDist(m_SpawnOffsetMinY, m_SpawnOffsetMaxY);
                    pos.y = yDist(GetRng());
                    pos.z = side * m_SpawnSideOffsetZ;

                    Termina::Actor* actorY = Instantiate(prefabY);
                    if (actorY) {
                        actorY->GetComponent<Termina::Transform>().SetPosition(pos);
                        if (actorY->HasComponent<ObstacleComponent>())
                            actorY->GetComponent<ObstacleComponent>().SetType(ObstacleType::Y);
                        m_SpawnedActorsY.push_back(actorY);
                        ++m_CurrentSpawnedY;
                    }
                }
            }
        }
    }
}

void ProceduralGenerator::Serialize(nlohmann::json& out) const
{
    out["Spawn Interval"] = m_SpawnInterval;
    out["Max Spawned"] = m_MaxSpawned;
    out["Destroy X"] = m_DestroyX;
    out["SpawnOffsetMinZ"] = m_SpawnOffsetMinZ;
    out["SpawnOffsetMaxZ"] = m_SpawnOffsetMaxZ;
    out["Spawn Interval Y"] = m_SpawnIntervalY;
    out["Max Spawned Y"] = m_MaxSpawnedY;
    out["Spawn Distance X For Y"] = m_SpawnDistanceXForY;
    out["Spawn Side Offset Z"] = m_SpawnSideOffsetZ;
    out["Pool"] = nlohmann::json::array();
    for (const auto& p : m_Pool) out["Pool"].push_back(p.Path);
    out["PoolY"] = nlohmann::json::array();
    for (const auto& p : m_PoolY) out["PoolY"].push_back(p.Path);
}

void ProceduralGenerator::Deserialize(const nlohmann::json& in)
{
    if (in.contains("Spawn Interval")) m_SpawnInterval = in["Spawn Interval"];
    if (in.contains("Max Spawned")) m_MaxSpawned = in["Max Spawned"];
    if (in.contains("Destroy X")) m_DestroyX = in["Destroy X"];
    if (in.contains("SpawnOffsetMinZ")) m_SpawnOffsetMinZ = in["SpawnOffsetMinZ"];
    if (in.contains("SpawnOffsetMaxZ")) m_SpawnOffsetMaxZ = in["SpawnOffsetMaxZ"];
    if (in.contains("Spawn Interval Y")) m_SpawnIntervalY = in["Spawn Interval Y"];
    if (in.contains("Max Spawned Y")) m_MaxSpawnedY = in["Max Spawned Y"];
    if (in.contains("Spawn Distance X For Y")) m_SpawnDistanceXForY = in["Spawn Distance X For Y"];
    if (in.contains("Spawn Side Offset Z")) m_SpawnSideOffsetZ = in["Spawn Side Offset Z"];
    if (in.contains("Pool") && in["Pool"].is_array()) {
        m_Pool.clear();
        for (auto& v : in["Pool"]) {
            if (v.is_string()) m_Pool.emplace_back(v.get<std::string>());
        }
    }
    if (in.contains("PoolY") && in["PoolY"].is_array()) {
        m_PoolY.clear();
        for (auto& v : in["PoolY"]) {
            if (v.is_string()) m_PoolY.emplace_back(v.get<std::string>());
        }
    }
}

void ProceduralGenerator::Inspect()
{
    ImGui::DragFloat("Spawn Interval", &m_SpawnInterval, 0.01f);
    ImGui::DragInt("Max Spawned", &m_MaxSpawned, 1);
    ImGui::DragFloat("Destroy X", &m_DestroyX, 0.1f);
    ImGui::DragFloat("Spawn Min Z", &m_SpawnOffsetMinZ, 0.1f);
    ImGui::DragFloat("Spawn Max Z", &m_SpawnOffsetMaxZ, 0.1f);

    ImGui::Text("Pool:");
    for (size_t i = 0; i < m_Pool.size(); ++i) {
        char buf[64];
        sprintf(buf, "Prefab %zu", i);
        if (m_Pool[i].Inspect(buf)) {
        }
        ImGui::SameLine();
        if (ImGui::Button((std::string("Remove##") + std::to_string(i)).c_str())) {
            m_Pool.erase(m_Pool.begin() + i);
            --i;
            continue;
        }
    }

    if (ImGui::Button("Add Empty Prefab")) {
        m_Pool.emplace_back();
    }

    ImGui::Separator();
    ImGui::Text("Y-type Obstacles:");
    ImGui::DragFloat("Spawn Interval Y", &m_SpawnIntervalY, 0.01f);
    ImGui::DragInt("Max Spawned Y", &m_MaxSpawnedY, 1);
    ImGui::DragFloat("Spawn Distance X For Y", &m_SpawnDistanceXForY, 0.1f);
    ImGui::DragFloat("Spawn Side Offset Z", &m_SpawnSideOffsetZ, 0.1f);

    ImGui::Text("Pool Y:");
    for (size_t i = 0; i < m_PoolY.size(); ++i) {
        char buf[64];
        sprintf(buf, "PrefabY %zu", i);
        if (m_PoolY[i].Inspect(buf)) {}
        ImGui::SameLine();
        if (ImGui::Button((std::string("RemoveY##") + std::to_string(i)).c_str())) {
            m_PoolY.erase(m_PoolY.begin() + i);
            --i;
            continue;
        }
    }

    if (ImGui::Button("Add Empty Prefab Y")) {
        m_PoolY.emplace_back();
    }
}

void ProceduralGenerator::AddPrefab(const Prefab& prefab)
{
    if (prefab.IsValid()) m_Pool.push_back(prefab);
}

void ProceduralGenerator::AddPrefabY(const Prefab& prefab)
{
    if (prefab.IsValid()) m_PoolY.push_back(prefab);
}

void ProceduralGenerator::ClearPool(){ m_Pool.clear(); }

void ProceduralGenerator::ClearPoolY(){ m_PoolY.clear(); }
