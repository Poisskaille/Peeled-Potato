#include "ID.hpp"

namespace Termina {
    IDGenerator& IDGenerator::Get()
    {
        static IDGenerator instance;
        return instance;
    }

    IDGenerator::IDGenerator()
        : m_Generator(std::random_device{}())
        , m_Distribution(1, std::numeric_limits<uint64>::max())
    {
    }
    
    uint64 IDGenerator::Generate()
    {
        uint64 id;
        do
        {
            id = m_Distribution(m_Generator);
        } while (IsUsed(id));
    
        Reserve(id);
        return id;
    }
    
    void IDGenerator::Reserve(uint64 id)
    {
        m_UsedIDs.insert(id);
    }
    
    void IDGenerator::Release(uint64 id)
    {
        m_UsedIDs.erase(id);
    }
    
    bool IDGenerator::IsUsed(uint64 id) const
    {
        return m_UsedIDs.find(id) != m_UsedIDs.end();
    }
    
    void IDGenerator::Clear()
    {
        m_UsedIDs.clear();
    }
}
