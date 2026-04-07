#include "ProceduralGenerator.hpp"

#include <ImGui/imgui.h>
#include <random>

static std::mt19937& GetRng()
{
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

void ProceduralGenerator::Update(float dt)
{
    if (m_Pool.empty())
        return;

    m_Timer += dt;
    if (m_Timer < m_SpawnInterval)
        return;

    m_Timer = 0.0f;

    if (m_CurrentSpawned >= m_MaxSpawned)
        return;

    // Choose random prefab from pool
    std::uniform_int_distribution<size_t> idxDist(0, m_Pool.size() - 1);
    size_t idx = idxDist(GetRng());
    const Prefab& prefab = m_Pool[idx];
    if (!prefab.IsValid())
        return;

    // compute spawn position relative to generator
    glm::vec3 pos = m_Transform->GetPosition();
    std::uniform_real_distribution<float> xDist(m_SpawnOffsetMinX, m_SpawnOffsetMaxX);
    std::uniform_real_distribution<float> yDist(m_SpawnOffsetMinY, m_SpawnOffsetMaxY);
    pos.x += xDist(GetRng());
    pos.y += yDist(GetRng());

    // instantiate prefab
    Termina::Actor* actor = Instantiate(prefab);
    if (!actor)
        return;

    actor->GetComponent<Termina::Transform>().SetPosition(pos);

    m_CurrentSpawned++;
}

void ProceduralGenerator::Serialize(nlohmann::json& out) const
{
    out["Spawn Interval"] = m_SpawnInterval;
    out["Max Spawned"] = m_MaxSpawned;
    out["Pool"] = nlohmann::json::array();
    for (const auto& p : m_Pool) out["Pool"].push_back(p.Path);
}

void ProceduralGenerator::Deserialize(const nlohmann::json& in)
{
    if (in.contains("Spawn Interval")) m_SpawnInterval = in["Spawn Interval"];
    if (in.contains("Max Spawned")) m_MaxSpawned = in["Max Spawned"];
    if (in.contains("Pool") && in["Pool"].is_array()) {
        m_Pool.clear();
        for (auto& v : in["Pool"]) {
            if (v.is_string()) m_Pool.emplace_back(v.get<std::string>());
        }
    }
}

void ProceduralGenerator::Inspect()
{
    ImGui::DragFloat("Spawn Interval", &m_SpawnInterval, 0.01f);
    ImGui::DragInt("Max Spawned", &m_MaxSpawned, 1);

    ImGui::Text("Pool:");
    for (size_t i = 0; i < m_Pool.size(); ++i) {
        char buf[64];
        sprintf(buf, "Prefab %zu", i);
        if (m_Pool[i].Inspect(buf)) {
            // path changed via inspector
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
}

void ProceduralGenerator::AddPrefab(const Prefab& prefab)
{
    if (prefab.IsValid()) m_Pool.push_back(prefab);
}

void ProceduralGenerator::ClearPool()
{
    m_Pool.clear();
}
