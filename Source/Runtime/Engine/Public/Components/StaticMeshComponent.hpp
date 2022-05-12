#ifndef __STATIC_MESH_COMPONENT_HPP__
#define __STATIC_MESH_COMPONENT_HPP__

#include "Components/PrimitiveComponent.hpp"

class Model;
class MaterialInstance;

class ENGINE_API StaticMeshComponent : public PrimitiveComponent
{
    REFLECT(StaticMeshComponent)

    public:

    // ============================== [Public Constructor and Destructor] ============================== //

        StaticMeshComponent ()                                      = default;

        StaticMeshComponent (StaticMeshComponent const& p_other)    = default;

        StaticMeshComponent (StaticMeshComponent&&      p_other)    = default;

        ~StaticMeshComponent()                                      = default;

    // ============================== [Public Local Methods] ============================== //

        void    SetModel            (ANSICHAR const*                            p_name)             noexcept;

        void    SetModel            (std::shared_ptr<Model> const&              p_model)            noexcept;

        void    SetMaterialInstance (uint32                                     p_index,
                                     ANSICHAR const*                            p_name)             noexcept;

        void    SetMaterialInstance (uint32                                     p_index,
                                     std::shared_ptr<MaterialInstance> const&   p_materialInstance) noexcept;

    // ==================================================================================== //

        INLINE std::shared_ptr<Model>                         const&    GetModel                ()                      const noexcept  { return m_model; }

        INLINE std::shared_ptr<MaterialInstance>&                       GetMaterialInstance     (size_t p_index = 0)    noexcept        { return m_materialInstances[p_index]; }

        INLINE std::vector<std::shared_ptr<MaterialInstance>> const&    GetMaterialInstances    ()                      const noexcept  { return m_materialInstances; }
        
    // ==================================================================================== //

        virtual void    Serialize   (Json&          p_serialize)    const override;

        virtual void    Deserialize (Json const&    p_deserialize,
                                     Json const&    p_components)  override;

    // ==================================================================================== //

        StaticMeshComponent&    operator=   (StaticMeshComponent const& p_copy) = default;

        StaticMeshComponent&    operator=   (StaticMeshComponent&&      p_move) = default;

    private:

    // ============================== [Private Local Properties] ============================== //

        PROPERTY()
        std::shared_ptr<Model>                          m_model;

        PROPERTY()
        std::vector<std::shared_ptr<MaterialInstance>>  m_materialInstances;

};  // !class StaticMeshComponent

#include "StaticMeshComponent.generated.hpp"

#endif // !__STATIC_MESH_COMPONENT_HPP__