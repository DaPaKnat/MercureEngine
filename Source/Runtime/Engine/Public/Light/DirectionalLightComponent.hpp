#ifndef __DIRECTIONAL_LIGHT_COMPONENT_HPP__
#define __DIRECTIONAL_LIGHT_COMPONENT_HPP__

#include "LightComponent.hpp"

struct MS_ALIGN(16) DirectionalLightData
{
    Matrix4x4 view;
    Matrix4x4 projection;
    Color     color;
    Vector3   direction;
    float     intensity;
};

class ENGINE_API DirectionalLightComponent : public LightComponent
{
    REFLECT(DirectionalLightComponent)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        DirectionalLightComponent   ();

        DirectionalLightComponent   (DirectionalLightComponent const&)  = default;

        DirectionalLightComponent   (DirectionalLightComponent&&)       = default;

        ~DirectionalLightComponent  ()                                  = default;

    // ============================== [Public Local Methods] ============================== //

        void    Deserialize (Json const&    p_deserialize,
                             Json const&    p_components)   override;

        void    Serialize   (Json&          p_serialize)    const override;

    // ==================================================================================== //

        DirectionalLightData GetRenderData() const noexcept;

    // ============================== [Public Local Operators] ============================== //

        DirectionalLightComponent&  operator=   (DirectionalLightComponent const&)  = default;

        DirectionalLightComponent&  operator=   (DirectionalLightComponent&&)       = default;

    private:

    // ============================== [Private Local Properties] ============================== //

        PROPERTY()
        float   m_size;
        
        PROPERTY()
        float   m_range;

};  // !class DirectionalLightComponent

#include "DirectionalLightComponent.generated.hpp"

#endif // !__DIRECTIONAL_LIGHT_COMPONENT_HPP__