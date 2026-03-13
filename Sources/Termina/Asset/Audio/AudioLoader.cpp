#include "AudioLoader.hpp"
#include "Audio/AudioSystem.hpp"
#include "Core/Logger.hpp"

#include <Termina/Core/Application.hpp>
#include <Termina/Renderer/Renderer.hpp>

namespace Termina {
    AudioAsset* AudioLoader::LoadFromDisk(const std::string& path)
    {
        AudioSystem* system = Application::GetSystem<AudioSystem>();
        AudioData* data = new AudioData(system->GetEngine(), path);
        if (!data->IsValid())
        {
            delete data;
            return nullptr;
        }
        return new AudioAsset(data);
    }

    AudioAsset* AudioLoader::LoadFromPackage(const uint8* /*data*/, size_t /*size*/)
    {
        return nullptr;
    }

    std::vector<uint8> AudioLoader::ExportToPackage(const AudioAsset* /*asset*/)
    {
        return {};
    }
} // namespace Termina
