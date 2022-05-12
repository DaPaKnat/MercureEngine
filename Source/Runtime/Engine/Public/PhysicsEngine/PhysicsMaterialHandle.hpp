#ifndef __PHYSICS_MATERIAL_HANDLE_HPP__
#define __PHYSICS_MATERIAL_HANDLE_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"
#include "PhysicsEnums.hpp"

/**
 * Struture used as interface between physx::material and Physics Engine Material,
 */
struct PhysicsMaterialHandle
{
    // ==============================[Public Local Properties]============================== //

        physx::PxMaterial*  m_material;

    // ==============================[Public Constructor + Destructor]============================== //

        PhysicsMaterialHandle   ();

        PhysicsMaterialHandle   (physx::PxMaterial*             p_material);

        PhysicsMaterialHandle   (PhysicsMaterialHandle const&   p_copy);

        PhysicsMaterialHandle   (PhysicsMaterialHandle&&        p_move);

        ~PhysicsMaterialHandle  ()                                              = default;

    // ==============================[Public Local Methods]============================== //

        float               GetDynamicFriction          ()                                                      const;

        float               GetRestitution              ()                                                      const;

        float               GetStaticFriction           ()                                                      const;

        FORCEINLINE bool    IsValid                     ()                                                      const;

        void                Release                     ();

        void                SetDynamicFriction          (float const                p_newDynamicFriction);

        void                SetFrictionCombineMode      (EPhysicsCombineMode const  p_newFrictionCombineMode);

        void                SetRestitution              (float const                p_newRestitution);

        void                SetRestitutionCombineMode   (EPhysicsCombineMode const  p_newFrictionCombineMode);

        void                SetStaticFrinction          (float const                p_newStaticFriction);

        void                SetUserData                 (void*                      p_userData);

    // ==============================[Public Local Operator]============================== //

        bool                    operator==  (PhysicsMaterialHandle const&   p_other)    const;
        bool                    operator!=  (PhysicsMaterialHandle const&   p_other)    const;

        PhysicsMaterialHandle&  operator=   (PhysicsMaterialHandle const&   p_copy);
        PhysicsMaterialHandle&  operator=   (PhysicsMaterialHandle&&        p_move);

}; // !PhysicsActorHandle

#include "PhysicsMaterialHandle.inl"

#endif // !__PHYSICS_MATERIAL_HANDLE_HPP__