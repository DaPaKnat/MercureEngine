#ifndef __BODY_COLLISION_DATA_HPP__
#define __BODY_COLLISION_DATA_HPP__

#include "BodyCollisionFilterData.hpp"
#include "BodyCollisionFlags.hpp"

struct BodyCollisionData
{
    // ==============================[Public Local Properties]============================== //

        /** ... */
        BodyCollisionFlags      m_collisionFlags;

        /** ... */
        BodyCollisionFilterData m_collisionFilterData;

    // ==============================[Public Local Constructor + Destructorc]============================== //

        BodyCollisionData   ()  = default;

        BodyCollisionData   (BodyCollisionFlags const&      p_collisionFlags,
                             BodyCollisionFilterData const& p_collisionFilterData);

        BodyCollisionData   (BodyCollisionData const&       p_copy);

        BodyCollisionData   (BodyCollisionData&&            p_move);

        ~BodyCollisionData ()   = default;

    // ==============================[Public Local Operators]============================== //

        bool                operator==  (BodyCollisionData const&  p_other)    const;
        bool                operator!=  (BodyCollisionData const&  p_other)    const;

        BodyCollisionData&  operator=   (BodyCollisionData const&  p_copy);
        BodyCollisionData&  operator=   (BodyCollisionData&&       p_move);

};  // !struct BodyCollisionData

#endif // !__BODY_COLLISION_DATA_HPP__