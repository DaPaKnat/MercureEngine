#ifndef __TRANSFORM_INL__
#define __TRANSFORM_INL__

/*==============================[Public Static Methods]==============================*/

/**
 * Computes a transform with the weighted blend of the supplied two transforms.
 *
 * @param p_a       The transform corresponding to the start of the blending.
 * @param p_b       The transform corresponding to the end of the blending.
 * @param p_alpha   The weight value of the blending.
 *
 * @return          p_a (If p_alpha <= 0.0f)
 * @return          p_a (If p_alpha <= 0.0f)
 * @return          The blending of p_a and p_b by p_alpha.
 */
FORCEINLINE Transform   Transform::Blend            (Transform const&   p_a,
                                                     Transform const&   p_b,
                                                     float const        p_alpha)
{
    if      (p_alpha <= MATH_EPSILON_F)
        return p_a;

    else if (p_alpha >= 1.0f - MATH_EPSILON_F)
        return p_b;

    else
    {
        return Transform
        (
            Vector3::Lerp(p_a.m_translation, p_b.m_translation, p_alpha),
            Quaternion::NSlerp(p_a.m_rotation, p_b.m_rotation, p_alpha),
            Vector3::Lerp(p_a.m_scale, p_b.m_scale, p_alpha)
        );
    }
}

/**
 * Check if two Transforms are nearly equal within a specified tolerance.
 * (Both quaternion can represent the same rotation but be not Equal, 
 *  this function check if the components are nearly equal).
 * 
 * @param p_a           The first transform to check against.
 * @param p_b           The second transform to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return True if both transform are nearly equal, otherwise false.
 */
FORCEINLINE bool        Transform::IsNearlyEqual    (Transform const&   p_a,
                                                     Transform const&   p_b,
                                                     float const        p_tolerance)
{
    return  Vector3::IsNearlyEqual(p_a.m_translation, p_b.m_translation)    &&
            Quaternion::IsNearlyEqual(p_a.m_rotation, p_b.m_rotation)       &&
            Vector3::IsNearlyEqual(p_a.m_scale, p_b.m_scale);
}

/**
 * Check if two Transforms are nearly same within a specified tolerance.
 * (Checks if the quaternions represent the same rotation) 
 * 
 * @param p_a           The first transform to check against.
 * @param p_b           The second transform to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return True if both transform are nearly same, otherwise false.
 */
FORCEINLINE bool        Transform::IsNearlySame     (Transform const&   p_a,
                                                     Transform const&   p_b,
                                                     float const        p_tolerance)
{
    return  Vector3::IsNearlyEqual(p_a.m_translation, p_b.m_translation)    &&
            Quaternion::IsNearlySame(p_a.m_rotation, p_b.m_rotation)        &&
            Vector3::IsNearlyEqual(p_a.m_scale, p_b.m_scale);
}

/**
 * Check if two Transforms represent the exactly the same transformation
 * (Checks if the quaternions represent the same rotation) 
 * 
 * @param p_a           The first transform to check against.
 * @param p_b           The second transform to check against.
 *
 * @return True if both transform represent the same transformation, otherwise false.
 */
FORCEINLINE bool        Transform::IsSame           (Transform const&   p_a,
                                                     Transform const&   p_b)
{
    return  (p_a.m_translation == p_b.m_translation)                    &&
            Quaternion::IsNearlySame(p_a.m_rotation, p_b.m_rotation)    &&
            (p_a.m_scale == p_b.m_scale);   
}

/*==============================[Constructors + Destructor]==============================*/

/** 
 * Default constructor 
 *
 * The Rotation is set as Quaternion::Identity
 * The Translation is set as Vector3::Zero
 * The Scale is set as Vector3::One
 */
FORCEINLINE Transform::Transform    ()
:   m_rotation      {Quaternion::Identity},
    m_translation   {Vector3::Zero},
    m_scale         {Vector3::One}
{}

/** 
 * Constructor with inital rotation.
 * The Translation is set as Vector3::Zero
 * The Scale is set as Vector3::One
 *
 * @param p_rotation    The value to use for the rotation component.
 */
