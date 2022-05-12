#include "PCH.hpp"

#include "Light/LightComponent.hpp"

// ============================== [Public Constructor] ============================== //

LightComponent::LightComponent  () : SceneComponent(),
    m_color     { Color::White },
    m_intensity { 50.0f }
{

}