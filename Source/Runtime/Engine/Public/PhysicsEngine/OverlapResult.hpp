#ifndef __OVERLAP_RESULT_HPP__
#define __OVERLAP_RESULT_HPP__

#include "CoreMinimal.hpp"


// ==============================[Forward Declaration]============================== //

class Entity;
class PrimitiveComponent;

// ================================================================================= //

struct ENGINE_API OverlapResult
{
    // ==============================[Public Local Properties]============================== //
        
        /* Entity that the overlap check hit. */
        Entity*             m_actor;

        /* PrimitiveComponent that the overlap check hit. */
        PrimitiveComponent* m_component;
        
        /** Indicates if this hit was requesting a block - if false, was requesting a touch instead */
        bool                m_isBlockingHit;

    // ==============================[Public Constructor + Destructor]============================== //

        OverlapResult   () = default;;

        OverlapResult   (OverlapResult const&   p_copy) = default;

        OverlapResult   (OverlapResult&&        p_move) = default;

        ~OverlapResult  ()  = default;

}; // !class OverlapResult

#endif // !__OVERLAP_RESULT_HPP__
