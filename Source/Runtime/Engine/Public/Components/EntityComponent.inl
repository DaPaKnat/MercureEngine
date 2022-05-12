#ifndef __ENTITY_COMPONENT_INL__
#define __ENTITY_COMPONENT_INL__

#include "ObjectManager.hpp"

inline EntityComponent::EntityComponent () :
    m_canEverTick           {true},
    m_isActive              {false},
    m_owner                 {nullptr},
    m_hasBegunPlay          {false},
    m_physicsStateCreated   {false},
    m_isInitialized         {false}
{}

// =================================[Public Local Methods]================================= //

FORCEINLINE void    EntityComponent::CanEverTick            (bool const p_canEverTick)
{
    m_canEverTick = p_canEverTick;
}

FORCEINLINE bool    EntityComponent::CanEverTick            ()  const
{
    return m_canEverTick;
}

INLINE bool         EntityComponent::Destroy                ()
{
    if (IsPendingKill())
        return false;

    DestroyPhysicsState();
    OnDestroy();

    return ObjectManager::Get().DestroyObject(this);
}

/**
 * Gets the Entity Owner of this Entity component.
 *
 * @return The owner of this component.
 */
FORCEINLINE Entity* EntityComponent::GetOwner               ()  const
{
    return m_owner;
}

FORCEINLINE bool    EntityComponent::HasBegunPlay           ()  const
{
    return m_hasBegunPlay;
}

FORCEINLINE void    EntityComponent::Initialize             ()
{
    if (!m_isInitialized)
    {
        OnInitialize();
        CreatePhysicsState();
    }
}

FORCEINLINE bool    EntityComponent::IsInitialized          ()  const
{
    return m_isInitialized;
}

/**
 * Ask if the physics state of this component is created.
 *
 * @return True if the physics state is created, otherwise false.
 */
FORCEINLINE bool    EntityComponent::IsPhysicsStateCreated  ()  const
{
    return m_physicsStateCreated;
}

FORCEINLINE void    EntityComponent::PostInitialize         ()
{
    if (!m_isInitialized)
        OnPostInitialize();

    m_isInitialized = true;
}

FORCEINLINE void    EntityComponent::PreInitialize          ()
{
    if (!m_isInitialized)
        OnPreInitialize();
}

// =================================[Protected Local Methods]================================= //

FORCEINLINE void    EntityComponent::OnDestroy              ()
{

}

FORCEINLINE void    EntityComponent::OnInitialize           ()
{

}

FORCEINLINE void    EntityComponent::OnPostInitialize       ()
{

}

FORCEINLINE void    EntityComponent::OnPreInitialize        ()
{

}

#endif // !__ENTITY_COMPONENT_INL__
