#ifndef __VULKAN_MATERIAL_INSTANCE_HPP__
#define __VULKAN_MATERIAL_INSTANCE_HPP__

#include "Material.hpp"

class ENGINE_API MaterialInstance : public Asset
{
    REFLECT(MaterialInstance)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        MaterialInstance    () = delete;

        MaterialInstance    (std::string               const&   p_name) : Asset(p_name) {};

        MaterialInstance    (std::string               const&   p_name,
                             std::shared_ptr<Material> const&   p_material);

        MaterialInstance    (std::string               const&   p_name,
                             std::shared_ptr<Material>&&        p_material);

        ~MaterialInstance   () = default;

    // ============================== [Public Local Methods] ============================== //

        void    SetAlbedo   (Color const&   p_albedo)   noexcept;

        void    SetAlbedo   (Color&&        p_albedo)   noexcept;

        void    SetMetallic (float          p_value)    noexcept;

        void    SetRoughness(float          p_value)    noexcept;

        void    SetAO       (float          p_value)    noexcept;

    // ==================================================================================== //

        INLINE bool                             IsOpaque                ()  const noexcept  { return m_material->IsOpaque(); }

        INLINE std::shared_ptr<Material> const& GetMaterial             ()  const noexcept  { return m_material; }

        INLINE MaterialData              const& GetMaterialData         ()  const noexcept  { return m_data; }

        INLINE MaterialRenderData        const& GetMaterialRenderData   ()  const noexcept  { return m_material->GetMaterialRenderData(); }

        INLINE MaterialData              const* GetMaterialDataPtr      ()  const noexcept  { return &m_data; }

        INLINE MaterialRenderData        const* GetMaterialRenderDataPtr()  const noexcept  { return m_material->GetMaterialRenderDataPtr(); }

    private:

    // ============================== [Private Local Properties] ============================== //

        std::shared_ptr<Material>   m_material;

        PROPERTY()
        MaterialData                m_data;

    // ============================== [Interface Private Local Methods] ============================== //

        void    Deserialize (std::string const& p_path) noexcept override;

        void    Serialize   (std::string const& p_path) noexcept override;

};  // !class MaterialInstance

#include "MaterialInstance.generated.hpp"

#endif // !__VULKAN_MATERIAL_INSTANCE_HPP__