#include "PCH.hpp"
#include "RHI.hpp"

#include "Vulkan/Asset/Shader/Shader.hpp"

// ============================== [Public Constructor and Destructor] ============================== //

Shader::Shader  (std::string const&     p_name,
                 std::vector<uint32>&&  p_code) : Asset(p_name),
    m_code { std::move(p_code) }
{
    m_isPending.store(true, std::memory_order_relaxed);

    VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

    VkShaderModuleCreateInfo moduleCI = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };

    moduleCI.codeSize = m_code.size() * sizeof(uint32);
    moduleCI.pCode    = m_code.data();

    VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCI, nullptr, &m_module));

    Debug::SetShaderModuleName(device, m_module, m_name.c_str());

    m_isLoaded .store(true,  std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}

// ============================== [Interface Private Local Methods] ============================== //

void    Shader::Deserialize (std::string const& p_path) noexcept
{
    std::ifstream asset(p_path, std::ios::in | std::ios::ate | std::ios::binary);

    if (asset.is_open())
    {
        size_t size = static_cast<size_t>(asset.tellg());

        asset.seekg(0);

        std::string header;

        std::getline(asset, header);

        if (header == Reflect::GetEnumName(EAssetType::SHADER))
        {
            VkDevice const device = RHI::Get().GetDevice()->GetLogicalDevice();

            m_code.resize((size - header.size() - 1) / sizeof(uint32));

            asset.read(reinterpret_cast<ANSICHAR*>(m_code.data()), m_code.size() * sizeof(uint32));

            VkShaderModuleCreateInfo moduleCI = { VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };

            moduleCI.codeSize = m_code.size() * sizeof(uint32);
            moduleCI.pCode    = m_code.data();

            VK_CHECK_RESULT(vkCreateShaderModule(device, &moduleCI, nullptr, &m_module));

            Debug::SetShaderModuleName(device, m_module, m_name.c_str());

            m_isLoaded.store(true,  std::memory_order_release);
        }

        else
            LOG(LogAssetManager, Error, "Shader file corrupted : %s", p_path.c_str());
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for deserialization", p_path.c_str());

    m_isPending.store(false, std::memory_order_release);
}

void    Shader::Serialize   (std::string const& p_path) noexcept
{
    std::ofstream asset(p_path, std::ios::out | std::ios::binary);

    if (asset.is_open())
    {
        // Inserts asset type as header in the file.
        asset << EAssetType::SHADER << '\n';

        asset.write(reinterpret_cast<ANSICHAR*>(m_code.data()), sizeof(uint32) * m_code.size());
    }

    else
        LOG(LogAssetManager, Error, "Failed to open \"%s\" for serialization", p_path.c_str());

    m_code.clear();

    vkDestroyShaderModule(RHI::Get().GetDevice()->GetLogicalDevice(), m_module, nullptr);

    m_isLoaded .store(false, std::memory_order_release);
    m_isPending.store(false, std::memory_order_release);
}