#ifndef __POINT_LIGHT_COMPONENT_HPP__
#define __POINT_LIGHT_COMPONENT_HPP__

#include "LightComponent.hpp"

struct MS_ALIGN(16) PointLightData
{
    Matrix4x4 views[6];
    Matrix4x4 projection;
    Color     color;
    Vector3   position;
    float     intensity;
};

class ENGINE_API PointLightComponent : public LightComponent
{
    REFLECT(PointLightComponent)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        PointLightComponent ();

        PointLightComponent (PointLightComponent const&)    = default;

        PointLightComponent (PointLightComponent&&)         = default;

        ~PointLightComponent()                              = default;

    // ============================== [Public Local Methods] ============================== //

        void    Deserialize (Json const&    p_deserialize,
                             Json const&    p_components)   override;

        void    Serialize   (Json&          p_serialize)    const override;

    // ==================================================================================== //

        PointLightData GetRenderData() const noexcept;

    // ============================== [Public Local Operators] ============================== //

        PointLightComponent&    operator=   (PointLightComponent const&)    = default;

        PointLightComponent&    operator=   (PointLightComponent&&)         = default;

    private:

};  // !class PointLightComponent

#include "PointLightComponent.generated.hpp"

#endif // !__POINT_LIGHT_COMPONENT_HPP__