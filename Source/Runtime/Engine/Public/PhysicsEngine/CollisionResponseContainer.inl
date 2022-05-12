#ifndef __COLLISION_RESPONSE_CONTAINER_INL__
#define __COLLISION_RESPONSE_CONTAINER_INL__

// ==============================[Public Static Methods]============================== //

/**
 * Gets default collision response container.
 */
FORCEINLINE CollisionResponseContainer const&   CollisionResponseContainer::GetDefaultResponseContainer ()
{
    return CollisionResponseContainer::DefaultResponseContainer;
}

// ==============================[Public Local Methods]==============================//

/**
 * Gets the response set on the specified channel.
 *
 * @param   The channel that we want know the response set on.
 */
FORCEINLINE ECollisionResponse  CollisionResponseContainer::GetResponse (ECollisionChannel const    p_channel)  const
{
    return ECollisionResponse(m_enumArray[static_cast<uint8>(p_channel)]);
}

#endif // !__COLLISION_RESPONSE_CONTAINER_INL__
