#ifndef __QUATERNION_HPP__
#define __QUATERNION_HPP__

#include "Vector3.hpp"

// ==============================[Forward Declaration] ============================== //

    class Matrix4x4;

// ================================================================================== //

/**
 * Floating point quaternion that can represent a rotation about an axis in 3-D space.
 * The X, Y, Z, W components also double as the Axis/Angle format.
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 * Note that this is the opposite order of FTransform multiplication.
 *
 * Example: LocalToWorld = (LocalToWorld * DeltaRotation) will change rotation in local space by DeltaRotation.
 * Example: LocalToWorld = (DeltaRotation * LocalToWorld) will change rotation in world space by DeltaRotation.
 */

struct ENGINE_API Quaternion
{
	REFLECT(Quaternion)

    //=========================[Public Static Properties]=========================//
        
        /** Identity quaternion (0.0f, 0.0f, 0.0f, 1.0f) */
        static  Quaternion const    Identity;

        /** Identity quaternion (0.0f, 0.0f, 0.0f, 0.0f) */
        static  Quaternion const    Zero;

    //=========================[Public Static Methods]=========================//        
       
        static  FORCEINLINE float       AngleDeg            (const Quaternion&  p_a,
                                                             const Quaternion&  p_b);

        static  FORCEINLINE float       AngleRad            (const Quaternion&  p_a,
                                                             const Quaternion&  p_b);
                                                            
        static  FORCEINLINE Quaternion  AxisAngleDeg        (Vector3 const&     p_axis,
                                                             float const        p_angleDeg);

        static  FORCEINLINE Quaternion  AxisAngleRad        (Vector3 const&     p_axis,
                                                             float const        p_angleRad);

        static  FORCEINLINE float       Dot                 (const Quaternion&  p_a,
                                                             const Quaternion&  p_b);

        static  FORCEINLINE Quaternion  FindBetweenNormals  (Vector3 const&     p_fromDirection,
                                                                 Vector3 const&     p_toDirection);

        static  FORCEINLINE Quaternion  FindBetweenVectors  (Vector3 const&     p_fromDirection,
                                                             Vector3 const&     p_toDirection);
        
