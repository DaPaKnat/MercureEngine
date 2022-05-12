#include "PCH.hpp"
#include "AssetManager.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>

#include "Builder\TextureBuilder.hpp"

// ============================== [Private Static Variables] ============================== //

std::array<std::string, 8> TextureBuilder::SupportedExtensions = { ".jpg", ".png", ".tga", ".bmp", ".psd", ".gif", ".hdr", ".pic" };

// ============================== [Public Static Methods] ============================== //

bool                        TextureBuilder::IsExtensionSupported    (std::string const& p_extension) noexcept
{
    for (std::string const& supportedExtension : SupportedExtensions)
    {
        if (supportedExtension == p_extension)
            return true;
    }

    return false;
}

std::shared_ptr<Texture>    TextureBuilder::BuildFromFile           (std::string const& p_name,
                                                                     std::string const& p_path) noexcept
{
    if (p_name.empty() || p_path.empty())
        return nullptr;

    std::string extension(std::filesystem::path(p_path).extension().string());

    if (!IsExtensionSupported(extension))
    {
        LOG(LogAssetManager, Error, "Extension not supported : %s", extension.c_str());
        return nullptr;
    }

    int32 width, height, channels;

    if (stbi_uc* pixels = stbi_load(p_path.c_str(), &width, &height, &channels, STBI_rgb_alpha))
    {
        std::shared_ptr<Texture> texture = std::make_shared<Texture>(p_name, pixels, static_cast<uint32>(width), static_cast<uint32>(height));

        if (!std::filesystem::copy_file(p_path, p_name + ".asset", std::filesystem::copy_options::overwrite_existing))
            LOG(LogAssetManager, Error, "Failed to copy \"%s\" to asset directory", p_path.c_str());

        stbi_image_free(pixels);

        return texture;
    }

    else
        LOG(LogAssetManager, Error, "Failed to load texture : %s", p_path.c_str());

    return nullptr;
}