#ifndef __PHYSICS_QUERY_FILTER_CALLBACK_HPP__
#define __PHYSICS_QUERY_FILTER_CALLBACK_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"

#include "CollisionQueryParams.hpp"

// ==============================[Forward Declaraction]==============================//

class Entity;
class PrimitiveComponent;

// ==================================================================================//

class PhysicsQueryFilterCallback : public physx::PxQueryFilterCallback
{

    public:

    // ==============================[Public Static Methods]============================== //
    
        static physx::PxQueryHitType::Enum  ComputeQueryHitType (physx::PxFilterData const& p_queryFilter,
                                                                 physx::PxFilterData const& p_shapeFilter,
                                                                 bool const                 p_preFilter = false);

    // ==============================[Public Constructor + Destructor]============================== //

        /** Whether to ignore blocks (convert an eBLOCK result to eNONE). */
        bool                                    m_ignoreBlocks;

        /** Whether to ignore touches (convert an eTOUCH result to eNONE). */
        bool                                    m_ignoreTouches;

        bool                                    m_isOverlapQuery;

        std::vector<PrimitiveComponent*> const& m_ignoredComponents;

        std::vector<Entity*> const&             m_ignoredEntities;

    // ==============================[Public Local Properties]============================== //

        PhysicsQueryFilterCallback  () = default;

        PhysicsQueryFilterCallback  (CollisionQueryParams const&        p_queryParams);

        PhysicsQueryFilterCallback  (PhysicsQueryFilterCallback const&  p_copy) = default;

        PhysicsQueryFilterCallback  (PhysicsQueryFilterCallback&&       p_move) = default;

        ~PhysicsQueryFilterCallback () = default;


    // ==============================[Public Local Methods]============================== //

        virtual physx::PxQueryHitType::Enum preFilter   (physx::PxFilterData const& p_filterData,
                                                         physx::PxShape const*      p_shape,
                                                         physx::PxRigidActor const* p_actor,
                                                         physx::PxHitFlags&         p_outQueryFlags)    override;

        virtual physx::PxQueryHitType::Enum postFilter  (physx::PxFilterData const& p_filterData,
                                                         physx::PxQueryHit const&   p_queryHit)         override;

        
    protected:

    // ==============================[Protected Local Properties]============================== //

        /** The value return by 'postFilter' methods */
        physx::PxQueryHitType::Enum m_preFilterReturnValue;

}; // !class PhysicsQueryFilterCallback 

#endif // !__PHYSICS_QUERY_FILTER_CALLBACK_HPP__