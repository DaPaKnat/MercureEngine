#ifndef __BODY_COLLISION_FLAGS_HPP__
#define __BODY_COLLISION_FLAGS_HPP__

#include "CoreMinimal.hpp"

/**
 * Description : TODO
 */
struct BodyCollisionFlags
{

    // ==============================[Public Local Properties]============================== //

        /** ... */
        bool    m_enableSimulationCollisionSimple;

        /** ... */
        bool    m_enableSimulationCollisionComplex;

        /** ... */
        bool    m_enableQueryCollision;

    // ==============================[Public Constructor + Destructor]============================== //

        BodyCollisionFlags  ();

        BodyCollisionFlags  (bool const                 p_enableSimulationCollisionSimple,
                             bool const                 p_enableSimulationCollisionComplex,
                             bool const                 p_enableQueryCollision);

        BodyCollisionFlags  (BodyCollisionFlags const&  p_copy);

        BodyCollisionFlags  (BodyCollisionFlags&&       p_move);

        ~BodyCollisionFlags ()                                  = default;


    // ==============================[Public Local Operators]============================== //

        bool                operator==  (BodyCollisionFlags const&  p_other)    const;
        bool                operator!=  (BodyCollisionFlags const&  p_other)    const;

        BodyCollisionFlags& operator=   (BodyCollisionFlags const&  p_copy);
        BodyCollisionFlags& operator=   (BodyCollisionFlags&&       p_move);

};  // !struct BodyCollisionFlags

#endif // !__BODY_COLLISION_FLAGS_HPP__