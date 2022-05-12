#ifndef __SPOT_LIGHT_COMPONENT_HPP__
#define __SPOT_LIGHT_COMPONENT_HPP__

#include "LightComponent.hpp"

struct MS_ALIGN(16) SpotLightData
{
    Matrix4x4 view;
    Matrix4x4 projection;
    Color     color;
    Vector3   position;
    float     intensity;
    Vector3   direction;
    float     cutOff;
    float     outerCutOff;
};

class ENGINE_API SpotLightComponent : public LightComponent
{
    REFLECT(SpotLightComponent)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        SpotLightComponent  ();

        SpotLightComponent  (SpotLightComponent const&) = default;

        SpotLightComponent  (SpotLightComponent&&)      = default;

        ~SpotLightComponent ()                          = default;

    // ============================== [Public Local Methods] ============================== //

        void    Deserialize (Json const&    p_deserialize,
                             Json const&    p_components)   override;

        void    Serialize   (Json&          p_serialize)    const override;

    // ==================================================================================== //

        SpotLightData GetRenderData() const noexcept;

    // ============================== [Public Local Operators] ============================== //

        SpotLightComponent& operator=   (SpotLightComponent const&) = default;

        SpotLightComponent& operator=   (SpotLightComponent&&)      = default;

    private:

    // ============================== [Private Local Properties] ============================== //

        PROPERTY()
        float   m_angle;

};  // !class SpotLightComponent

#include "SpotLightComponent.generated.hpp"

#endif // !__SPOT_LIGHT_COMPONENT_HPP__