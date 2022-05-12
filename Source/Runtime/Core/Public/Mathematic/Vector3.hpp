#ifndef __MATH_VECTOR_3_HPP__
#define __MATH_VECTOR_3_HPP__

#include "Vector2.hpp"

/**
 * A vector in 3-D space composed of components (X, Y, Z) with floating point precision.
 */
struct ENGINE_API Vector3
{
	REFLECT(Vector3)

		//=========================[Public Static Properties]=========================//

        /** World right vector (1,0,0) */
        static  Vector3 const   Right;
        
        /** World left vector (-1,0,0) */
        static  Vector3 const   Left;

        /** World up vector (0,1,0) */
        static  Vector3 const   Up;
        
        /** World down vector (0,-1,0) */
        static  Vector3 const   Down;
        
        /** World forward vector (0,0,1) */
        static  Vector3 const   Forward;
       
        /** World backward vector (0,0,-1) */
        static  Vector3 const   Backward;

        /** One vector (1,1,1) */
        static  Vector3 const   One;

        /** Zero vector (0,0,0) */
        static  Vector3 const   Zero;

        /** Positive infinity vector (inf,inf,inf) */
        static  Vector3 const   PositiveInfinity;

        /** Negative infinity vector (-inf,-inf,-inf) */
        static  Vector3 const   NegativeInfinity;

    //=========================[Public Static Methods]=========================//
      
        static  FORCEINLINE float   AngleDeg            (Vector3 const& p_from, 
                                                         Vector3 const& p_to);

        static  FORCEINLINE float   AngleRad            (Vector3 const& p_from,
                                                         Vector3 const& p_to);

        static  FORCEINLINE Vector3 Clamp               (Vector3 const& p_value,
                                                         Vector3 const& p_min,
                                                         Vector3 const& p_max);

        static  FORCEINLINE Vector3 ClampMagnitude      (Vector3 const& p_vector,
                                                         float const    p_minMagnitude,
                                                         float const    p_maxMagnitude);

        static  FORCEINLINE Vector3 ClampMaxMagnitude   (Vector3 const& p_vector, 
                                                         float const    p_maxMagnitude);

        static  FORCEINLINE Vector3 ClampMinMagnitude   (Vector3 const& p_vector, 
                                                         float const    p_minMagnitude);

