#ifndef __VULKAN_SHADER_HPP__
#define __VULKAN_SHADER_HPP__

#include "Asset.hpp"

#include "Vulkan/Vulkan.hpp"

class ENGINE_API Shader : public Asset
{
    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Shader  () = delete;

        Shader  (std::string const&     p_name) : Asset(p_name) {};

        Shader  (std::string const&     p_name,
                 std::vector<uint32>&&  p_code);

        ~Shader () = default;

    // ============================== [Public Local Methods] ============================== //

        INLINE VkShaderModule const GetModule() const noexcept { return m_module; }

    private:

    // ============================== [Private Local Properties] ============================== //

        std::vector<uint32> m_code;

        VkShaderModule      m_module;

    // ============================== [Interface Private Local Methods] ============================== //

        void    Deserialize (std::string const& p_path) noexcept final override;

        void    Serialize   (std::string const& p_path) noexcept final override;

};  // !class Shader

#endif // !__VULKAN_SHADER_HPP__