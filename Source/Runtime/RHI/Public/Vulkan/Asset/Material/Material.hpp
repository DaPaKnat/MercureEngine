#ifndef __VULKAN_MATERIAL_HPP__
#define __VULKAN_MATERIAL_HPP__

#include "Asset.hpp"

// ============================== [Forward Declaration] ============================== //

class Texture;

// ============================== [Data Structures] ============================== //

struct MaterialCreateInfo
{
    Color albedo;
    float metallic;
    float roughness;
    float ao;

    std::string albedoMapName;
    std::string normalMapName;
    std::string metallicMapName;
    std::string roughnessMapName;
    std::string aoMapName;

};  // !struct MaterialCreateInfo

struct MS_ALIGN(4) MaterialData
{
    PROPERTY()
    Color albedo;
    PROPERTY()
    float metallic;
    PROPERTY()
    float roughness;
    PROPERTY()
    float ao;

    bool operator==(MaterialData const& p_other) const
    {
        return albedo    == p_other.albedo    &&
               metallic  == p_other.metallic  &&
               roughness == p_other.roughness &&
               ao        == p_other.ao;
    }

    bool operator!=(MaterialData const& p_other) const
    {
        return albedo    != p_other.albedo    ||
               metallic  != p_other.metallic  ||
               roughness != p_other.roughness ||
               ao        != p_other.ao;
    }

};  // !struct MaterialData

struct MaterialRenderData
{
    VkDescriptorPool      descriptorPool      = VK_NULL_HANDLE;
    VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
    VkDescriptorSet       descriptorSet       = VK_NULL_HANDLE;
    VkPipelineLayout      pipelineLayout      = VK_NULL_HANDLE;
    VkPipeline            pipeline            = VK_NULL_HANDLE;

    bool operator==(MaterialRenderData const& p_other) const
    {
        return descriptorPool      == p_other.descriptorPool      &&
               descriptorSetLayout == p_other.descriptorSetLayout &&
               descriptorSet       == p_other.descriptorSet       &&
               pipelineLayout      == p_other.pipelineLayout      &&
               pipeline            == p_other.pipeline;
    }

    bool operator!=(MaterialRenderData const& p_other) const
    {
        return descriptorPool      != p_other.descriptorPool      ||
               descriptorSetLayout != p_other.descriptorSetLayout ||
               descriptorSet       != p_other.descriptorSet       ||
               pipelineLayout      != p_other.pipelineLayout      ||
               pipeline            != p_other.pipeline;
    }

};  // !struct MaterialRenderData

// =============================================================================== //

class ENGINE_API Material : public Asset
{
    REFLECT(Material)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        Material    () = delete;

        Material    (std::string const&     p_name) : Asset(p_name) {};

        Material    (std::string const&     p_name,
                     MaterialCreateInfo&&   p_data);

        ~Material   () = default;

    // ============================== [Public Local Methods] ============================== //

        INLINE bool                         IsOpaque                ()  const noexcept  { return m_data.albedo.m_a == 1.0f; }

        INLINE MaterialData       const&    GetMaterialData         ()  const noexcept  { return m_data; }

        INLINE MaterialRenderData const&    GetMaterialRenderData   ()  const noexcept  { return m_renderData; }

        INLINE MaterialData       const*    GetMaterialDataPtr      ()  const noexcept  { return &m_data; }

        INLINE MaterialRenderData const*    GetMaterialRenderDataPtr()  const noexcept  { return &m_renderData; }

    private:

    // ============================== [Private Local Properties] ============================== //

        /**
         * Index 0 : Albedo
         * Index 1 : Normal
         * Index 2 : Metallic
         * Index 3 : Roughness
         * Index 4 : AO
         */
        std::array<std::shared_ptr<Texture>, 5> m_textures;

        PROPERTY()
        MaterialData                            m_data;

        MaterialRenderData                      m_renderData;

    // ============================== [Interface Private Local Methods] ============================== //

        void    Deserialize (std::string const& p_path) noexcept override;

        void    Serialize   (std::string const& p_path) noexcept override;

    // ============================== [Private Local Methods] ============================== //

        void    SetupDescriptorPool         () noexcept;

        void    SetupDescriptorSetLayout    () noexcept;

        void    SetupDescriptorSet          () noexcept;

        void    SetupPipelineLayout         () noexcept;

        void    SetupPipeline               () noexcept;

};  // !class Material

#include "Material.generated.hpp"

#endif // !__VULKAN_MATERIAL_HPP__