#ifndef __LIGHT_COMPONENT_HPP__
#define __LIGHT_COMPONENT_HPP__

#include "Components/SceneComponent.hpp"

class ENGINE_API BASE LightComponent : public SceneComponent
{
    REFLECT(LightComponent)

    public:

    // ============================== [Public Constructors and Destructor] ============================== //

        LightComponent  ();

        LightComponent  (LightComponent const&) = default;

        LightComponent  (LightComponent&&)      = default;

        ~LightComponent ()                      = default;

    // ============================== [Public Local Operators] ============================== //

        LightComponent& operator=   (LightComponent const&) = default;

        LightComponent& operator=   (LightComponent&&)      = default;

    protected:

    // ============================== [Private Local Properties] ============================== //

        PROPERTY()
        Color   m_color;

        PROPERTY()
        float   m_intensity;

};  // !class LightComponent

#include "LightComponent.generated.hpp"

#endif // !__LIGHT_COMPONENT_HPP__