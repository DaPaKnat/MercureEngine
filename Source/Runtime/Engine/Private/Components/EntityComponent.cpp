#include "PCH.hpp"

#include "Components/EntityComponent.hpp"

#include "World.hpp"

// =================================[Virtual Public Local Methods]================================= //

void    EntityComponent::Activate                   ()
{
    if (ShouldActivate() == true)
    {
        m_canEverTick   = false;
        m_isActive      = true;
        
        // Call Delegate
    }
}

void    EntityComponent::BeginPlay                  ()
{
    m_hasBegunPlay = true;
}

void    EntityComponent::Deactivate                 ()
{
    if (ShouldActivate() == false)
    {
        m_canEverTick   = false;
        m_isActive      = false;
        
        // Call Delegate
    }
}

void    EntityComponent::EndPlay                    ()
{
    m_hasBegunPlay = false;

    DestroyPhysicsState();
}

/**
 * Checks if the physics state of this body is valid.
 *
 * @return True if the physic state of this is valid, otherwise false.
 */
bool    EntityComponent::HasValidPhysicsState           ()  const
{
    return false;
}

bool    EntityComponent::IsActive                       ()
{
    return m_isActive;
}

/**
 * Create physics engine information for this component.
 */
void    EntityComponent::OnCreatePhysicsState           ()
{
    if (ShouldCreatePhysicsState() == false)
    {
        LOG(LogPhysics, Warning, "(OnCreatePhysicsState) You tried to create physics state wherease you're not supposed to !");
        return;
    }

    if (m_physicsStateCreated == true)
    {
        LOG(LogPhysics, Warning, "(OnCreatePhysicsState) You tried to create physics state wherease is already created!");
        return;
    }

    m_physicsStateCreated = true;
}

/**
 * Shut down physics engine information for this component.
 */
void    EntityComponent::OnDestroyPhysicsState          ()
{
    if (m_physicsStateCreated == false)
    {
        LOG(LogPhysics, Warning, "(OnDestroyPhysicsState) You tried to shut down physics state wherease is not created !");
        return;
    }

    m_physicsStateCreated = false;
}

void    EntityComponent::OnEntityEnableCollisionChanged()
{

}

void    EntityComponent::Serialize                  (Json&          p_seriliaze)  const
{
    p_seriliaze = Json(*this);
}

void    EntityComponent::SetActive                      (bool const                     p_newActive)
{
    if (p_newActive == true)
    {
        Activate();
    }

    else // (p_newActive == false)
    {
        Deactivate();
    }
}

bool    EntityComponent::ShouldActivate                 ()  const
{
    return !m_isActive;
}

/**
 * Should this component create a physics state (Should we call CreatePhysicsState).
 *
 * @return True if we should create a physics state.
 */
bool    EntityComponent::ShouldCreatePhysicsState       ()  const
{
    return false;
}

void    EntityComponent::ToggleActive                   ()
{
    SetActive(!m_isActive);
}

void    EntityComponent::Tick                           (float const                    p_deltaTime)
{

}

/**
 * Update the local to world transform of this component.
 * An entity component is not allway spatialized (= SceneComponent). In that case, it's do nothing.
 */
void    EntityComponent::UpdateLocalToWorld             (EUpdateTransformFlags const    p_updateTransformFlags /** = EUpdateTransformFlags::None */, 
                                                         ETeleportType const            p_teleportType /**= ETeleportType::None */)
{
}


// =================================[Public Local Methods]================================= //

/**
 * Creates the physics state of this component.
 */
void    EntityComponent::CreatePhysicsState     ()
{
    if (!m_physicsStateCreated && GetWorld()->GetPhysicsScene() && ShouldCreatePhysicsState())
    {
        OnCreatePhysicsState();

        if (!m_physicsStateCreated)
        {
            LOG(LogEntityComponent, Warning, " Physics state creation failed : OnCreatePhysicsState failed !");
        }
    }
}

/**
 * Destroys the physics state of this component.
 */
void    EntityComponent::DestroyPhysicsState    ()
{
    if (m_physicsStateCreated)
    {
        OnDestroyPhysicsState();

        if (m_physicsStateCreated)
        {
            LOG(LogEntityComponent, Warning, " Physics state destruction failed : OnDestroyPhysicsState failed !");
        }
    }
}

World*  EntityComponent::GetWorld               ()  const
{
    return m_owner ? m_owner->GetWorld() : nullptr;
}

/**
 * Recreate the physics state.
 */
void    EntityComponent::RecreatePhysicsState   ()
{
    DestroyPhysicsState();

    CreatePhysicsState();
}

 void   EntityComponent::SetOwner               (Entity* p_owner)
 {
     m_owner = p_owner;
 }