        static  FORCEINLINE bool        IsNearlyEqual       (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool        IsNearlySame        (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool        IsSame              (const Quaternion&  p_a,
                                                             const Quaternion&  p_b);


        static  FORCEINLINE Quaternion  Lerp                (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_tolerance);

        static  FORCEINLINE Quaternion  MakeFromEulerDeg    (Vector3 const&     p_eulerDegree);

        static  FORCEINLINE Quaternion  MakeFromEulerDeg    (float const        p_xAngleDegree,
                                                             float const        p_yAngleDegree,
                                                             float const        p_zAngleDegree);

        static              Quaternion  MakeFromEulerRad    (Vector3 const&     p_eulerRadian);

        static              Quaternion  MakeFromEulerRad    (float const        p_xAngleRadian,
                                                             float const        p_yAngleRadian,
                                                             float const        p_zAngleRadian);

        static  FORCEINLINE Quaternion  NLerp               (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_tolerance);
        
        static  FORCEINLINE Quaternion  NSlerp              (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_tolerance);

        static  FORCEINLINE Quaternion  RotateTowardsDeg    (const Quaternion&  p_from,
                                                             const Quaternion&  p_to,
                                                             float const        p_maxDegreeDelta);

        static  FORCEINLINE Quaternion  RotateTowardsRad    (const Quaternion&  p_from,
                                                             const Quaternion&  p_to,
                                                             float const        p_maxRadianDelta);

        static  FORCEINLINE Quaternion  Slerp               (const Quaternion&  p_a,
                                                             const Quaternion&  p_b,
                                                             float const        p_alpha);

    //=========================[Public Local Properties]=========================//
      
        /** The quaternion's X-component. */
		PROPERTY()
	    float m_x;

	    /** The quaternion's Y-component. */
		PROPERTY()
	    float m_y;

	    /** The quaternion's Z-component. */
		PROPERTY()
	    float m_z;

	    /** The quaternion's W-component. */
		PROPERTY()
	    float m_w;

    //=========================[Constructor + Destructor]=========================//
       
        FORCEINLINE Quaternion  ();

        FORCEINLINE Quaternion  (float const        p_x,
                                 float const        p_y,
                                 float const        p_z,
                                 float const        p_w);
        
        FORCEINLINE Quaternion  (Vector3 const&     p_axis,
                                 float const        p_angle);

        FORCEINLINE Quaternion  (Matrix4x4 const&   p_matrix);

        FORCEINLINE Quaternion  (const Quaternion&  p_copy);

        FORCEINLINE Quaternion  (Quaternion&&       p_move);


                    ~Quaternion () = default;

    //=========================[Public Local Methods]=========================//
        
        FORCEINLINE void        Conjugate           ();

        FORCEINLINE float       GetAngleDeg         ()                                              const;
        
        FORCEINLINE float       GetAngleRad         ()                                              const;

        FORCEINLINE Vector3     GetAxisX            ()                                              const;

        FORCEINLINE Vector3     GetAxisY            ()                                              const;

        FORCEINLINE Vector3     GetAxisZ            ()                                              const;

        FORCEINLINE Vector3     GetBackward         ()                                              const;

        FORCEINLINE Quaternion  GetConjugate        ()                                              const;

        FORCEINLINE Vector3     GetDown             ()                                              const;

        FORCEINLINE Vector3     GetEulerDeg         ()                                              const;
        
        FORCEINLINE Vector3     GetEulerRad         ()                                              const;

        FORCEINLINE Vector3     GetForward          ()                                              const;

        FORCEINLINE Quaternion  GetInverse          ()                                              const;

        FORCEINLINE Vector3     GetLeft             ()                                              const;

        FORCEINLINE float       GetMagnitude        ()                                              const;

        FORCEINLINE float       GetMagnitudeSquared ()                                              const;

        FORCEINLINE Vector3     GetRight            ()                                              const;

        FORCEINLINE Vector3     GetRotationAxis     ()                                              const;
        
        FORCEINLINE Quaternion  GetSafeNormalized   (float const    p_tolerance = MATH_EPSILON_F)   const;    

        FORCEINLINE Quaternion  GetUnsafeNormalized ()                                              const;

        FORCEINLINE Vector3     GetUp               ()                                              const;

        FORCEINLINE void        Inverse             ();

        FORCEINLINE bool        IsIdentity          (float const    p_tolerance = MATH_EPSILON_F)   const;

        FORCEINLINE bool        IsNormalized        ()                                              const;

        FORCEINLINE bool        IsNearlyZero        (float const    p_tolerance = MATH_EPSILON_F)   const;

        FORCEINLINE bool        IsZero              ()                                              const;

        FORCEINLINE bool        Normalize           (float const    p_tolerance = MATH_EPSILON_F);
        
        FORCEINLINE Vector3     RotateVector        (Vector3 const& p_vector)                       const;

        FORCEINLINE void        Set                 (float const    p_x,
                                                     float const    p_y,
                                                     float const    p_z,
                                                     float const    p_w);

        FORCEINLINE void        ToAxisAndAngleDeg   (Vector3&       p_outAxis,
                                                     float&         p_outAngleDeg)                  const;

        FORCEINLINE void        ToAxisAndAngleRad   (Vector3&       p_outAxis,
                                                     float&         p_outAngleRad)                  const;

        FORCEINLINE Vector3     UnrotateVector      (Vector3 const& p_vector)                       const;
        
    //=========================[Public Local Operators]=========================//

        FORCEINLINE bool        operator==  (const Quaternion&  p_quaternion)   const;
        FORCEINLINE bool        operator!=  (const Quaternion&  p_quaternion)   const;

        FORCEINLINE Quaternion& operator=   (const Quaternion&  p_other);
        FORCEINLINE Quaternion& operator=   (Quaternion&&       p_other);

        FORCEINLINE float       operator[]  (int32 const        p_index)        const;
        FORCEINLINE float&      operator[]  (int32 const        p_index);

        FORCEINLINE Quaternion  operator+   (const Quaternion&  p_quaternion)   const;
        FORCEINLINE Quaternion& operator+=  (const Quaternion&  p_quaternion);

        FORCEINLINE Quaternion  operator-   ()                                  const;
        FORCEINLINE Quaternion  operator-   (const Quaternion&  p_quaternion)   const;
        FORCEINLINE Quaternion& operator-=  (const Quaternion&  p_quaternion);

        FORCEINLINE Quaternion  operator*   (const Quaternion&  p_quaternion)   const;
        FORCEINLINE Quaternion  operator*   (float const        p_scale)        const;
        FORCEINLINE Vector3     operator*   (Vector3 const&     p_vector)       const;
        FORCEINLINE Quaternion& operator*=  (const Quaternion&  p_quaternion);
        FORCEINLINE Quaternion& operator*=  (float const        p_scale);

        FORCEINLINE Quaternion  operator/   (float const        p_scale)        const;  
        FORCEINLINE Quaternion& operator/=  (float const        p_scale);                   

        FORCEINLINE float       operator|   (const Quaternion&  p_quaternion)   const;

}; // !struct Quaternion

/*==============================[Public Globale Operators]==============================*/
    
    FORCEINLINE Quaternion  operator*   (float const        p_scale,
                                         const Quaternion&  p_quaternion);

#include "Quaternion.inl"
#include "Quaternion.generated.hpp"

#endif // !__QUATERNION_HPP__