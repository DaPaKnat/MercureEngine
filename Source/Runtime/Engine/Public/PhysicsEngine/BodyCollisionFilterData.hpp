#ifndef __BODY_COLLISION_FILTER_DATA_HPP__
#define __BODY_COLLISION_FILTER_DATA_HPP__

#include "CollisionFilterData.hpp"

struct BodyCollisionFilterData
{
    // ==============================[Public Local Properties]============================== //

        /** ... */
        CollisionFilterData m_simulationFilter;

        /** ... */
        CollisionFilterData m_querySimpleFilter;

        /** ... */
        CollisionFilterData m_queryComplexFilter;

    // ==============================[Public Local Properties]============================== //

        BodyCollisionFilterData     ()  = default;

        BodyCollisionFilterData     (CollisionFilterData const&     p_simulationFilter,
                                     CollisionFilterData const&     p_querySimplefilter,
                                     CollisionFilterData const&     p_queryComplexFilter);

        BodyCollisionFilterData     (BodyCollisionFilterData const& p_copy);

        BodyCollisionFilterData     (BodyCollisionFilterData&&      p_move);

        ~BodyCollisionFilterData    ()  = default;

    // ==============================[Public Local Operators]============================== //

        bool                        operator==  (BodyCollisionFilterData const& p_other)    const;
        bool                        operator!=  (BodyCollisionFilterData const& p_other)    const;

        BodyCollisionFilterData&    operator=   (BodyCollisionFilterData const& p_copy);
        BodyCollisionFilterData&    operator=   (BodyCollisionFilterData&&      p_move);

};  // !struct BodyBodyCollisionFilterData

#endif // !__BODY_COLLISION_FILTER_DATA_HPP__