#include <Termina/World/ComponentRegistry.hpp>
#include <Termina/Core/Logger.hpp>

#include <algorithm>

namespace Termina {
    ComponentRegistry& ComponentRegistry::Get()
    {
        static ComponentRegistry instance;
        return instance;
    }

    bool ComponentRegistry::RegisterScripted(const std::string& name, std::function<Component*(Actor*)> factory)
    {
        if (!factory) return false;
        std::lock_guard<std::mutex> lock(m_Mutex);
        if (m_NameToType.count(name) || m_ScriptedByName.count(name)) return false;
        m_ScriptedByName.emplace(name, std::move(factory));
        return true;
    }

    Component* ComponentRegistry::CreateByType(std::type_index type, Actor* owner) const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_ByType.find(type);
        if (it == m_ByType.end()) return nullptr;
        return it->second.Factory(owner);
    }

    Component* ComponentRegistry::CreateByName(const std::string& name, Actor* owner) const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto nameIt = m_NameToType.find(name);
        if (nameIt != m_NameToType.end()) {
            auto typeIt = m_ByType.find(nameIt->second);
            if (typeIt != m_ByType.end())
                return typeIt->second.Factory(owner);
        }
        auto scriptIt = m_ScriptedByName.find(name);
        if (scriptIt != m_ScriptedByName.end())
            return scriptIt->second(owner);
        return nullptr;
    }

    bool ComponentRegistry::IsRegistered(std::type_index type) const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_ByType.count(type) > 0;
    }

    bool ComponentRegistry::IsRegisteredByName(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_NameToType.count(name) || m_ScriptedByName.count(name);
    }

    std::string ComponentRegistry::GetNameForType(std::type_index type) const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto it = m_ByType.find(type);
        if (it == m_ByType.end()) return {};
        return it->second.Name;
    }

    bool ComponentRegistry::UnregisterByName(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        auto nameIt = m_NameToType.find(name);
        if (nameIt != m_NameToType.end()) {
            std::type_index idx = nameIt->second;
            m_ByType.erase(idx);
            m_Order.erase(std::remove(m_Order.begin(), m_Order.end(), idx), m_Order.end());
            m_NameToType.erase(nameIt);
            return true;
        }
        auto scriptIt = m_ScriptedByName.find(name);
        if (scriptIt != m_ScriptedByName.end()) {
            m_ScriptedByName.erase(scriptIt);
            return true;
        }
        return false;
    }

    size_t ComponentRegistry::Count() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        return m_ByType.size() + m_ScriptedByName.size();
    }

    void ComponentRegistry::Clear()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_ByType.clear();
        m_NameToType.clear();
        m_Order.clear();
        m_ScriptedByName.clear();
    }

    void ComponentRegistry::Report() const
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        for (const auto& [name, type] : m_NameToType)
        TN_INFO("Registered component: %s (type: %s)", name.c_str(), type.name());
        for (const auto& [name, _] : m_ScriptedByName)
        TN_INFO("Registered scripted component: %s", name.c_str());
    }
} // namespace Termina
