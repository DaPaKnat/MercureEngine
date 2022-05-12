#ifndef __FIND_FLOOR_RESULT_HPP__
#define __FIND_FLOOR_RESULT_HPP__

#include "CoreMinimal.hpp"

#include "PhysicsEngine/HitResult.hpp"

struct ENGINE_API FindFloorResult
{
    REFLECT(FindFloorResult)

    // ==============================[Public Local Properties]============================== //

        PROPERTY()
        bool        m_isBlockingHit     = false;
    
        PROPERTY()
        bool        m_isWalkableFloor   = false;

        PROPERTY()
        bool        m_lineTrace         = false;

        PROPERTY()
        float       m_floorDistance     = 0.0f;

        PROPERTY()
        float       m_lineDistance      = 0.0f;

        PROPERTY()
        HitResult   m_hitResult;


    // ==============================[Public Constructor + Destructor]============================== //

        FindFloorResult     () = default;

        FindFloorResult     (FindFloorResult const& p_copy) = default;

        FindFloorResult     (FindFloorResult&&      p_move) = default;

        ~FindFloorResult    () = default;

    // ==============================[Public Local Methods]============================== //

        void    Clear               ();

        float   GetDistanceToFloor  ()  const;

        bool    IsWalkableFloor     ()  const;

        void    SetFromLineTrace    (HitResult const&   p_inHit,
                                     float const        p_inSweepFloorDistance,
                                     float const        p_inLineDistance,
                                     bool const         p_isWalkableFloor);

        void    SetFromSweep        (HitResult const&   p_inHit,
                                     float const        p_inSweepFloorDistance,
                                     bool const         p_isWalkableFloor);

    // ==============================[Public Local Operators]============================== //

        FindFloorResult&    operator=   (FindFloorResult const& p_copy) = default;
        FindFloorResult&    operator=   (FindFloorResult&&      p_move) = default;

}; // !struct FindFloorResult

#include "FindFloorResult.generated.hpp"

#endif // !__FIND_FLOOR_RESULT_HPP__