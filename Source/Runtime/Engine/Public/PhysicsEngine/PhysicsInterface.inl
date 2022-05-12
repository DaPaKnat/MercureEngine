#ifndef __PHYSICS_INTERFACE_INL__
#define __PHYSICS_INTERFACE_INL__

// ==============================[Public Static Methods]============================== //

/**
 * Methods used to modify the value of a physx::PxActorFlag int physx::PxActorFlags.
 *
 * @param p_flagtoSet       The flag to set into the flag container.
 * @param p_flagContainer   The flag container where we will set a specified flag.
 * @param p_value           The value of the flag that we want to modify
 */
template <const physx::PxActorFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyActorFlag                                           (physx::PxActorFlags&       p_flagContainer,
                                                                                         bool const                 p_value)
{
    ModifyFlagDefault(p_flagContainer, p_flagToSet, p_value);
}
/**
 * Methods used to modify the value of a physx::PxActorFlag of a physx::PxActor.
 *
 * @param p_flagtoSet       The flag to set into the flag container.
 * @param p_actor           The actor to applied the flag modification.
 * @param p_value           The value of the flag that we want to modify.
 */
template <const physx::PxActorFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyActorFlag                                           (physx::PxActor*            p_actor,
                                                                                         bool const                 p_value)
{
    physx::PxActorFlags actorFlags {p_actor->getActorFlags()};
    ModifyActorFlag<p_flagToSet>(actorFlags, p_value);
    p_actor->setActorFlags(actorFlags);
}

/**
 * Default methods used to modify the value of a PhysX flag into a PhysXFlagContainer.
 *
 * @param p_flagContainer   The flag container where we will set a specified flag.
 * @param p_flagtoSet       The flag to set into the flag container.
 * @param p_value           The value of the flag that we want to modify
 */
template <typename FlagContainer, typename FlagType>
INLINE void PhysicsInterface::ModifyFlagDefault                                         (FlagContainer&             p_flagContainer,
                                                                                         FlagType const             p_flagtoSet,
                                                                                         bool const                 p_value)
{
    if (p_value == true)
    {
        p_flagContainer |= p_flagtoSet;
    }

    else // (m_value == false)
    {
        p_flagContainer.clear(p_flagtoSet);
    }
}

/**
 * Methods used to modify the value of a physx::PxRigidBodyFlag into physx::PxRigidBodyFlags.
 *
 * @param p_flagtoSet       The flag to set into the flag container.
 * @param p_flagContainer   The flag container where we will set a specified flag.
 * @param p_value           The value of the flag that we want to modify.
 */
template <const physx::PxRigidBodyFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyRigidBodyFlag                                       (physx::PxRigidBodyFlags&   p_flagContainer,
                                                                                         bool const                 p_value)
{
    PhysicsInterface::ModifyFlagDefault(p_flagContainer, p_flagToSet, p_value);
}

/**
 * Methods used to modify the value of a physx::PxRigidBodyFlag::eKINEMATIC into physx::PxRigidBodyFlags.
 * A physx::PxRigidBody can't be Continuous Collision Detectection (CCD) and Kinematic at the same time.
 * If we want ENABLE Kinematic and the CCD is on, we disable it and turn on speculative CCD instead.
 * If we want DISABLE Kinematic and the Speculative CCD is on, we disable it and turn on CCD instead.
 *
 * @param p_flagContainer   The flag container where we will set a specified flag.
 * @param p_value           The value of the flag that we want to modify
 */
template<>
INLINE void PhysicsInterface::ModifyRigidBodyFlag<physx::PxRigidBodyFlag::eKINEMATIC>   (physx::PxRigidBodyFlags&   p_flagContainer,
                                                                                         bool const                 p_value)
{
	if (p_value && p_flagContainer.isSet(physx::PxRigidBodyFlag::eENABLE_CCD))
	{
        p_flagContainer |= physx::PxRigidBodyFlag::eKINEMATIC;
        p_flagContainer |= physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD;
        p_flagContainer.clear(physx::PxRigidBodyFlag::eENABLE_CCD);
	}

	else if (!p_value && p_flagContainer.isSet(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD))
	{
        p_flagContainer |= physx::PxRigidBodyFlag::eENABLE_CCD;
        p_flagContainer.clear(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD);
        p_flagContainer.clear(physx::PxRigidBodyFlag::eKINEMATIC);
	}

	else
	{
		ModifyFlagDefault(p_flagContainer, physx::PxRigidBodyFlag::eKINEMATIC, p_value);
	}  
}

/**
 * Methods used to modify the value of a physx::PxRigidBodyFlag::eENABLE_CCD into physx::PxRigidBodyFlags.
 * A physx::PxRigidBody can't be Continuous Collision Detectection (CCD) and Kinematic at the same time.
 * If we want ENABLE CCD and the Speculative CCD is on, we disable it too.
 * If we want DISABLE CCD but the Kinematic is on, we enable the Speculative CCD instead.
 *
 * @param p_flagContainer   The flag container where we will set a specified flag.
 * @param p_value           The value of the flag that we want to modify
 */
template<>
INLINE void PhysicsInterface::ModifyRigidBodyFlag<physx::PxRigidBodyFlag::eENABLE_CCD>  (physx::PxRigidBodyFlags&   p_flagContainer,
                                                                                         bool const                 p_value)
{
	if (!p_value && p_flagContainer.isSet(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD))
	{
        p_flagContainer.clear(physx::PxRigidBodyFlag::eENABLE_CCD);
        p_flagContainer.clear(physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD);
	}

	else if (p_value && p_flagContainer.isSet(physx::PxRigidBodyFlag::eKINEMATIC))
	{
        p_flagContainer |= physx::PxRigidBodyFlag::eENABLE_SPECULATIVE_CCD;
	}

	else
	{
		ModifyFlagDefault(p_flagContainer, physx::PxRigidBodyFlag::eENABLE_CCD, p_value);
	}
}

/**
 * Methods used to modify the value of a physx::PxRigidBodyFlag of a physx::PxRigidBody.
 *
 * @param p_flagtoSet       The flag to set into the flag container.
 * @param p_rigidBody       The rigidBody to applied the flag modification.
 * @param p_value           The value of the flag that we want to modify.
 */
template <const physx::PxRigidBodyFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyRigidBodyFlag                                       (physx::PxRigidBody*        p_rigidBody,
                                                                                         bool const                 p_value)
{
    physx::PxRigidBodyFlags rigidBodyFlags  {p_rigidBody->getRigidBodyFlags()};
    ModifyRigidBodyFlag<p_flagToSet>(rigidBodyFlags, p_value);
    p_rigidBody->setRigidBodyFlags(rigidBodyFlags);
}

template <const physx::PxShapeFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyShapeFlag                                           (physx::PxShapeFlags&       p_flagContainer,
                                                                                         bool const                 p_value)
{
    ModifyFlagDefault(p_flagContainer, p_flagToSet, p_value);
}


template <const physx::PxShapeFlag::Enum p_flagToSet>
INLINE void PhysicsInterface::ModifyShapeFlag                                           (physx::PxShape*            p_shape,
                                                                                         bool const                 p_value)
{
    physx::PxShapeFlags shapeFlags  {p_shape->getFlags()};
    ModifyShapeFlag<p_flagToSet>(shapeFlags, p_value);
    p_shape->setFlags(shapeFlags);
}

#endif // !__PHYSICS_INTERFACE_INL__
