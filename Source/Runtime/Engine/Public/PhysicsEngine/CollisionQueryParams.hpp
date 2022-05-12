#ifndef __COLLISION_QUERY_PARAMS_HPP__
#define __COLLISION_QUERY_PARAMS_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"

#include "Components/PrimitiveComponent.hpp"

// ==============================[Enum Definition]============================== //

/**
 * Description : TODO
 */
enum class EQueryMobilityType : uint8
{
    Any,
    Dynamic,
    Static,

}; // !enum class EQueryMobilityType 

// ============================================================================= //


class ENGINE_API CollisionQueryParams
{
    public: 

    // ==============================[Public Local Properties]============================== //

        static CollisionQueryParams DefaultQueryParams;

    // ==============================[Public Local Properties]============================== //

        bool                m_findInitialOverlaps;

        bool                m_ignoreBlocks;

        bool                m_ignoreTouchs;

        EQueryMobilityType  m_queryMobilityType;

        bool                m_returnFaceIndex;

        bool                m_returnPhysicalMaterial;

        bool                m_traceComplex;


    // ==============================[Public Local Constructor + Destructor]============================== //

        CollisionQueryParams    ();

        CollisionQueryParams    (CollisionQueryParams const&    p_copy) = default;

        CollisionQueryParams    (CollisionQueryParams&&         p_move) = default;

        ~CollisionQueryParams   () = default;

    // ==============================[Public Local Methods]============================== //

        void                                    AddIgnoredComponent         (PrimitiveComponent*                        p_componentToIgnore);

        void                                    AddIgnoredComponents        (std::vector<PrimitiveComponent*> const&    p_componentsToIgnore);

        void                                    AddIgnoredEntity            (Entity*                                    p_entityToIgnore);

        void                                    AddIgnoredEntities          (std::vector<Entity*> const&                p_entitiesToIgnore);

        void                                    ClearIgnoredComponents      ();

        void                                    ClearIgnoredEntities        ();
        
        std::vector<PrimitiveComponent*> const& GetIngoredComponents        ()  const;

        std::vector<Entity*> const&             GetIgnoredEntities          ()  const;

        physx::PxQueryFlags                     GetPhysXQueryMobilityFlags  ()  const;

    private:

    // ==============================[Public Local Properties]============================== //

        std::vector<Entity*>                m_ignoredEntities;

        std::vector<PrimitiveComponent*>    m_ignoredComponents;

}; // !class CollisionQueryParams

#endif // !__COLLISION_QUERY_PARAMS_HPP__