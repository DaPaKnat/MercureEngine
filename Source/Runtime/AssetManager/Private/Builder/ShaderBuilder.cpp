#include "PCH.hpp"
#include "RHIAsset.hpp"

#include "Builder/ShaderBuilder.hpp"

// ============================== [Private Static Variables] ============================== //

std::array<std::string, 1> ShaderBuilder::SupportedExtensions = { ".glsl" };

// ============================== [Public Static Methods] ============================== //

bool                    ShaderBuilder::IsExtensionSupported (std::string const& p_extension) noexcept
{
    for (std::string const& supportedExtension : SupportedExtensions)
    {
        if (supportedExtension == p_extension)
            return true;
    }

    return false;
}

std::shared_ptr<Shader> ShaderBuilder::BuildFromFile        (std::string const& p_name,
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

    std::ifstream file(p_path);

    if (!file.is_open())
    {
        LOG(LogAssetManager, Error, "Failed to open file : ", p_path.c_str());
        return nullptr;
    }
    
    shaderc::Compiler       compiler;
    shaderc::CompileOptions options;
    shaderc_shader_kind     kind;
    
    extension = std::filesystem::path(p_path).filename().stem().extension().string();

    if (extension == ".vert")
        kind = shaderc_shader_kind::shaderc_vertex_shader;

    else if (extension == ".frag")
        kind = shaderc_shader_kind::shaderc_fragment_shader;

    else if (extension == ".geom")
        kind = shaderc_shader_kind::shaderc_geometry_shader;

    else if (extension == ".comp")
        kind = shaderc_shader_kind::shaderc_compute_shader;

    else
    {
        LOG(LogAssetManager, Error, "Extension not supported : ", extension.c_str());
        return nullptr;
    }

    std::string const content = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    
    shaderc::CompilationResult module = compiler.CompileGlslToSpv(content, kind, p_path.c_str(), options);
    
    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
    {
        LOG(LogRHI, Error, "Failed to compile shader : %s, %s", p_path.c_str(), module.GetErrorMessage().c_str());
        return nullptr;
    }

    return std::make_shared<Shader>(p_name, std::vector<uint32>(module.cbegin(), module.cend()));
}