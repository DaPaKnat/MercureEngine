#include "PCH.hpp"

#include "Environment/Floor.hpp"

#include "Utility/CollisionChannelMacro.hpp"

DEFINE_LOG_CATEGORY(LogFloor);

// ============================== [Public Constructor] ============================== //

Floor::Floor    () : Super()
{
    m_canEverTick = false;

    if (CreateDefaultComponents())
    {
        InitializeProperties();
        InitializeHierarchy ();
        InitializeCollider  ();
        InitializeMesh      ();
    }
}

// ============================== [Private Local Methods] ============================== //

bool    Floor::CreateDefaultComponents  ()
{
    if (!(m_collider = AddComponent<BoxComponent>("Floor Collider")))
    {
        LOG(LogFloor, Error, "Floor entity '%s' creation failed ! (AddComponent<BoxComponent>(\"Floor Collider\") has returned nullptr.", GetName());
        return false;
    }

    if (!(m_mesh = AddComponent<StaticMeshComponent>("Floor Mesh")))
    {
        LOG(LogFloor, Error, "Floor entity '%s' creation failed ! (AddComponent<StaticMeshComponent>(\"Floor Mesh\") has returned nullptr.", GetName());
        return false;
    }
    
    return true;
}

void    Floor::InitializeCollider    ()
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

void    Floor::InitializeHierarchy   ()
{
    SetRootComponent(m_collider);
    m_mesh->SetupAttachment(m_collider);
}

void    Floor::InitializeMesh        ()
{
    // Set Default Collision Responses //
    m_mesh->SetSimulatePhysics(false);
    m_mesh->SetCollisionObjectType(ECC_IGNORED);
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);

    // Set Default Properties //
    m_mesh->SetModel("Default/Models/cube");
    m_mesh->SetMaterialInstance(0u, "Game/WhiteTiles/WhiteTiles_Instance");
}

void    Floor::InitializeProperties()
{
    SetEnableCollision(true);
}