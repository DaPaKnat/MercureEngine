#include "PCH.hpp"

#include "GameFramework/DirectionalLight.hpp"

DEFINE_LOG_CATEGORY(LogDirectionalLight);

// ============================== [Public Constructors and Destructor] ============================== //

DirectionalLight::DirectionalLight  ()
: Super()
{
    m_canEverTick = false;

    if (ComponentsCreation())
    {
        SetRootComponent(m_component);
    }
}

// ============================== [Private Local Methods] ============================== //

bool    DirectionalLight::ComponentsCreation    ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_component = AddComponent<DirectionalLightComponent>("Directional Light Component")))
    {
        LOG(LogDirectionalLight, Error, "Directional Light Entity '%s' creation failed ! (AddComponent<DirectionalLightComponent>(\"Directional Light Component\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    
    return isComponentCreationHasSuccessed;
}