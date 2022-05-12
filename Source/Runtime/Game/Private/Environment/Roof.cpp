#include "PCH.hpp"

#include "Environment/Roof.hpp"

#include "Utility/CollisionChannelMacro.hpp"

DEFINE_LOG_CATEGORY(LogRoof);

// ============================== [Public Constructor] ============================== //

Roof::Roof  () : Super()
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

bool    Roof::CreateDefaultComponents   ()
{
    if (!(m_collider = AddComponent<BoxComponent>("Roof Collider")))
    {
        LOG(LogRoof, Error, "Roof entity '%s' creation failed ! (AddComponent<BoxComponent>(\"Roof Collider\") has returned nullptr.", GetName());
        return false;
    }

    if (!(m_mesh = AddComponent<StaticMeshComponent>("Roof Mesh")))
    {
        LOG(LogRoof, Error, "Roof entity '%s' creation failed ! (AddComponent<StaticMeshComponent>(\"Roof Mesh\") has returned nullptr.", GetName());
        return false;
    }
    
    return true;
}

void    Roof::InitializeCollider        ()
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

void    Roof::InitializeHierarchy       ()
{
    SetRootComponent(m_collider);
    m_mesh->SetupAttachment(m_collider);
}

void    Roof::InitializeMesh            ()
{
    // Set Default Collision Responses //
    m_mesh->SetSimulatePhysics(false);
    m_mesh->SetCollisionObjectType(ECC_IGNORED);
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);

    // Set Default Properties //
    m_mesh->SetModel("Default/Models/cube");
    m_mesh->SetMaterialInstance(0u, "Game/ConcretePanel/ConcretePanel_Instance");
}

void    Roof::InitializeProperties      ()
{
    SetEnableCollision(true);
}