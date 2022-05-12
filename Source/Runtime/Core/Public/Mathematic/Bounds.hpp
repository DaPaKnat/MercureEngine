#ifndef __BOUNDS_HPP__
#define __BOUNDS_HPP__

#include "Vector3.hpp"

/**
 * Representation of an Axis-Aligned Bounding Box (AABB).
 */
struct ENGINE_API Bounds
{
    
    // ==============================[Public Static Methods]====================== //

        static Bounds               Overlap (Bounds const&  p_a,
                                             Bounds const&  p_b);

        static FORCEINLINE Bounds   Build   (Vector3 const& p_origin,
                                             Vector3 const& p_extent);

    // ==============================[Public Local Properties]============================== //

        /** ... */
		PROPERTY()
        Vector3 m_max;

        /** ... */
		PROPERTY()
        Vector3 m_min;


    // ==============================[Public Constructor + Destructor]====================== //

        FORCEINLINE Bounds  ();

        FORCEINLINE Bounds  (Vector3 const& p_min,
                             Vector3 const& p_max);

        FORCEINLINE Bounds  (Bounds const&  p_copy);

        FORCEINLINE Bounds  (Bounds&&		p_move);

        ~Bounds ()                                      = default;

    // ==============================[Public Local Methods]====================== //

        FORCEINLINE void    Expand              (float const    p_value);

        FORCEINLINE void    Expand              (Vector3 const& p_value);

        FORCEINLINE void    Expand              (Vector3 const& p_minExp,
                                                 Vector3 const& p_maxExp);

        FORCEINLINE Vector3 GetCenter           ()                              const;
        
        FORCEINLINE void    GetCenterAndExtent  (Vector3&       p_center,
                                                 Vector3&       p_extent)       const;

        FORCEINLINE Vector3 GetClosestPoint     (Vector3 const& p_point)        const;
        
        FORCEINLINE float   GetDistance         (Vector3 const& p_point)		const;

        FORCEINLINE float   GetDistanceSquared  (Vector3 const& p_point)		const;

        FORCEINLINE Bounds  GetExpanded         (float const    p_value)        const;

        FORCEINLINE Bounds  GetExpanded         (Vector3 const& p_value)        const;

        FORCEINLINE Bounds  GetExpanded         (Vector3 const& p_minExp,
                                                 Vector3 const& p_maxExp)       const;

        FORCEINLINE Vector3 GetExtent           ()                              const;

        FORCEINLINE Bounds  GetMoved            (Vector3 const& p_destination)  const;

        FORCEINLINE Vector3 GetSize             ()                              const;

        FORCEINLINE float   GetVolume           ()                              const;

        FORCEINLINE bool    IsInside            (Vector3 const& p_point)        const;

        FORCEINLINE bool    IsInside            (Bounds const& p_other)         const;

        FORCEINLINE bool    IsInsideOrOn        (Vector3 const& p_point)        const;

        FORCEINLINE bool    IsIntersecting      (Bounds const&  p_other)        const;

        FORCEINLINE bool    IsValid             ()                              const;

        FORCEINLINE void    Move                (Vector3 const& p_destination);

        FORCEINLINE void    Shift               (Vector3 const& p_offset);

    // ==============================[Public Local Operators]====================== //

        FORCEINLINE bool    operator==  (Bounds const&  p_other)    const;
        FORCEINLINE bool    operator!=  (Bounds const&  p_other)    const;

        FORCEINLINE Bounds& operator=   (Bounds const&  p_copy);
        FORCEINLINE Bounds& operator=   (Bounds&&       p_move);

}; // !class Bounds

#include "Bounds.inl"

#endif // !__BOUNDS_HPP__