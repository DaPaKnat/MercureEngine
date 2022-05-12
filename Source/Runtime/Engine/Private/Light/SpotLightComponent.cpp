#include "PCH.hpp"

#include "Light/SpotLightComponent.hpp"

// ============================== [Public Constructor] ============================== //

SpotLightComponent::SpotLightComponent  () : LightComponent(),
    m_angle { 100.0f }
{

}

// ============================== [Public Local Methods] ============================== //

void    SpotLightComponent::Deserialize (Json const& p_deserialize,
                                         Json const& p_components)
{
    *this = p_deserialize;

    LightComponent::Deserialize(p_deserialize, p_components);
}

void    SpotLightComponent::Serialize   (Json& p_serialize) const
{
    p_serialize = Json(*this);

    SceneComponent::SerializeHierarchy(p_serialize);
}

// ==================================================================================== //

SpotLightData   SpotLightComponent::GetRenderData   () const noexcept
{
    return SpotLightData
    {
        GetWorldTransform().GetMatrixInverse(),                                             // Light's view matrix
        Matrix4x4::Perspective(Math::DegToRad(m_angle * 2.0f), 1.0f, 0.1f, m_intensity),    // Light's projection matrix
        m_color,                                                                            // Light's color
        GetWorldLocation(),                                                                 // Light's position
        m_intensity,                                                                        // Light's intensity
        GetForward(),                                                                       // Light's direction
        Math::Cos(Math::DegToRad(m_angle - 5.0f)),                                          // Light's cut-off angle
        Math::Cos(Math::DegToRad(m_angle))                                                  // Light's outer cut-off angle
    };
}