#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include "Matrix4x4.hpp"

/**
 * Transform is composed of Translation, Rotation (as a Quaternion) and a Scale
 */
class ENGINE_API Transform
{
	REFLECT(Transform)

    public:

    // ==============================[Public Static Properties]============================== //
    
        /** 
         * Identity Transform
         *
         * Translation  {0.0f, 0.0f, 0.0f)
         * Rotation     {0.0f, 0.0f, 0.0f, 1.0f)
         * scale        {1.0f, 1.0f, 1.0f)
         */
        static  Transform const Identity;

    // ==============================[Public Static Methods]============================== //

        static  FORCEINLINE Transform   Blend           (Transform const&   p_a,
                                                         Transform const&   p_b,
                                                         float const        p_alpha);

        static  FORCEINLINE bool        IsNearlyEqual   (Transform const&   p_a,
                                                         Transform const&   p_b,
                                                         float const        p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool        IsNearlySame    (Transform const&   p_a,
                                                         Transform const&   p_b,
                                                         float const        p_tolerance = MATH_EPSILON_F);

        static  FORCEINLINE bool        IsSame          (Transform const&   p_a,
                                                         Transform const&   p_b);

        static              Transform   GetRelative     (Transform const&   p_referential,
                                                         Transform const&   p_transform);

    // ==============================[Protected Local Properties]============================== //
        
        /** Rotation of this transform */
		PROPERTY()
        Quaternion  m_rotation;

        /** Translation of this transform */
		PROPERTY()
        Vector3     m_translation;

        /** Scale of this transform (Applied in Local) */
		PROPERTY()
        Vector3     m_scale;

    // ==============================[Constructor + Destructor]============================== //

        FORCEINLINE Transform   ();

        FORCEINLINE Transform   (Quaternion const&  p_rotation);

        FORCEINLINE Transform   (Vector3 const&     p_translation);

        FORCEINLINE Transform   (Vector3 const&     p_translation,
                                 Quaternion const&  p_rotation,
                                 Vector3 const&     p_scale = Vector3::One);

        FORCEINLINE Transform   (const Matrix4x4&   p_transformMatrix);

        FORCEINLINE Transform   (Transform const&   p_copy);

        FORCEINLINE Transform   (Transform&&        p_move);

                    ~Transform  () = default;

    // ==============================[Public Local Methods]============================== //

        FORCEINLINE void        BlendWith                       (Transform const&   p_other,
                                                                 float const        p_alpha);

        FORCEINLINE float       GetDeterminant                  ()                                  const;


        FORCEINLINE Vector3     GetBackward                     ()                                  const;

        FORCEINLINE Vector3     GetDown                         ()                                  const;

        FORCEINLINE Vector3     GetForward                      ()                                  const;

        FORCEINLINE Transform   GetInverse                      ()                                  const;

        FORCEINLINE Vector3     GetLeft                         ()                                  const;

        FORCEINLINE Matrix4x4   GetMatrix                       ()                                  const;

        FORCEINLINE Matrix4x4   GetMatrixInverse                ()                                  const;

        FORCEINLINE Matrix4x4   GetMatrixInverseNoScale         ()                                  const;
        
        FORCEINLINE Matrix4x4   GetMatrixNoScale                ()                                  const;

        FORCEINLINE Vector3     GetRight                        ()                                  const;

        FORCEINLINE Transform   GetScaled                       (Vector3 const&     p_scale)        const;

        FORCEINLINE Transform   GetScaled                       (float const        p_scale)        const;
        
        FORCEINLINE Vector3     GetScaledBackward               ()                                  const;

        FORCEINLINE Vector3     GetScaledDown                   ()                                  const;

        FORCEINLINE Vector3     GetScaledForward                ()                                  const;

        FORCEINLINE Vector3     GetScaledLeft                   ()                                  const;
        
        FORCEINLINE Vector3     GetScaledRight                  ()                                  const;

        FORCEINLINE Vector3     GetScaledUp                     ()                                  const;

        FORCEINLINE Transform   GetTranslated                   (Vector3 const&     p_translation)  const;

        FORCEINLINE Vector3     GetUp                           ()                                  const;

        FORCEINLINE bool        HasScaleWithNegativeValue       ()                                  const;

        FORCEINLINE void        Inverse                         ();

        FORCEINLINE Vector3     InverseTransformPoint           (Vector3 const&     p_position)     const;

        FORCEINLINE Vector3     InverseTransformPointNoScale    (Vector3 const&     p_position)     const;

        FORCEINLINE Quaternion  InverseTransformRotation        (Quaternion const&  p_rotation)     const;

        FORCEINLINE Vector3     InverseTransformVector          (Vector3 const&     p_direction)    const;

        FORCEINLINE Vector3     InverseTransformVectorNoScale   (Vector3 const&     p_direction)    const;
        
        FORCEINLINE void        Set                             (Vector3 const&     p_translation,
                                                                 Quaternion const&  p_rotation,
                                                                 Vector3 const&     p_scale);

        FORCEINLINE void        Set                             (Matrix4x4 const&   p_matrix4x4);

        FORCEINLINE void        SetIdentity                     ();

        FORCEINLINE Vector3     TransformPoint                  (Vector3 const&     p_point)        const;

        FORCEINLINE Vector3     TransformPointNoScale           (Vector3 const&     p_point)                                  const;

        FORCEINLINE Quaternion  TransformRotation               (Quaternion const&  p_rotation)     const;

        FORCEINLINE Vector3     TransformVector                 (Vector3 const&     p_vector)       const;

        FORCEINLINE Vector3     TransformVectorNoScale          (Vector3 const&     p_vector)       const;

        

    // ==============================[Public Local Operators]============================== //

        FORCEINLINE bool        operator==  (Transform const&   p_transform)    const;
        FORCEINLINE bool        operator!=  (Transform const&   p_transform)    const;

        FORCEINLINE Transform&  operator=   (Transform const&   p_copy);
        FORCEINLINE Transform&  operator=   (Transform&&        p_move);

        FORCEINLINE Transform   operator*   (Transform const&   p_transform)    const;
        FORCEINLINE Transform&  operator*=  (Transform const&   p_transform);

    private:

    // ==============================[Private Static Methods]============================== //

    
        FORCEINLINE static Transform   ConstructTransformFromMatrixWithDesiredScale (Matrix4x4 const&   p_a, 
                                                                                     Matrix4x4 const&   p_b,
                                                                                     Vector3 const&     p_desiredScale);

        
        FORCEINLINE static Transform   GetRelativeUsingMatrixWithScale              (Transform const&   p_referential,
                                                                                     Transform const&   p_transform);

        FORCEINLINE static Transform   MultiplyUsingMatrixWithScale                 (Transform const&   p_a,
                                                                                     Transform const&   p_b);                                                                     


}; // !class Transform

#include "Transform.inl"
#include "Transform.generated.hpp"

#endif // !__TRANSFORM_HPP__