#include "AudioLoader.hpp"
#include "Audio/AudioSystem.hpp"
#include "Core/Logger.hpp"

#include <Termina/Core/Application.hpp>
#include <Termina/Renderer/Renderer.hpp>

namespace Termina {
    AudioAsset* AudioLoader::LoadFromDisk(const std::string& path)
    {
        AudioSystem* system = Application::GetSystem<AudioSystem>();
        return new AudioAsset(new AudioData(system->GetEngine(), path));
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
