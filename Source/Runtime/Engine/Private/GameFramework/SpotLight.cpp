#include "PCH.hpp"

#include "GameFramework/SpotLight.hpp"

DEFINE_LOG_CATEGORY(LogSpotLight);

// ============================== [Public Constructors and Destructor] ============================== //

SpotLight::SpotLight()
: Super()
{
    m_canEverTick = false;

    if (ComponentsCreation())
    {
        SetRootComponent(m_component);
    }
}

// ============================== [Private Local Methods] ============================== //

bool    SpotLight::ComponentsCreation    ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_component = AddComponent<SpotLightComponent>("Spot Light Component")))
    {
        LOG(LogSpotLight, Error, "Spot Light Entity '%s' creation failed ! (AddComponent<SpotLightComponent>(\"Spot Light Component\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    
    return isComponentCreationHasSuccessed;
}