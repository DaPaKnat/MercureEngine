#ifndef __OVERLAP_INFO_HPP__
#define __OVERLAP_INFO_HPP__

#include "CoreMinimal.hpp"

#include "HitResult.hpp"

/**
 * Description : TODO
 */
struct ENGINE_API OverlapInfo
{
	REFLECT(OverlapInfo)
    // ==============================[Public Local Properties]============================== //

        /** ... */
		PROPERTY()
        HitResult   m_overlapInfo;

        /** ... */
		PROPERTY()
        bool        m_fromSweep;

     // ==============================[Public Constructor + Destructor]============================== //

        OverlapInfo     () = default;

        OverlapInfo     (HitResult const&           p_hitResult,
                         bool const                 p_fromSweep);

        OverlapInfo     (PrimitiveComponent* const  p_component,
                         bool const                 p_fromSweep = false);

        OverlapInfo     (OverlapInfo const& p_copy);

        OverlapInfo     (OverlapInfo&&      p_move);

        ~OverlapInfo    () = default;

     // ==============================[Public Local Operators]============================== //

        bool            operator==  (OverlapInfo const& p_other)    const;

        bool            operator!=  (OverlapInfo const& p_other)    const;


        OverlapInfo&    operator=   (OverlapInfo const& p_copy);

        OverlapInfo&    operator=   (OverlapInfo&&      p_move);

}; // !struct OverlapInfo

#endif // __OVERLAP_INFO_HPP__