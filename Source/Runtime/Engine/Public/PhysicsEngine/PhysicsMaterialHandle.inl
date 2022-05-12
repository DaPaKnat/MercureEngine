#ifndef __PHYSICS_MATERIAL_HANDLE_INL__
#define __PHYSICS_MATERIAL_HANDLE_INL__

// ==============================[Public Local Methods]============================== //

/**
 * Check if physx::PxMaterial (m_material) is valid.
 *
 * @return True if m_material != nullptr, otherwise false.
 */
FORCEINLINE bool    PhysicsMaterialHandle::IsValid  ()  const
{
    return (m_material == nullptr);
}

#endif // !__PHYSICS_MATERIAL_HANDLE_INL__