FORCEINLINE Transform::Transform    (Quaternion const&  p_rotation)
:   m_rotation      {p_rotation},
    m_translation   {Vector3::Zero},
    m_scale         {Vector3::One}
{}

/** 
 * Constructor with inital translation.
 * The Rotation is set as Quaternion::Identity
 * The Scale is set as Vector3::One
 *
 * @param p_translation The value to use for the translation component.
 */
FORCEINLINE Transform::Transform    (Vector3 const&     p_translation)
:   m_rotation      {Quaternion::Identity},
    m_translation   {p_translation},
    m_scale         {Vector3::One}
{}

/**
 * Constructor with all component initialized.
 *
 * @param p_rotation    The value to use for the rotation component.
 * @param p_translation The value to use for the translation component.
 * @param p_scale       The value to use for the scale component.
 */
FORCEINLINE Transform::Transform    (Vector3 const&     p_translation,
                                     Quaternion const&  p_rotation,
                                     Vector3 const&     p_scale)
:   m_rotation      {p_rotation},
    m_translation   {p_translation},
    m_scale         {p_scale}
{}

/**
 * Constructor for converting a Matrix into a Transform.
 */
FORCEINLINE Transform::Transform    (Matrix4x4 const&   p_matrix4x4)
{
    Set(p_matrix4x4);
}

/**
 * Copy constructor using initial Transform for each component.
 *
 * @param p_copy    The initial transform to copy.
 */
FORCEINLINE Transform::Transform    (Transform const&   p_copy)
:   m_rotation      {p_copy.m_rotation},
    m_translation   {p_copy.m_translation},
    m_scale         {p_copy.m_scale}
{}

/**
 * Move constructor using initial Transform for each component.
 *
 * @param p_move    The initial transform to move.
 */
