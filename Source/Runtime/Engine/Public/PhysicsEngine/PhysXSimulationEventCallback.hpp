#ifndef __PHYSX_SIMULATION_EVENT_CALLBACK_HPP__
#define __PHYSX_SIMULATION_EVENT_CALLBACK_HPP__

#include "PxPhysicsAPI.h"

/**
 * Class used to receive PhysX simulation Event.
 */
class PhysXSimulationEventCallback : public physx::PxSimulationEventCallback
{
    public:

    // ==============================[Public Constructor + Destructor]============================== //

        PhysXSimulationEventCallback    ()                                              = default;

        PhysXSimulationEventCallback    (PhysXSimulationEventCallback const&    p_copy) = default;

        PhysXSimulationEventCallback    (PhysXSimulationEventCallback&&         p_move) = default;

        ~PhysXSimulationEventCallback   ()                                              = default;

    // ==============================[Virtual Public Local Methods]============================== //

        virtual void onAdvance          (physx::PxRigidBody const* const*   p_bodyBuffer, 
                                         physx::PxTransform const*          p_poseBuffer, 
                                         physx::PxU32 const                 p_count)                override;

	    virtual void onConstraintBreak  (physx::PxConstraintInfo*           p_constraints, 
                                         physx::PxU32                       p_count)                override;

        virtual void onContact          (physx::PxContactPairHeader const&  p_pairHeader, 
                                         physx::PxContactPair const*        p_pairs, 
                                         physx::PxU32                       p_nbPairs)              override;

	    virtual void onSleep            (physx::PxActor**                   p_actors, 
                                         physx::PxU32                       p_count)                override;

	    virtual void onTrigger          (physx::PxTriggerPair*              p_pairs, 
                                         physx::PxU32                       p_count)                override;

        virtual void onWake             (physx::PxActor**                   p_actors, 
                                         physx::PxU32                       p_count)                override;

}; // !class PhysXSimulationEventCallback

#endif // !__PHYSX_SIMULATION_EVENT_CALLBACK_HPP__
