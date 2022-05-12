#ifndef __ENTITY_INL__
#define __ENTITY_INL__

// =================================[Public Local Methods]================================= //

FORCEINLINE bool									Entity::CanEverTick			 ()	 const
{
	return m_canEverTick;
}

FORCEINLINE void									Entity::CanEverTick			 (bool const			p_canEverTick)
{
	m_canEverTick = p_canEverTick;
}

INLINE void									        Entity::BeginPlay			 ()
{
    if (m_hasBegunPlay || IsPendingKill())
        return;

    m_hasBegunPlay = true;

	for (auto component : m_ownedComponents)
	{
		if (component != nullptr)
		{
            if (!component->IsInitialized())
            {
                component->PreInitialize();
                component->Initialize();
                component->PostInitialize();
            }

			component->BeginPlay();
		}
	}
}

FORCEINLINE bool                                    Entity::Destroy              ()
{
    if (IsPendingKill())
        return false;

    OnDestroy();

    return true;
}

FORCEINLINE void									Entity::EndPlay				 ()
{
    if (!m_hasBegunPlay)
        return;

    m_hasBegunPlay = false;

	for (auto component : m_ownedComponents)
	{
		if (component != nullptr)
		{
		    component->EndPlay();
		}
	}
}

/**
 * Get the backward (-Z) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The backward (-Z) vector from this Entity.
 */
FORCEINLINE Vector3                                 Entity::GetBackward			 ()  const
{
    return (m_rootComponent ? m_rootComponent->GetBackward() : Vector3::Backward);
}

FORCEINLINE std::vector<EntityComponent*> const&    Entity::GetComponents        ()  const
{
    return m_ownedComponents;
}

template <typename Type>
void                                                Entity::GetComponents        (std::vector<Type*>&   p_outEntityComp)    const
{
    if (IsPendingKill())
        return;

    for (EntityComponent* EntityComp : m_ownedComponents)
    {
        Type* casted {dynamic_cast<Type*>(EntityComp)};

        if (casted != nullptr)
        {
            p_outEntityComp.push_back(casted);
        }
    }
}

template <typename Type>
void                                                Entity::GetComponents        (std::vector<Type>&    p_outEntityComp)    const
{
    if (IsPendingKill())
        return;

    for (EntityComponent* EntityComp : m_ownedComponents)
    {
        Type* casted {dynamic_cast<Type*>(EntityComp)};

        if (casted != nullptr)
        {
            p_outEntityComp.push_back(*casted);
        }
    }
}

/**
 * Computes the distance fron this Entity to an another Entity
 *
 * @param p_other   The other Entity to compute the distance with.
 *
 * @return  The distance beetween this and p_other.
 */
FORCEINLINE float                                   Entity::GetDistanceTo        (Entity const*         p_other)    const
{
    return  p_other ? Vector3::Distance(GetWorldLocation(), p_other->GetWorldLocation()) : 0.0f;
}

/**
 * Computes the distance squared fron this Entity to an another Entity
 *
 * @param p_other   The other Entity to compute the distance squared with.
 *
 * @return  The distance squared beetween this and p_other.
 */
FORCEINLINE float                                   Entity::GetDistanceSquaredTo (Entity const*         p_other)    const
{
    return p_other ? Vector3::DistanceSquared(GetWorldLocation(), p_other->GetWorldLocation()) : 0.0f;
}

/**
 * Get the down (-Y) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The down (-Y) vector from this Entity.
 *
 */
FORCEINLINE Vector3                                 Entity::GetDown              ()  const
{
    return (m_rootComponent ? m_rootComponent->GetDown() : Vector3::Down);
}

/**
 * Gets current state of collision for the whole Entity 
 *
 * @return  True if collision is enabled for this Entity, otherwise false.
 */
FORCEINLINE bool                                    Entity::GetEnableCollision   ()  const
{
    return m_enableCollision;
}

/**
 * Get the forward (+Z) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The forward (+Z) vector from this Entity.
 *
 */
FORCEINLINE Vector3                                 Entity::GetForward           ()  const
{
    return (m_rootComponent ? m_rootComponent->GetForward() : Vector3::Forward);
}

/**
 * Get the left (-X) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The left (-X) vector from this Entity.
 *
 */
FORCEINLINE Vector3                                 Entity::GetLeft              ()  const
{
    return (m_rootComponent ? m_rootComponent->GetLeft() : Vector3::Left);
}

/**
 * Gets the relative scale of this Entity (The relative scale of its RootComponent).
 *
 * @return  The relative scale of this Entity. If RootComponent is nullptr, return Vector3::One.
 */
