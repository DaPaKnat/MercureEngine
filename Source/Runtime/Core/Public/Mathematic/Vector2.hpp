#ifndef __MATH_VECTOR_2_HPP__
#define __MATH_VECTOR_2_HPP__

#include <stdexcept>

#include "Utility.hpp"

/**
 * A vector in 2-D space composed of components (X, Y) with floating point precision.
 */
struct ENGINE_API Vector2
{
	REFLECT(Vector2)

    //=========================[Public Static Properties]=========================//

        /** World right vector (1,0) */
        static  Vector2 const   Right;
        
        /** World left vector (-1,0) */
        static  Vector2 const   Left;

        /** World up vector (0,1) */
        static  Vector2 const   Up;
        
        /** World down vector (0,-1) */
        static  Vector2 const   Down;
        
        /** One vector (1,1) */
        static  Vector2 const   One;

        /** Zero vector (0,0) */
        static  Vector2 const   Zero;

        /** Positive infinity vector (inf,inf) */
        static  Vector2 const   PositiveInfinity;

        /** Negative infinity vector (-inf,-inf) */
        static  Vector2 const   NegativeInfinity;

    //=========================[Public Static Methods]=========================//
       
        static  FORCEINLINE float   AngleDeg            (Vector2 const& p_from, 
                                                         Vector2 const& p_to);

        static  FORCEINLINE float   AngleRad            (Vector2 const& p_from,
                                                         Vector2 const& p_to);

        static  FORCEINLINE Vector2 Clamp               (Vector2 const& p_value,
                                                         Vector2 const& p_min,
                                                         Vector2 const& p_max);

        static  FORCEINLINE Vector2 ClampMagnitude      (Vector2 const& p_vector,
                                                         float const    p_minMagnitude,
                                                         float const    p_maxMagnitude);

        static  FORCEINLINE Vector2 ClampMaxMagnitude   (Vector2 const& p_vector, 
                                                         float const    p_maxMagnitude);

        static  FORCEINLINE Vector2 ClampMinMagnitude   (Vector2 const& p_vector, 
                                                         float const    p_minMagnitude);

        static  FORCEINLINE float   Cross               (Vector2 const& p_a, 
                                                         Vector2 const& p_b);

        static  FORCEINLINE float   Distance            (Vector2 const& p_a,
                                                         Vector2 const& p_b);

        static  FORCEINLINE float   DistanceSquared     (Vector2 const& p_a,
                                                         Vector2 const& p_b);

        static  FORCEINLINE float   Dot                 (Vector2 const& p_a,
                                                         Vector2 const& p_b);

        static  FORCEINLINE bool    IsCollinear         (Vector2 const& p_a, 
                                                         Vector2 const& p_b);

