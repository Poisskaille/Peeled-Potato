#pragma once

#include <Termina/Audio/AudioData.hpp>
#include <Termina/Core/IInspectable.hpp>

namespace Termina {

    /// An asset holding sound data
    class AudioAsset : public IInspectable
    {
    public:
        explicit AudioAsset(AudioData* data)
            : m_Data(data)
        {
        }

        ~AudioAsset()
        {
            delete m_Data;
        }

        AudioData* GetData() const { return m_Data; }

        void Inspect() override {}

    private:
        AudioData* m_Data = nullptr;
    };

} // namespace Termina
