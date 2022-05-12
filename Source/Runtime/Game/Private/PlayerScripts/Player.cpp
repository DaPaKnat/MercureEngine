#include "PCH.hpp"

#include "Application.hpp"

#include "PlayerScripts/Player.hpp"
#include "PlayerScripts/PlayerMovementComponent.hpp"
#include "PlayerScripts/PlayerController.hpp"

#include "Utility/CollisionChannelMacro.hpp"

#include "World.hpp"

DEFINE_LOG_CATEGORY(LogPlayer);

// ==============================[Public Local Constructor]============================== //

Player::Player  ()
{
    bool isComponentCreationHasSuccessed    {ComponentsCreation()};
    m_canEverTick = true;

    if (isComponentCreationHasSuccessed)
    {
        SetEnableCollision(true);
        InitializeCapsuleCollider();
        InitializeRifleMesh();
        InitializeRifleMuzzle();
        InitializePlayerCamera();
    }

    else
    {
        m_canEverTick = false;
    }
}

// ==============================[Virtual Public Local Methods]============================== //

void    Player::BeginPlay   ()
{
    Super::BeginPlay();

    World* myWorld  {GetWorld()};

    if (myWorld != nullptr)
    {
        myWorld->SetMainCamera(m_playerCamera);
    }

    else
    {
        LOG(LogPlayer, Error, "The world of %s is nullptr !", GetName());
    }
}

// ==============================[Public Local Methods]============================== //

void                Player::AddMovementInput        (Vector3 const& p_worldDirection,
                                                     float const    p_scaleValue)
{
    AddMovementInput(p_worldDirection * p_scaleValue);
}

void                Player::AddMovementInput        (Vector3 const&  p_worldAcceleration)
{
    m_movementInput += p_worldAcceleration;
}

Vector3             Player::ConsumeMovementInput    ()
{
    m_lastMovementInput = m_movementInput;
    m_movementInput     = Vector3::Zero;

    return m_lastMovementInput;
}

CapsuleComponent*   Player::GetCapsuleCollider      ()  const
{
    return m_capsuleCollider;
}

void                Player::Jump                    ()
{
    if (m_playerMovementComponent)
    {
        m_playerMovementComponent->Jump();
    }

    else
    {
        LOG(LogPlayer, Error, "Player entity '%s' can't jump ! (m_playerMovementComponent is nullptr)", GetName());
    }
}

void                Player::StopJumping             ()
{
    if (m_playerMovementComponent)
    {
        m_playerMovementComponent->StopJumping();
    }

    else
    {
        LOG(LogPlayer, Error, "Player entity '%s' can't stop jumping ! (m_playerMovementComponent is nullptr)", GetName());
    }
}

// ==============================[Private Local Methods]============================== //

bool    Player::ComponentsCreation          ()
{
    bool isComponentCreationHasSuccessed    {true};

    if (!(m_capsuleCollider = AddComponent<CapsuleComponent>("Capsule Collider")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<CapsuleComponent>(\"Capsule Collider\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    

    if (!(m_rifleMesh = AddComponent<StaticMeshComponent>("Rifle Mesh")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<StaticMeshComponent>(\"Rifle Mesh\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }
    
    if (!(m_rifleMuzzle = AddComponent<SceneComponent>("Rifle Muzzle")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<SceneComponent>(\"Rifle Muzzle\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }

    
    if (!(m_playerCamera = AddComponent<CameraComponent>("Player Camera")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<CameraComponent>(\"Player Camera\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }

    if (!(m_playerMovementComponent = AddComponent<PlayerMovementComponent>("Player Movement Component")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<PlayerMovementComponent>(\"Player Movement Component\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }

    if (!(m_playerController = AddComponent<PlayerController>("Player Controller Component")))
    {
        LOG(LogPlayer, Error, "Player entity '%s' creation failed ! (AddComponent<m_playerController>(\"Player Controller Component\") has returned nullptr.", GetName());
        isComponentCreationHasSuccessed = false;
    }

    return isComponentCreationHasSuccessed;
}

void    Player::InitializeCapsuleCollider   ()
{
    SetRootComponent(m_capsuleCollider);

    // Set Default Collision Responses //
    m_capsuleCollider->SetCollisionObjectType(ECC_PLAYER);
    m_capsuleCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_BLOCK_ALL, ECollisionResponse::Block);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_UNCOLORE_OBJECT, ECollisionResponse::Block);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_BLUE_OBJECT, ECollisionResponse::Block);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_PLAYER, ECollisionResponse::Ignore);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_BULLET, ECollisionResponse::Ignore);
    m_capsuleCollider->SetCollisionResponseToChannel(ECC_IGNORED, ECollisionResponse::Ignore);

    // Set Default Properties //
    m_capsuleCollider->SetSimulatePhysics(false);
    m_capsuleCollider->SetEnableGravity(false);
}

void    Player::InitializePlayerCamera      ()
{
    m_playerCamera->SetupAttachment(m_capsuleCollider);
}

void    Player::InitializeProperties        ()
{
    SetEnableCollision(true);
}

void    Player::InitializeRifleMesh         ()
{
    m_rifleMesh->SetupAttachment(m_capsuleCollider);

    // Set Default Collision Responses //
    m_rifleMesh->SetSimulatePhysics(false);
    m_rifleMesh->SetCollisionObjectType(ECC_IGNORED);
    m_rifleMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_rifleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::Ignore);
}

void    Player::InitializeRifleMuzzle       ()
{
    m_rifleMuzzle->SetupAttachment(m_rifleMesh);
}
