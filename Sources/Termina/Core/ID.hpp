#pragma once

#include "Common.hpp"

#include <unordered_set>
#include <random>

namespace Termina {

    /// A simple ID generator that generates unique 64-bit IDs.
    class IDGenerator
    {
    public:
        static IDGenerator& Get();

        /// Generates a new unique ID.
        uint64 Generate();

        /// Reserves an ID for future use, preventing it from being generated again.
        void Reserve(uint64 id);
        /// Releases an ID, allowing it to be generated again.
        void Release(uint64 id);
        /// Returns `true` if the ID is currently in use.
        bool IsUsed(uint64 id) const;
        /// Clears all reserved IDs, allowing all IDs to be generated again.
        void Clear();

    private:
        IDGenerator();

        std::unordered_set<uint64> m_UsedIDs;
        std::mt19937_64 m_Generator;
        std::uniform_int_distribution<uint64> m_Distribution;
    };

    inline uint64 NewID()
    {
        return IDGenerator::Get().Generate();
    }
}
