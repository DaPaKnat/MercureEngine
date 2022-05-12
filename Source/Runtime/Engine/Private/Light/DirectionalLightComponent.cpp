#include "PCH.hpp"

#include "Light/DirectionalLightComponent.hpp"

// ============================== [Public Constructor] ============================== //

DirectionalLightComponent::DirectionalLightComponent    () : LightComponent(),
    m_size  { 100.0f },
    m_range { 100.0f }
{

}

// ============================== [Public Local Methods] ============================== //

void    DirectionalLightComponent::Deserialize  (Json const& p_deserialize,
                                                 Json const& p_components)
{
    *this = p_deserialize;
}

void    DirectionalLightComponent::Serialize    (Json& p_serialize) const
{
    p_serialize = Json(*this);
}

// ==================================================================================== //

DirectionalLightData    DirectionalLightComponent::GetRenderData    () const noexcept
{
    return DirectionalLightData
    {
        GetWorldTransform().GetMatrixInverse(),                             // Light's view matrix
        Matrix4x4::Ortho(-m_size, m_size, -m_size, m_size, 0.1f, m_range),  // Light's projection matrix
        m_color,                                                            // Light's color
        GetForward(),                                                       // Light's direction
        m_intensity                                                         // Light's intensity
    };
}