#ifndef __PHYSICS_QUERY_FILTER_CALLBACK_SWEEP_HPP__
#define __PHYSICS_QUERY_FILTER_CALLBACK_SWEEP_HPP__

#include "PhysicsEngine/PhysicsQueryFilterCallback.hpp"

class PhysicsQueryFilterCallbackSweep : public PhysicsQueryFilterCallback
{
    typedef PhysicsQueryFilterCallback Base;

    public:
    
    // ==============================[Public Constructor + Destructor]============================== //
    
        bool    m_discardInitialOverlaps;

    // ==============================[Public Local Properties]============================== //

        PhysicsQueryFilterCallbackSweep     () = default;

        PhysicsQueryFilterCallbackSweep     (CollisionQueryParams const&            p_queryParams);

        PhysicsQueryFilterCallbackSweep     (PhysicsQueryFilterCallbackSweep const& p_copy) = default;

        PhysicsQueryFilterCallbackSweep     (PhysicsQueryFilterCallbackSweep&&      p_move) = default;

        ~PhysicsQueryFilterCallbackSweep    () = default;

    // ==============================[Public Local Methods]============================== //

        virtual physx::PxQueryHitType::Enum postFilter  (physx::PxFilterData const& p_filterData,
                                                         physx::PxQueryHit const&   p_queryHit)         override;
        
}; // !class PhysicsQueryFilterCallbackSweep 

#endif // !__PHYSICS_QUERY_FILTER_CALLBACK_SWEEP_HPP__