FORCEINLINE Vector3                                 Entity::GetRelativeScale     ()  const
{
    return (m_rootComponent ? m_rootComponent->GetRelativeScale() : Vector3::One);
}

/**
 * Get the right (+X) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The right (+X) vector from this Entity.
 *
 */
FORCEINLINE Vector3                                 Entity::GetRight             ()  const
{
    return (m_rootComponent ? m_rootComponent->GetRight() : Vector3::Right);
}

/**
 * Gets the root component of this Entity. If the root component is nullptr,
 * this Entity is not spacialized.
 *
 * @return The root component of this.
 */
FORCEINLINE SceneComponent*                         Entity::GetRootComponent     ()  const
{
    return m_rootComponent;
}

/**
 * Get the up (-Y) vector (Length 1.0f) from this Entity, in world space.
 *
 * @return The up (-X) vector from this Entity.
 *
 */
FORCEINLINE Vector3                                 Entity::GetUp                ()  const
{
    return (m_rootComponent ? m_rootComponent->GetUp() : Vector3::Up);
}

/**
 * Gets the location in world space of this Entity (The location in world space of its RootComponent).
 * 
 * @return  The location in world space of this Entity. If RootComponent is nullptr, return Vector3::Zero.
 */
FORCEINLINE Vector3                                 Entity::GetWorldLocation     ()  const
{
    return (m_rootComponent ? m_rootComponent->GetWorldLocation() : Vector3::Zero);
}

/**
 * Gets the rotation in world space of this Entity (The rotation in world space of its RootComponent).
 * 
 * @return  The rotation in world space of this Entity. If RootComponent is nullptr, return Quaternion::Identity.
 */
FORCEINLINE Quaternion                              Entity::GetWorldRotation     ()  const
{
    return (m_rootComponent ? m_rootComponent->GetWorldRotation() : Quaternion::Identity);
}

/**
 * Gets the scale in world space of this Entity (The scale in world space of its RootComponent).
 *
 * @return  The scale in world space of this Entity. If RootComponent is nullptr, return Vector3::One.
 */
FORCEINLINE Vector3                                 Entity::GetWorldScale        ()  const
{
    return (m_rootComponent ? m_rootComponent->GetWorldScale() : Vector3::One);
}

/**
 * Gets the transform in world space of this Entity (The transform in world space of its RootComponent).
 * Corresponds to the local-to-world transform of this Entity/RootComponent.
 *
 * @return  The transform in world space of this Entity. If RootComponent is nullptr, return Transform::Identity.
 */
FORCEINLINE Transform                               Entity::GetWorldTransform    ()  const
{
    return (m_rootComponent ? m_rootComponent->GetWorldTransform() : Transform::Identity);
}

FORCEINLINE bool                                    Entity::HasBegunPlay         ()  const
{
    return m_hasBegunPlay;
}

INLINE void                                         Entity::Initialize           ()
{
    if (IsInitialized())
        return;

    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            if (!component->IsInitialized())
            {
                component->Initialize();
            }
        }
    }

    OnInitialize();
}

INLINE bool                                         Entity::IsInitialized        ()  const
{
    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            if (!component->IsInitialized())
                return false;
        }
    }

    return true;
}

FORCEINLINE void                                    Entity::OnDestroy            ()
{

}

FORCEINLINE void                                    Entity::OnInitialize         ()
{

}

FORCEINLINE void                                    Entity::OnPreInitialize      ()
{

}

FORCEINLINE void                                    Entity::OnPostInitialize     ()
{
    UpdateComponentTransforms();
}

INLINE void                                         Entity::PostInitialize       ()
{
    if (IsInitialized())
        return;

    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            if (!component->IsInitialized())
            {
                component->PostInitialize();
            }
        }
    }

    OnPostInitialize();
}

INLINE void                                         Entity::PreInitialize        ()
{
    if (IsInitialized())
        return;

    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            if (!component->IsInitialized())
            {
                component->PreInitialize();
            }
        }
    }

    OnPreInitialize();
}

FORCEINLINE bool                                    Entity::RemoveComponent      (EntityComponent*      p_entityComponent)
{
    if (p_entityComponent == nullptr)
        return false;

    auto result = std::find(m_ownedComponents.begin(), m_ownedComponents.end(), p_entityComponent);

    if (result != m_ownedComponents.end())
        m_ownedComponents.erase(result);

    return p_entityComponent->Destroy();
}

INLINE void                                         Entity::Tick                 (float const           p_deltaTime)
{
    if (!m_canEverTick || !m_hasBegunPlay || IsPendingKill())
        return;

    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            if (component->CanEverTick())
            {
                component->Tick(p_deltaTime);
            }
        }
    }
}

#endif // !__ENTITY_INL__