        static  FORCEINLINE Vector3 Cross               (Vector3 const& p_a, 
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 DegToRad            (Vector3 const& p_degVector);

        static  FORCEINLINE float   Distance            (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE float   Distance2D          (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE float   DistanceSquared     (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE float   DistanceSquared2D   (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE float   Dot                 (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 EulerAngleDeg       (Vector3 const& p_a, 
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 EulerAngleRad       (Vector3 const& p_a, 
                                                         Vector3 const& p_b);

        static  FORCEINLINE bool    IsCollinear         (Vector3 const& p_a, 
                                                         Vector3 const& p_b);

        static  FORCEINLINE bool    IsNearlyEqual       (Vector3 const& p_a,
                                                         Vector3 const& p_b,
                                                         float const    p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool    IsOrthogonal        (Vector3 const& p_a, 
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 Lerp                (Vector3 const& p_a, 
                                                         Vector3 const& p_b, 
                                                         float const    p_alpha);

        static  FORCEINLINE Vector3 Max                 (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 Min                 (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 MoveTowards         (Vector3 const& p_current, 
                                                         Vector3 const& p_target, 
                                                         float const    p_maxDistanceDelta);

        static  FORCEINLINE Vector3 Project             (Vector3 const& p_vector,    
                                                         Vector3 const& p_onTarget);

        static  FORCEINLINE Vector3 ProjectOnNormal     (Vector3 const& p_vector,
                                                         Vector3 const& p_onNormal);

        static  FORCEINLINE Vector3 ProjectOnPlane      (Vector3 const& p_vector,
                                                         Vector3 const& p_planeNormal);

        static  FORCEINLINE Vector3 RadToDeg            (Vector3 const& p_radVector);

        static  FORCEINLINE Vector3 Reflect             (Vector3 const& p_direction,
                                                         Vector3 const& p_normal);

        static  FORCEINLINE Vector3 ReflectInvert       (Vector3 const& p_direction,
                                                         Vector3 const& p_normal);

        static  FORCEINLINE Vector3 SafeScaleReciprocal (Vector3 const& p_scale,
                                                         float const    p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE float   SignedAngleDeg      (Vector3 const& p_from,
                                                         Vector3 const& p_to, 
                                                         Vector3 const& p_axis);

        static  FORCEINLINE float   SignedAngleRad      (Vector3 const& p_from, 
                                                         Vector3 const& p_to, 
                                                         Vector3 const& p_axis);

        static  FORCEINLINE Vector3 SignedEulerAngleDeg (Vector3 const& p_a,
                                                         Vector3 const& p_b);

        static  FORCEINLINE Vector3 SignedEulerAngleRad (Vector3 const& p_a,
                                                         Vector3 const& p_b);                          

    //=========================[Public Local Properties]=========================//
      
        /** Vector's X component. */
		PROPERTY()
        float   m_x;

        /** Vector's Y component. */
		PROPERTY()
        float   m_y;

        /** Vector's Z component. */
		PROPERTY()
        float   m_z;

    //=========================[Constructor + Destructor]=========================//
      
        FORCEINLINE Vector3     ();

        FORCEINLINE Vector3     (float const    p_value);

        FORCEINLINE Vector3     (float const    p_x,
                                 float const    p_y, 
                                 float const    p_z);

        FORCEINLINE Vector3     (Vector3 const& p_from, 
                                 Vector3 const& p_to);

        FORCEINLINE Vector3     (Vector2 const& p_vector2, 
                                 float const    p_z = 0.0f);

        FORCEINLINE Vector3     (Vector3 const& p_copy);

        FORCEINLINE Vector3     (Vector3&&      p_move);

                    ~Vector3    ()   = default;

    //=========================[Public Local Methods]=========================//
        
        FORCEINLINE bool    AllComponentsEquals     (float const    p_tolerance = MATH_EPSILON_F)               const;
                
        FORCEINLINE float   Component               (int32 const    p_index)                                    const;

        FORCEINLINE float&  Component               (int32 const    p_index);

        FORCEINLINE Vector3 GetAbs                  ()                                                          const;

        FORCEINLINE float   GetAbsMax               ()                                                          const;

        FORCEINLINE float   GetAbsMin               ()                                                          const;
                        
        FORCEINLINE float   GetMagnitude            ()                                                          const;
                
        FORCEINLINE float   GetMagnitude2D          ()                                                          const;
                
        FORCEINLINE float   GetMagnitudeSquared     ()                                                          const;
                
        FORCEINLINE float   GetMagnitudeSquared2D   ()                                                          const;

        FORCEINLINE float   GetMax                  ()                                                          const;

        FORCEINLINE float   GetMin                  ()                                                          const;

        FORCEINLINE Vector3 GetSign                 ()                                                          const;

        FORCEINLINE Vector3 GetSafeNormalized       (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE Vector3 GetSafeNormalized2D     (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE Vector3 GetUnsafeNormalized     ()                                                          const;

        FORCEINLINE bool    IsNearlyZero            (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE bool    IsNormalized            ()                                                          const;

        FORCEINLINE bool    IsUniform               (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE bool    IsUnit                  (float const    p_lengthSquaredTolerance = MATH_EPSILON_F)  const;

        FORCEINLINE bool    IsZero                  ()                                                          const;

      
        FORCEINLINE bool    Normalize               (float const    p_tolerance = MATH_EPSILON_F);

        FORCEINLINE void    Set                     (float const    p_x, 
                                                     float const    p_y, 
                                                     float const    p_z);
        
        FORCEINLINE void    Set                     (float const    p_value);

        FORCEINLINE void    ToDirectionAndLength    (Vector3&       p_outDirection,
                                                     float&         p_outLength)                                const;

    //===========================[Public Local Operators]=========================//
       
        FORCEINLINE bool        operator==  (Vector3 const& p_vector)   const;
        FORCEINLINE bool        operator!=  (Vector3 const& p_vector)   const;

        FORCEINLINE Vector3&    operator=   (Vector3 const& p_copy);
        FORCEINLINE Vector3&    operator=   (Vector3&&      p_move);

        FORCEINLINE float       operator[]  (int32 const    p_index)    const;
        FORCEINLINE float&      operator[]  (int32 const    p_index);

        FORCEINLINE Vector3     operator+   (Vector3 const& p_vector)   const;
        FORCEINLINE Vector3     operator+   (float const    p_value)    const;
        FORCEINLINE Vector3&    operator+=  (Vector3 const& p_vector);
        FORCEINLINE Vector3&    operator+=  (float const    p_value);

        FORCEINLINE Vector3     operator-   ()                          const;
        FORCEINLINE Vector3     operator-   (Vector3 const& p_vector)   const;
        FORCEINLINE Vector3     operator-   (float const    p_value)    const;
        FORCEINLINE Vector3&    operator-=  (Vector3 const& p_vector);
        FORCEINLINE Vector3&    operator-=  (float const    p_value);

        FORCEINLINE Vector3     operator*   (Vector3 const& p_vector)   const;
        FORCEINLINE Vector3     operator*   (float const    p_scale)    const;
        FORCEINLINE Vector3&    operator*=  (Vector3 const& p_vector);
        FORCEINLINE Vector3&    operator*=  (float const    p_scale);

        FORCEINLINE Vector3     operator/   (Vector3 const& p_vector)   const;
        FORCEINLINE Vector3     operator/   (float const    p_scale)    const;
        FORCEINLINE Vector3&    operator/=  (Vector3 const& p_vector);
        FORCEINLINE Vector3&    operator/=  (float const    p_scale);

        FORCEINLINE Vector3     operator^   (Vector3 const& p_vector)   const;
        FORCEINLINE float       operator|   (Vector3 const& p_vector)   const;

}; // !struct Vector3

/*==============================[Public Globale Operators]==============================*/

    FORCEINLINE Vector3 operator+   (float const    p_value, 
                                     Vector3 const& p_vector);
    
    FORCEINLINE Vector3 operator*   (float const    p_scale,
                                     Vector3 const& p_vector);
    
#include "Vector3.inl"
#include "Vector3.generated.hpp"

#endif // !__MATH_VECTOR_3_HPP__