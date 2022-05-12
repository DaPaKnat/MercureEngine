#include "PCH.hpp"

#include "Light/PointLightComponent.hpp"

// ============================== [Public Constructor] ============================== //

PointLightComponent::PointLightComponent    () : LightComponent()
{

}

// ============================== [Public Local Methods] ============================== //

void    PointLightComponent::Deserialize    (Json const& p_deserialize,
                                             Json const& p_components)
{
    *this = p_deserialize;

    LightComponent::Deserialize(p_deserialize, p_components);
}

void    PointLightComponent::Serialize      (Json& p_serialize) const
{
    p_serialize = Json(*this);

    SceneComponent::SerializeHierarchy(p_serialize);
}

// ==================================================================================== //

PointLightData  PointLightComponent::GetRenderData  () const noexcept
{
    return PointLightData
    {
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Right,    Vector3::Up),       // Light's right    view matrix
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Left,     Vector3::Up),       // Light's left     view matrix
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Up,       Vector3::Backward), // Light's top      view matrix
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Down,     Vector3::Forward),  // Light's down     view matrix
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Forward,  Vector3::Up),       // Light's forward  view matrix
        Matrix4x4::View(GetWorldLocation(), GetWorldLocation() + Vector3::Backward, Vector3::Up),       // Light's backward view matrix
        Matrix4x4::Perspective(Math::DegToRad(90.0f), 1.0f, 0.1f, m_intensity),                         // Light's projection matrix
        m_color,                                                                                        // Light's color
        GetWorldLocation(),                                                                             // Light's direction
        m_intensity                                                                                     // Light's intensity
    };
}