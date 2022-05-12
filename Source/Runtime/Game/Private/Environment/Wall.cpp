#include "PCH.hpp"

#include "Environment/Wall.hpp"

#include "Utility/CollisionChannelMacro.hpp"

DEFINE_LOG_CATEGORY(LogWall);

// ============================== [Public Constructors and Destructor] ============================== //

Wall::Wall  ()
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
}

// ============================== [Private Local Methods] ============================== //

bool    Wall::ComponentsCreation    ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_collider = AddComponent<BoxComponent>("Wall Collider")))
    {
        LOG(LogWall, Error, "Wall entity '%s' creation failed ! (AddComponent<BoxComponent>(\"Wall Collider\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    

    if (!(m_mesh = AddComponent<StaticMeshComponent>("Wall Mesh")))
    {
        LOG(LogWall, Error, "Wall entity '%s' creation failed ! (AddComponent<StaticMeshComponent>(\"Wall Mesh\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    
    return isComponentCreationHasSuccessed;
}

void    Wall::InitializeCollider    ()
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

void    Wall::InitializeHierarchy   ()
{
    SetRootComponent(m_collider);
    m_mesh->SetupAttachment(m_collider);
}

void    Wall::InitializeMesh        ()
{
    // Set Default Collision Responses //
    m_mesh->SetSimulatePhysics(false);
    m_mesh->SetCollisionObjectType(ECC_IGNORED);
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);

    // Set Default Properties //
    m_mesh->SetModel("Default/Models/cube");
    m_mesh->SetMaterialInstance(0u, "Game/MetalPlate/MetalPlate_Instance");
}

void    Wall::InitializeProperties()
{
    SetEnableCollision(true);
}