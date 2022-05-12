#include "PCH.hpp"

#include "GameFramework/PointLight.hpp"

DEFINE_LOG_CATEGORY(LogPointLight);

// ============================== [Public Constructors and Destructor] ============================== //

PointLight::PointLight()
: Super()
{
    m_canEverTick = false;

    if (ComponentsCreation())
    {
        SetRootComponent(m_component);
    }
}

// ============================== [Private Local Methods] ============================== //

bool    PointLight::ComponentsCreation  ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_component = AddComponent<PointLightComponent>("Point Light Component")))
    {
        LOG(LogPointLight, Error, "Point Light Entity '%s' creation failed ! (AddComponent<PointLightComponent>(\"Point Light Component\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    
    return isComponentCreationHasSuccessed;
}