FORCEINLINE Transform::Transform    (Transform&&        p_move)
:   m_rotation      {std::move(p_move.m_rotation)},
    m_translation   {std::move(p_move.m_translation)},
    m_scale         {std::move(p_move.m_scale)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 * Blend this transform with and another transform.
 *
 * @param p_other   The transform corresponding to the end of the blending.
 * @param p_alpha   The weight value of the blending.
 *
 */
FORCEINLINE void        Transform::BlendWith                        (Transform const&   p_other,
                                                                     float const        p_alpha)
{
    if (p_alpha <= MATH_EPSILON_F)
    {
        if (p_alpha >= 1.0f - MATH_EPSILON_F)
        {
            *this = p_other;
        }
    }
    else
    {
        m_rotation      = Quaternion::NSlerp(m_rotation, p_other.m_rotation, p_alpha);
        m_translation   = Vector3::Lerp(m_translation, p_other.m_translation, p_alpha);
        m_scale         = Vector3::Lerp(m_scale, p_other.m_scale, p_alpha);
    }
}

/** Get the Backward direction (-Z axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetBackward                      ()                                  const
{
    return  m_rotation.GetBackward();
}

/**
 * Computes the determinant of this transform.
 *
 * @return The determinant of this.
 */
FORCEINLINE float       Transform::GetDeterminant                   ()                                  const
{
    return m_scale.m_x * m_scale.m_y * m_scale.m_z;
}

/** Get the Down direction (-Y axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetDown                          ()                                  const
{
    return m_rotation.GetDown();
}

/** Get the Forward direction (+Z axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetForward                       ()                                  const
{
    return m_rotation.GetForward();
}

/**
 * Compute the inverse of this transform.
 *
 * @return The inverse of this.
 */
FORCEINLINE Transform   Transform::GetInverse                       ()                                  const
{
    Quaternion const    inverseRotation     {m_rotation.GetInverse()};
    Vector3 const       inverseScale        {Vector3::SafeScaleReciprocal(m_scale)};

    return Transform
    (
        inverseRotation * (inverseScale * -m_translation),
        inverseRotation,
        inverseScale
    );
}

/** Get the Left direction (-X axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetLeft                          ()                                  const
{
    return m_rotation.GetLeft();
}

/**
 * Computes the matrix corresponding to this transform.
 *
 * @return The matrix corresponding to this.
 */
FORCEINLINE Matrix4x4   Transform::GetMatrix                        ()                                  const
{
    return Matrix4x4::TRS(m_translation, m_rotation, m_scale);
}

/**
 * Computes the matrix corresponding to the inverse of this transform.
 *
 * @return The matrix corresponding to the inverse of this.
 */
FORCEINLINE Matrix4x4   Transform::GetMatrixInverse                 ()                                  const
{
    return GetMatrix().GetInverse();
}

/**
 * Computes the matrix corresponding to the inverse of this transform (Don't use the scale value).
 *
 * @return The matrix corresponding to the inverse of this.
 */
FORCEINLINE Matrix4x4   Transform::GetMatrixInverseNoScale          ()                                  const
{   
    return GetMatrixNoScale().GetInverse();
}

/**
 * Computes the matrix corresponding to this transform (Don't use the scale value).
 *
 * @return The matrix corresponding to this.
 */
FORCEINLINE Matrix4x4   Transform::GetMatrixNoScale                 ()                                  const
{
    return Matrix4x4::TRS(m_translation,m_rotation, Vector3::One);
}

/** Get the Right direction (+X axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetRight                         ()                                  const
{
    return m_rotation.GetRight();
}

/**
 * Computes this transform scaled by the scale vector given.
 *
 * @param p_scale   The scale vector.
 *
 * @return          This transform scaled by p_scale.
 */
FORCEINLINE Transform   Transform::GetScaled                        (Vector3 const&     p_scale)        const
{
    return Transform
    (
        m_translation,
        m_rotation,
        m_scale * p_scale
    );
}

/**
 * Computes this transform scaled by the value given.
 *
 * @param p_scale   The scale value.
 *
 * @return          This transform scaled by p_scale.
 */
FORCEINLINE Transform   Transform::GetScaled                        (float const        p_scale)        const
{
    return Transform
    (
        m_translation,
        m_rotation,
        m_scale * p_scale
    );
}

/**
 * Computes this transform translated with the value given.
 *
 * @param p_translation The vector translation to use.
 *
 * @return              This transform translated by p_translation.
 */
FORCEINLINE Transform   Transform::GetTranslated                    (Vector3 const&     p_translation)  const
{
    return Transform
    (
        m_translation + p_translation,
        m_rotation,
        m_scale
    );
}

/** Get the Backward direction (-Z axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledBackward                ()                                  const
{
    return m_rotation * (Vector3::Backward * m_scale);
}

/** Get the Down direction (-Y axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledDown                    ()                                  const
{
    return m_rotation * (Vector3::Down * m_scale);
}

/** Get the Forward direction (+Z axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledForward                 ()                                  const
{
    return m_rotation * (Vector3::Forward * m_scale);
}

/** Get the Left direction (-X axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledLeft                    ()                                  const
{
    return m_rotation * (Vector3::Left* m_scale);
}

/** Get the Right direction (+X axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledRight                   ()                                  const
{
    return m_rotation * (Vector3::Right* m_scale);
}

/** Get the Up direction (+Y axis) after it has been transform by this (With scaled). */
FORCEINLINE Vector3     Transform::GetScaledUp                      ()                                  const
{
    return m_rotation * (Vector3::Up * m_scale);
    return GetBackward() * m_scale;
}

/** Get the Up direction (+Y axis) after it has been transform by this (Without scaled). */
FORCEINLINE Vector3     Transform::GetUp                            ()                                  const
{
    return m_rotation.GetUp();
}

/**
 * Checks if one of the scale's component is negative.
 *
 * @return True, if one component is negative, false is they are all positives.
 */
FORCEINLINE bool        Transform::HasScaleWithNegativeValue        ()                                  const
{
    return m_scale.m_x < 0.0f || m_scale.m_y < 0.0f || m_scale.m_z < 0.0f;
}

/** Inverse this.*/
FORCEINLINE void        Transform::Inverse                          ()
{
    m_rotation      = m_rotation.GetInverse();
    m_scale         = Vector3::SafeScaleReciprocal(m_scale);
    m_translation   = m_rotation * (m_scale * -m_translation);
}

/**
 * Transform a vector corresponding to a position/point by the inverse of this transform.
 * (Apply translation, rotation and scale)
 *
 * @param p_point   The point to transform.
 *
 * @return          The point after application of the inverse of this transformation.
 */
FORCEINLINE Vector3     Transform::InverseTransformPoint            (Vector3 const&     p_point)        const
{
	return m_rotation.UnrotateVector(p_point - m_translation) * Vector3::SafeScaleReciprocal(m_scale);
}

/**
 * Transform a vector corresponding to a position/point by the inverse of this transform.
 * (Apply translation and rotation)
 *
 * @param p_point   The point to transform.
 *
 * @return          The point after application of the inverse of this transformation.
 */
FORCEINLINE Vector3     Transform::InverseTransformPointNoScale     (Vector3 const&     p_point)        const
{
	return m_rotation.UnrotateVector(p_point - m_translation);
}


/**
 * Transform a quaternion corresponding to a rotation by the inverse of this transform.
 * (Apply rotation)
 *
 * @param p_rotation    The rotation to transform.
 *
 * @return              The rotation after application of the inverse of this transformation.
 */
FORCEINLINE Quaternion  Transform::InverseTransformRotation         (Quaternion const&  p_rotation)     const
{
    return m_rotation.GetInverse() * p_rotation;
}

/**
 * Transform a vector corresponding to a direction by the inverse of this transform.
 * (Apply rotation and scale)
 *
 * @param p_direction   The direction to transform.
 *
 * @return              The direction after application of the inverse of this transformation.
 */
FORCEINLINE Vector3     Transform::InverseTransformVector           (Vector3 const&     p_vector)       const
{
    return m_rotation.UnrotateVector(p_vector) * Vector3::SafeScaleReciprocal(m_scale);
}

/**
 * Transform a vector corresponding to a direction by the inverse of this transform.
 * (Apply rotation)
 *
 * @param p_direction   The direction to transform.
 *
 * @return              The direction after application of the inverse of this transformation.
 */
FORCEINLINE Vector3     Transform::InverseTransformVectorNoScale    (Vector3 const&     p_vector)       const
{
    return m_rotation.UnrotateVector(p_vector);
}

/**
 * Set the components of this transform
 *
 * @param p_translation The new translation;
 * @param p_rotation    The new rotation;
 * @param p_scale       The new scale;
 */
FORCEINLINE void        Transform::Set                              (Vector3 const&     p_translation,
                                                                     Quaternion const&  p_rotation,
                                                                     Vector3 const&     p_scale)
{
    m_rotation      = p_rotation;
    m_translation   = p_translation;
    m_scale         = p_scale;
}

/**
 * Set this transform from a Matrix.
 *
 * @param p_matrix4x4 The matrix use to set the component of this transform.
 */
FORCEINLINE void        Transform::Set                              (Matrix4x4 const&   p_matrix4x4)
{
    Matrix4x4 matrix    {p_matrix4x4};

    // Get scale from the matrix 
    m_scale         = matrix.ExtractScale();
    m_rotation      = matrix.GetRotation();
    m_translation   = matrix.GetOrigin();

    m_rotation.Normalize();
}

/**
 * Sets the component of this transform to the identity transform
 *
 * Translation  = {0.0f, 0.0f, 0.0f}
 * Rotation     = {0.0f, 0.0f, 0.0f, 1.0f}
 * Scale        = {1.0f, 1.0f, 1.0f}
 *
 */
FORCEINLINE void        Transform::SetIdentity                      ()
{
    m_rotation      = Quaternion::Identity;
    m_translation   = Vector3::Zero;
    m_scale         = Vector3::One;
}

/**
 * Transform a vector corresponding to a position/point.
 * (Apply translation, rotation and scale)
 *
 * @param p_point   The point to transform.
 *
 * @return          The point after application of this transformation.
 */
FORCEINLINE Vector3     Transform::TransformPoint                   (Vector3 const&     p_point)        const
{
    return m_rotation.RotateVector(m_scale*p_point) + m_translation;
}

/**
 * Transform a vector corresponding to a position/point.
 * (Apply translation and rotation)
 *
 * @param p_point   The point to transform.
 *
 * @return          The point after application of this transformation.
 */
FORCEINLINE Vector3     Transform::TransformPointNoScale            (Vector3 const&     p_point)        const
{
    return m_rotation.RotateVector(p_point) + m_translation;
}

/**
 * Transform a quaternion corresponding to a rotation.
 * (Apply rotation)
 *
 * @param p_rotation    The rotation to transform.
 *
 * @return              The rotation after application of this transformation.
 */
FORCEINLINE Quaternion  Transform::TransformRotation                (Quaternion const&  p_rotation)     const
{
    return m_rotation * p_rotation;
}

/**
 * Transform a vector corresponding to a direction.
 * (Apply rotation and scale)
 *
 * @param p_direction   The direction to transform.
 *
 * @return              The direction after application of this transformation.
 */
FORCEINLINE Vector3     Transform::TransformVector                  (Vector3 const&     p_vector)       const
{
    return m_rotation.RotateVector(p_vector*m_scale);
}

/**
 * Transform a vector corresponding to a direction.
 * (Apply rotation)
 *
 * @param p_direction   The direction to transform.
 *
 * @return              The direction after application of this transformation.
 */
FORCEINLINE Vector3     Transform::TransformVectorNoScale           (Vector3 const&     p_vector)       const
{
    return m_rotation.RotateVector(p_vector);
}

/*==============================[Public Local Operators]==============================*/

/**
 * Check against another Transform for equality.
 *
 * @param p_tranform    The transform to check against.
 *
 * @return              True    if the transforms are equal.
 * @return              False   if the transforms are different.
 */
FORCEINLINE bool        Transform::operator==   (Transform const&   p_transform)    const
{
    return  m_rotation      == p_transform.m_rotation       &&
            m_translation   == p_transform.m_translation    &&
            m_scale         == p_transform.m_scale;
}

/**
 * Check against another transform for inequality.
 *
 * @param p_transform   The transform to check against.
 *
 * @return              True    if the transforms are different.
 * @return              False   if the transforms are equal.
 */
FORCEINLINE bool        Transform::operator!=   (Transform const&   p_transform)    const
{
    return  m_rotation      != p_transform.m_rotation       ||
            m_translation   != p_transform.m_translation    ||
            m_scale         != p_transform.m_scale;   
}

/**
 * Copy another transform into this one
 *
 * @param p_copy    The other transform to copy.
 *
 * @return          The reference to transform after copy.
 */
FORCEINLINE Transform&  Transform::operator=    (Transform const&   p_copy)
{
    m_rotation      = p_copy.m_rotation;
    m_translation   = p_copy.m_translation;
    m_scale         = p_copy.m_scale;

    return *this;
}

/**
 * Move another Transform into this one
 *
 * @param p_vector  The Transform to move.
 *
 * @return          The reference to transform after move.
 */
FORCEINLINE Transform&  Transform::operator=    (Transform&&        p_move)
{
    m_rotation      = std::move(p_move.m_rotation);
    m_translation   = std::move(p_move.m_translation);
    m_scale         = std::move(p_move.m_scale);

    return *this;
}

/**
 * Computes the result of the multiplying this by an another transorm
 *
 * @param p_transform   The transform to multiply this by/
 *
 * @return              The result of multiplication.
 */
FORCEINLINE Transform   Transform::operator*    (Transform const&   p_transform)    const
{
    /* Here Q = Quaternion as rotation, S = Scale and T = Translation
     *
     * A is the parent transform and B the children.
     * TQS(A) = T(A), Q(A), S(A) and TQS(A) = T(B), Q(B), S(B)
     *
     * Inverse Q(A) = Q'(A)
     *
     * TQS(A*B) :
     *
     * TQS(B)   = T(B) + Q(B) * S(B) * vec * Q'(B)
     * TQS(A*B) = T(A) + Q(A) * S(A) * TQS(B) * Q'(A)
     * TQS(A*B) = T(A) + Q(A) * S(A) * [T(B) + Q(B) * S(B) * vec * Q'(B)]) * Q'(A)
     * TQS(A*B) = T(A) + [Q(A) * S(A) * T(B) * Q'(A)] + [Q(A) * S(A) * Q(B) * S(B) * vec * Q'(B) * Q'(A)]
     * TQS(A*B) = T(A) + [Q(A) * S(A) * T(B) * Q'(A)] + [Q(A) * Q(B)] * [S(A) * S(B)] * vec * [Q'(B) * Q'(A)]
     * TQS(A*B) = T(A) + [Q(A) * S(A) * T(B) * Q'(A)] + [Q(A) * Q(B)] * [S(A) * S(B)] * vec * '[Q(A) * Q(B)]
     *
     * T(A*B)   = T(A) + Q(A) * S(A) * T(B) * Q'(A)
     * Q(A*B)   = Q(A) * Q(B)
     * S(A*B)   = S(A) * S(B)
     *
     */
    
    if (HasScaleWithNegativeValue() || p_transform.HasScaleWithNegativeValue())
    {
        return MultiplyUsingMatrixWithScale(GetMatrix(), p_transform.GetMatrix());
    }

    else
    {
        Quaternion const    newRot{ m_rotation * p_transform.m_rotation };
        Vector3 const       newScale{ m_scale *  p_transform.m_scale }; // Wrong
        Vector3 const       newPosition{ m_rotation * (m_scale * p_transform.m_translation) + m_translation };

        return Transform(newPosition, newRot, newScale);
    }
}

/**
 * Multiply this by an another transform
 *
 * @param p_transform   The transform to multiply this by/
 *
 * @return              The reference to this after multiplication.
 */
FORCEINLINE Transform&  Transform::operator*=   (Transform const&   p_transform)
{
    return (*this = std::move(*this * p_transform));
}

// ==============================[Private Static Methods]============================== //

FORCEINLINE Transform   Transform::ConstructTransformFromMatrixWithDesiredScale (Matrix4x4 const&   p_a, 
                                                                                 Matrix4x4 const&   p_b,
                                                                                 Vector3 const&     p_desiredScale)
{
    Matrix4x4 matrix    {p_a * p_b};
    matrix.RemoveScaling();

    Vector3 signedScale {p_desiredScale.GetSign()};

    matrix.SetAxisX(signedScale.m_x * matrix.GetScaledRight());
    matrix.SetAxisY(signedScale.m_y * matrix.GetScaledUp());
    matrix.SetAxisZ(signedScale.m_z * matrix.GetScaledForward());

    Quaternion rotation {matrix.GetRotation()};
    rotation.Normalize();

    return Transform(matrix.GetOrigin(), rotation, p_desiredScale);

}


FORCEINLINE Transform   Transform::GetRelativeUsingMatrixWithScale              (Transform const&   p_referential,
                                                                                 Transform const&   p_transform)
{
    Matrix4x4 const referential     {p_referential.GetMatrix()};
    Matrix4x4 const transform       {p_transform.GetMatrix()};

    Vector3 const   desiredScale    {p_transform.m_scale* Vector3::SafeScaleReciprocal(p_referential.m_scale)};
     
    return  ConstructTransformFromMatrixWithDesiredScale(referential.GetInverse(), transform, desiredScale);
}


FORCEINLINE Transform   Transform::MultiplyUsingMatrixWithScale                 (Transform const&   p_a,
                                                                                 Transform const&   p_b)
{
    return ConstructTransformFromMatrixWithDesiredScale(p_a.GetMatrix(), p_b.GetMatrix(), p_a.m_scale * p_b.m_scale);
}

#endif // !__TRANSFORM_INL__