        static  FORCEINLINE bool    IsNearlyEqual       (Vector2 const& p_a,
                                                         Vector2 const& p_b,
                                                         float const    p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool    IsOrthogonal        (Vector2 const& p_a, 
                                                         Vector2 const& p_b);

        static  FORCEINLINE Vector2 Lerp                (Vector2 const& p_a, 
                                                         Vector2 const& p_b, 
                                                         float const    p_alpha);

        static  FORCEINLINE Vector2 Max                 (Vector2 const& p_a,
                                                         Vector2 const& p_b);

        static  FORCEINLINE Vector2 Min                 (Vector2 const& p_a,
                                                         Vector2 const& p_b);

        static  FORCEINLINE Vector2 MoveTowards         (Vector2 const& p_current, 
                                                         Vector2 const& p_target, 
                                                         float const    p_maxDistanceDelta);

        static  FORCEINLINE Vector2 Project             (Vector2 const& p_vector,    
                                                         Vector2 const& p_onTarget);

        static  FORCEINLINE Vector2 ProjectOnNormal     (Vector2 const& p_vector,
                                                         Vector2 const& p_onNormal);

        static  FORCEINLINE Vector2 Reflect             (Vector2 const& p_direction,
                                                         Vector2 const& p_normal);

        static  FORCEINLINE Vector2 ReflectInvert       (Vector2 const& p_direction,
                                                         Vector2 const& p_normal);

        static  FORCEINLINE Vector2 RotateDeg           (Vector2 const& p_vector,
                                                         float const    p_angleDeg);

        static  FORCEINLINE Vector2 RotateRad           (Vector2 const& p_vector,
                                                         float const    p_angleRad);

        static  FORCEINLINE float   SignedAngleDeg      (Vector2 const& p_from,
                                                         Vector2 const& p_to);

        static  FORCEINLINE float   SignedAngleRad      (Vector2 const& p_from, 
                                                         Vector2 const& p_to);

    //=========================[Public Local Properties]=========================//
        
        /** Vector's X component. */
		PROPERTY()
        float   m_x;

        /** Vector's Y component. */
		PROPERTY() 
		float   m_y;

    //=========================[Constructor + Destructor]=========================//
        
        FORCEINLINE Vector2     ();

        FORCEINLINE Vector2     (float const    p_value);

        FORCEINLINE Vector2     (float const    p_x,
                                 float const    p_y);

        FORCEINLINE Vector2     (Vector2 const& p_from, 
                                 Vector2 const& p_to);

        FORCEINLINE Vector2     (Vector2 const& p_copy);

        FORCEINLINE Vector2     (Vector2&&      p_move);

                    ~Vector2    ()   = default;

    //=========================[Public Local Methods]=========================//
        
        FORCEINLINE bool        AllComponentsEquals     (float const    p_tolerance = MATH_EPSILON_F)               const;
                
        FORCEINLINE float       Component               (int32 const    p_index)                                    const;

        FORCEINLINE float&      Component               (int32 const    p_index);

        FORCEINLINE Vector2     GetAbs                  ()                                                          const;

        FORCEINLINE float       GetAbsMax               ()                                                          const;

        FORCEINLINE float       GetAbsMin               ()                                                          const;
                        
        FORCEINLINE float       GetMagnitude            ()                                                          const;
                
        FORCEINLINE float       GetMagnitudeSquared     ()                                                          const;
                
        FORCEINLINE float       GetMax                  ()                                                          const;

        FORCEINLINE float       GetMin                  ()                                                          const;

        FORCEINLINE Vector2     GetPerpendicular        ()                                                          const;

        FORCEINLINE Vector2     GetSign                 ()                                                          const;

        FORCEINLINE Vector2     GetSafeNormalized       (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE Vector2     GetUnsafeNormalized     ()                                                          const;

        FORCEINLINE bool        IsNearlyZero            (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE bool        IsNormalized            ()                                                          const;

        FORCEINLINE bool        IsUniform               (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE bool        IsUnit                  (float const    p_lengthSquaredTolerance = MATH_EPSILON_F)  const;

        FORCEINLINE bool        IsZero                  ()                                                          const;

      
        FORCEINLINE bool        Normalize               (float const    p_tolerance = MATH_EPSILON_F);

        FORCEINLINE void        Set                     (float const    p_value);

        FORCEINLINE void        Set                     (float const    p_x, 
                                                         float const    p_y);

        FORCEINLINE void        ToDirectionAndLength    (Vector2&       p_outDirection,
                                                         float&         p_outLength)                                const;

    //=========================[Public Local Operators]=========================//
       
        FORCEINLINE bool        operator==  (Vector2 const& p_vector)   const;
        FORCEINLINE bool        operator!=  (Vector2 const& p_vector)   const;

        FORCEINLINE Vector2&    operator=   (Vector2 const& p_other);
        FORCEINLINE Vector2&    operator=   (Vector2&&      p_move);

        FORCEINLINE float       operator[]  (int32 const    p_index)    const;
        FORCEINLINE float&      operator[]  (int32 const    p_index);

        FORCEINLINE Vector2     operator+   (Vector2 const& p_vector)   const;
        FORCEINLINE Vector2     operator+   (float const    p_value)    const;
        FORCEINLINE Vector2&    operator+=  (Vector2 const& p_vector);
        FORCEINLINE Vector2&    operator+=  (float const    p_value);

        FORCEINLINE Vector2     operator-   ()                          const;
        FORCEINLINE Vector2     operator-   (Vector2 const& p_vector)   const;
        FORCEINLINE Vector2     operator-   (float const    p_value)    const;
        FORCEINLINE Vector2&    operator-=  (Vector2 const& p_vector);
        FORCEINLINE Vector2&    operator-=  (float const    p_value);

        FORCEINLINE Vector2     operator*   (Vector2 const& p_vector)   const;
        FORCEINLINE Vector2     operator*   (float const    p_scale)    const;
        FORCEINLINE Vector2&    operator*=  (Vector2 const& p_vector);
        FORCEINLINE Vector2&    operator*=  (float const    p_scale);

        FORCEINLINE Vector2     operator/   (Vector2 const& p_vector)   const;
        FORCEINLINE Vector2     operator/   (float const    p_scale)    const;
        FORCEINLINE Vector2&    operator/=  (Vector2 const& p_vector);
        FORCEINLINE Vector2&    operator/=  (float const    p_scale);

        FORCEINLINE float       operator^   (Vector2 const& p_vector)   const;
        FORCEINLINE float       operator|   (Vector2 const& p_vector)   const;

}; // !struct Vector2

/*==============================[Public Globale Operators]==============================*/

    FORCEINLINE Vector2 operator+   (float const    p_value, 
                                     Vector2 const& p_vector);
    
    FORCEINLINE Vector2 operator*   (float const    p_scale,
                                     Vector2 const& p_vector);
    
#include "Vector2.inl"
#include "Vector2.generated.hpp"

#endif // !__MATH_VECTOR_2_HPP__
