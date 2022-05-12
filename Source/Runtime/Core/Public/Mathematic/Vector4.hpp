#ifndef __MATH_VECTOR_4_HPP__
#define __MATH_VECTOR_4_HPP__

#include "Vector3.hpp"

/**
 * A 4-D homogeneous vector.
 */
struct ENGINE_API Vector4
{
	REFLECT(Vector4)

    //=========================[Public Static Properties]=========================//
        
        /** One vector (1,1,1,1) */
        static  Vector4 const   One;

        /** Zero vector (0,0,0,0) */
        static  Vector4 const   Zero;

        /** Positive infinity vector (inf,inf,inf,inf) */
        static  Vector4 const   PositiveInfinity;

        /** Negative infinity vector (-inf,-inf,-inf,-inf) */
        static  Vector4 const   NegativeInfinity;

    //=========================[Public Static Methods]=========================//
                                                      
        static  FORCEINLINE float   Distance        (Vector4 const& p_a,
                                                     Vector4 const& p_b);

        static  FORCEINLINE float   DistanceSquared (Vector4 const& p_a,
                                                     Vector4 const& p_b);

        static  FORCEINLINE float   Dot             (Vector4 const& p_a,
                                                     Vector4 const& p_b);

        static  FORCEINLINE bool    IsNearlyEqual   (Vector4 const& p_a,
                                                     Vector4 const& p_b,
                                                     float const    p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE Vector4 Lerp            (Vector4 const& p_a, 
                                                     Vector4 const& p_b, 
                                                     float const    p_alpha);

        static  FORCEINLINE Vector4 Max             (Vector4 const& p_a,
                                                     Vector4 const& p_b);

        static  FORCEINLINE Vector4 Min             (Vector4 const& p_a,
                                                     Vector4 const& p_b);

        static  FORCEINLINE Vector4 MoveTowards     (Vector4 const& p_current, 
                                                     Vector4 const& p_target, 
                                                     float const    p_maxDistanceDelta);

        static  FORCEINLINE Vector4 Project         (Vector4 const& p_vector,
                                                     Vector4 const& p_onTarget);

        static  FORCEINLINE Vector4 ProjectOnNormal (Vector4 const& p_vector,
                                                     Vector4 const& p_onTargetNormalized);

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

        /** Vector's Z component. */
		PROPERTY()
        float   m_w;

    //=========================[Constructor + Destructor]=========================//
       
        FORCEINLINE Vector4     ();

        FORCEINLINE Vector4     (float const    p_value);

        FORCEINLINE Vector4     (float const    p_x,
                                 float const    p_y, 
                                 float const    p_z = 0.0f,
                                 float const    m_w = 0.0f);

        FORCEINLINE Vector4     (Vector2 const& p_vector2,
                                 float const    p_z = 0.0f,
                                 float const    p_w = 0.0f);

        FORCEINLINE Vector4     (Vector3 const& p_vector3, 
                                 float const    p_w = 0.0f);

        FORCEINLINE Vector4     (Vector4 const& p_from, 
                                 Vector4 const& p_to);

        FORCEINLINE Vector4     (Vector2 const& p_xy,
                                 Vector2 const& p_zw);

        FORCEINLINE Vector4     (Vector4 const& p_copy);

        FORCEINLINE Vector4     (Vector4&&      p_move);

                    ~Vector4    ()   = default;

    //=========================[Public Local Methods]=========================//
        
        FORCEINLINE bool    AllComponentsEquals (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE float   Component           (int32 const    p_index)                                    const;

        FORCEINLINE float&  Component           (int32 const    p_index);

        FORCEINLINE Vector4 GetHomogenized      ()                                                          const;

        FORCEINLINE Vector3 GetHomogenized3D    ()                                                          const;

        FORCEINLINE float   GetMagnitude        ()                                                          const;
                
        FORCEINLINE float   GetMagnitudeSquared ()                                                          const;
                
        FORCEINLINE Vector4 GetSafeNormalized   (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE Vector4 GetUnsafeNormalized ()                                                          const;

        FORCEINLINE void    Homogenize          ();

        FORCEINLINE bool    IsNearlyZero        (float const    p_tolerance = MATH_EPSILON_F)               const;

        FORCEINLINE bool    IsNormalized        ()                                                          const;

        FORCEINLINE bool    IsUnit              (float const    p_lengthSquaredTolerance = MATH_EPSILON_F)  const;

        FORCEINLINE bool    IsZero              ()                                                          const;

        FORCEINLINE bool    Normalize           (float const    p_tolerance = MATH_EPSILON_F);

        FORCEINLINE void    Set                 (float const    p_x, 
                                                 float const    p_y, 
                                                 float const    p_z,
                                                 float const    p_w);
        
        FORCEINLINE void    Set                 (Vector2 const& p_vector2,
                                                 float const    p_z = 0.0f,
                                                 float const    p_w = 0.0f);

        FORCEINLINE void    Set                 (Vector3 const& p_vector3,
                                                 float const    p_w = 0.0f);

        FORCEINLINE void    Set                 (float const    p_value);

    //=========================[Public Local Operators]=========================//
        
        FORCEINLINE bool        operator==  (Vector4 const& p_vector)   const;
        FORCEINLINE bool        operator!=  (Vector4 const& p_vector)   const;

        FORCEINLINE Vector4&    operator=   (Vector4 const& p_copy);
        FORCEINLINE Vector4&    operator=   (Vector3 const& p_copy);
        FORCEINLINE Vector4&    operator=   (Vector2 const& p_copy);

        FORCEINLINE Vector4&    operator=   (Vector4&&      p_move);

        FORCEINLINE float       operator[]  (int32 const    p_index)    const;
        FORCEINLINE float&      operator[]  (int32 const    p_index);

        FORCEINLINE Vector4     operator+   (Vector4 const& p_vector)   const;
        FORCEINLINE Vector4     operator+   (float const    p_value)    const;
        FORCEINLINE Vector4&    operator+=  (Vector4 const& p_vector);
        FORCEINLINE Vector4&    operator+=  (float const    p_value);

        FORCEINLINE Vector4     operator-   ()                          const;
        FORCEINLINE Vector4     operator-   (Vector4 const& p_vector)   const;
        FORCEINLINE Vector4     operator-   (float const    p_value)    const;
        FORCEINLINE Vector4&    operator-=  (Vector4 const& p_vector);
        FORCEINLINE Vector4&    operator-=  (float const    p_value);

        FORCEINLINE Vector4     operator*   (Vector4 const& p_vector)   const;
        FORCEINLINE Vector4     operator*   (float const    p_scale)    const;
        FORCEINLINE Vector4&    operator*=  (Vector4 const& p_vector);
        FORCEINLINE Vector4&    operator*=  (float const    p_scale);

        FORCEINLINE Vector4     operator/   (Vector4 const& p_vector)   const;
        FORCEINLINE Vector4     operator/   (float const    p_scale)    const;
        FORCEINLINE Vector4&    operator/=  (Vector4 const& p_vector);
        FORCEINLINE Vector4&    operator/=  (float const    p_scale);

        FORCEINLINE float       operator|   (Vector4 const& p_vector)   const;

}; // !struct Vector4

/*==============================[Public Globale Operators]==============================*/

    FORCEINLINE Vector4 operator+   (float const    p_value, 
                                     Vector4 const& p_vector);
    
    FORCEINLINE Vector4 operator*   (float const    p_scale,
                                     Vector4 const& p_vector);

#include "Vector4.inl"
#include "Vector4.generated.hpp"

#endif // !__MATH_VECTOR_4_HPP__