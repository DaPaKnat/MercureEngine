#include "PCH.hpp"

#include "Environment/Crate.hpp"

#include "Utility/CollisionChannelMacro.hpp"

// ============================== [Public Constructors and Destructor] ============================== //

Crate::Crate  ()
: Super()
{
    m_canEverTick = false;

    bool isComponentCreationHasSuccessed    {ComponentsCreation()};
    
    if (isComponentCreationHasSuccessed)
    {
        InitializeProperties();
        InitializeHierarchy();
        InitializeCollider();
        InitializeMesh();
    }

    SetWorldScale(Vector3(0.02f, 0.02f, 0.02f));
}

// ============================== [Private Local Methods] ============================== //

bool    Crate::ComponentsCreation    ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_collider = AddComponent<BoxComponent>("Crate Collider")))
        isComponentCreationHasSuccessed = false;
    

    if (!(m_mesh = AddComponent<StaticMeshComponent>("Crate Mesh")))
        isComponentCreationHasSuccessed = false;
    
    return isComponentCreationHasSuccessed;
}

void    Crate::InitializeCollider    ()
{
    // Set Default Collision Responses //
    m_collider->SetCollisionObjectType(ECC_BLOCK_ALL);
    m_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    m_collider->SetCollisionResponseToAllChannels(ECollisionResponse::Block);
    m_collider->SetCollisionResponseToChannel(ECC_BLOCK_ALL, ECollisionResponse::Ignore);
    m_collider->SetCollisionResponseToChannel(ECC_IGNORED, ECollisionResponse::Ignore);

    // Set Default Properties //
    m_collider->SetSimulatePhysics(false);
    m_collider->SetEnableGravity(false);
    m_collider->SetBoxExtent(0.5f, false);
}

void    Crate::InitializeHierarchy   ()
{
    SetRootComponent(m_collider);
    m_mesh->SetupAttachment(m_collider);
}

void    Crate::InitializeMesh        ()
{
    // Set Default Collision Responses //
    m_mesh->SetSimulatePhysics(false);
    m_mesh->SetCollisionObjectType(ECC_IGNORED);
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);

    // Set Default Properties //
    m_mesh->SetModel("Game/Crate/Crate");
    m_mesh->SetMaterialInstance(0u, "Game/Crate/Crate_Instance");
}

void    Crate::InitializeProperties()
{
    SetEnableCollision(true);
}