#pragma once

#include <Termina/Asset/AssetSystem.hpp>
#include <Termina/Asset/IAssetLoader.hpp>
#include "AudioAsset.hpp"

namespace Termina {

    /// Loads audio from disk
    class AudioLoader : public IAssetLoader<AudioAsset>
    {
    public:
        AudioAsset* LoadFromDisk(const std::string& path) override;

        /// Not yet implemented — package loading will be added with the asset package system.
        AudioAsset* LoadFromPackage(const uint8* data, size_t size) override;

        /// Not yet implemented — package export will be added with the asset package system.
        std::vector<uint8> ExportToPackage(const AudioAsset* asset) override;
    };

} // namespace Termina
