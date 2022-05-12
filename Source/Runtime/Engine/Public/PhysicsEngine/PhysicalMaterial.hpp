#ifndef __PHYSICAL_MATERIAL_HPP__
#define __PHYSICAL_MATERIAL_HPP__

#include "CoreMinimal.hpp"

#include "PhysicsMaterialHandle.hpp"

/**
 * Physical material describes how handle colliding physical object.
 */
class ENGINE_API PhysicalMaterial
{

    public:

    // ==============================[Public Constructor + Destructor]==================== //

        PhysicalMaterial    ();

        PhysicalMaterial    (PhysicalMaterial const&    p_copy) = default;

        PhysicalMaterial    (PhysicalMaterial&&         p_move) = default;

        ~PhysicalMaterial   ()                                  = default;

    // ==============================[Public Local Methods]==================== //
    
        FORCEINLINE float               GetDynamicFriction                      ()                                                                  const;

        FORCEINLINE EPhysicsCombineMode GetFrictionCombineMode                  ()                                                                  const;

        FORCEINLINE EPhysicsCombineMode GetOverrideFrictionCombineMode          ()                                                                  const;

        FORCEINLINE EPhysicsCombineMode GetOverrideRestitutionCombineMode       ()                                                                  const;

        PhysicsMaterialHandle&          GetPhysicsMaterialHandle                ();

        FORCEINLINE float               GetRestitution                          ()                                                                  const;

        FORCEINLINE EPhysicsCombineMode GetRestitutionCombineMode               ()                                                                  const;

        FORCEINLINE float               GetStaticFriction                       ()                                                                  const;

        FORCEINLINE bool                IsOverrideFrictionCombineMode           ()                                                                  const;

        FORCEINLINE bool                IsOverrideRestitutionCombineMode        ()                                                                  const;

        void                            SetDynamicFriction                      (float const                p_newDynamicFriction);

        void                            SetIsOverridingFrictionCombineMode      (bool const                 p_isOverrideCombineMode);

        void                            SetIsOverridingRestitutionCombineMode   (bool const                 p_isOverrideCombineMode);

        void                            SetOverrideFrictionCombineMode          (EPhysicsCombineMode const  p_newOverrideFrictionCombineMode,
                                                                                 bool const                 p_overrideFrictionCombineMode  = true );

        void                            SetOverrideRestitutionCombineMode       (EPhysicsCombineMode const  p_newOverrideRestitutionCombineMode,
                                                                                 bool const                 p_overrideResitutionCombineMode = true);

        void                            SetRestitution                          (float const                p_newRestitution);

        void                            SetStaticFriction                       (float const                p_newStaticFriction);

    protected:

    // ==============================[Public Local Properties]==================== //

        /** 
         * The friction use when the object is already moving. In range [0.0f, FLOAT_MAX[
         * A value of 0.0f feels like ice, 1.0f feels like rubber.
         **/
        float                   m_dynamicFriction;

        /** If set we will use the PhysicsCombineMode of this material, instead of the PhysicsCombineMode found in the Physics Settings. */
        bool                    m_isOverridingFrictionCombineMode;

        /** If set we will use the PhysicsCombineMode of this material, instead of the PhysicsCombineMode found in the Physics Settings. */
        bool                    m_isOverridingRestitutionCombineMode;

        /** Internal physics representation of our material */
        PhysicsMaterialHandle   m_materialHandle;

        /** Friction Combine Mode, controls how restitution is computed for multiple material. */
        EPhysicsCombineMode     m_overrideFrictionCombineMode;

        /** Restitution Combine Mode, controls how restitution is computed for multiple material. */
        EPhysicsCombineMode     m_overrideRestitutionCombineMode;

        /** 
         * Restitution (or Bounciness) of the material in range [0.0f, 1.0f].
         * If value = 0.0f -> No bounce.
         * If value = 1.0f -> outgoing velocity is same as incoming.
         */
        float                   m_restitution;

        /** The friction coefficient used when an object is lying on a surface. In range [0.0f, FLOAT_MAX[
         * A value of 0.0f feels like ice, 1.0f feels like rubber.
         */
        float                   m_staticFriction;

}; // !class PhysicalMaterial

#include "PhysicalMaterial.inl"

#endif // !__PHYSICAL_MATERIAL_HPP__