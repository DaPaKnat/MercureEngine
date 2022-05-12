#ifndef __MATH_VECTOR_3_INL__
#define __MATH_VECTOR_3_INL__

/*==============================[Public Static Methods]==============================*/

/**
 * Calculates the unsigned angle in degree between two 3D vectors
 * 
 * @param p_from    The vector from which the angular difference is measured.
 * @param p_to      The vector to which the angular difference is measured
 *
 * @return          The angle between the both vectors in degree
 */
FORCEINLINE float   Vector3::AngleDeg               (Vector3 const& p_from, 
                                                     Vector3 const& p_to)
{
    return Math::RadToDeg(Vector3::AngleRad(p_from, p_to));
}

/**
 * Calculates the unsigned angle in degree between two 3D vectors
 * 
 * @param p_from    The vector from which the angular difference is measured.
 * @param p_to      The vector to which the angular difference is measured
 *
 * @return          The angle between the both vectors in degree
 */
FORCEINLINE float   Vector3::AngleRad               (Vector3 const& p_from,
                                                     Vector3 const& p_to)
{
    return Math::Acos((p_from | p_to) / (Math::Sqrt(p_from.GetMagnitudeSquared() * p_to.GetMagnitudeSquared())));
}

/**
 * Clamps the vector given between two others vectors min and max inclued
 * (Component-wise clamp for Vector3)
 *
 * @param p_vector The vector to clamp.
 * @param p_min    The minimum vector (Each components corresponding to minimum).
 * @param p_max    The maximum vector (Each components corresponding to maximum).
 *
 * @return          The vector clamped.
 */
FORCEINLINE Vector3 Vector3::Clamp                  (Vector3 const& p_value,
                                                     Vector3 const& p_min,
                                                     Vector3 const& p_max)
{
    return Vector3
    (
        Math::Clamp(p_value.m_x, p_min.m_x, p_max.m_x),
        Math::Clamp(p_value.m_y, p_min.m_y, p_max.m_y),
        Math::Clamp(p_value.m_z, p_min.m_z, p_max.m_z)
    );
}

/**
 * Compute a copy of the vector given with its magnitude clamped between minimum and maximum magnitude given.
 *
 * @param p_vector          The vector to clamp.
 * @param p_minMagnitude    The minimum magnitude accepted.
 * @param p_maxMagnitude    The maximum magnitude accepted.
 *
 * @return                  The vector clamped.
 */
FORCEINLINE Vector3 Vector3::ClampMagnitude         (Vector3 const& p_vector,
                                                     float const    p_minMagnitude,
                                                     float const    p_maxMagnitude)
{
    float const vectorMagnitude {p_vector.GetMagnitude()};

    if (Math::IsNearlyZero(vectorMagnitude))
        return Vector3::Zero;

    return Math::Clamp(vectorMagnitude, p_minMagnitude, p_maxMagnitude) * p_vector/vectorMagnitude;
}

/**
 * Compute a copy of the vector given with its magnitude clamped to maximum magnitude given.
 *
 * @param p_vector          The vector to clamp.
 * @param p_maxMagnitude    The maximum magnitude accepted.
 *
 * @return                  The vector clamped.
 */
FORCEINLINE Vector3 Vector3::ClampMaxMagnitude      (Vector3 const& p_vector, 
                                                     float const    p_maxMagnitude)
{
    if (p_maxMagnitude < MATH_EPSILON)
        return Vector3::Zero;

    float const magnitudeSquared    {p_vector.GetMagnitudeSquared()};

    if (magnitudeSquared > Math::Square(p_maxMagnitude))
        return p_vector * (Math::InvSqrt(magnitudeSquared) * p_maxMagnitude);
    else
        return p_vector;
}

/**
 * Compute a copy of the vector given with its magnitude clamped to minimum magnitude given.
 *
 * @param p_vector          The vector to clamp.
 * @param p_minMagnitude    The minimum magnitude accepted.
 *
 * @return                  The vector clamped.
 */
FORCEINLINE Vector3 Vector3::ClampMinMagnitude      (Vector3 const& p_vector, 
                                                     float const    p_minMagnitude)
{
    if (p_minMagnitude < MATH_EPSILON)
        return Vector3::Zero;

    float const magnitudeSquared    {p_vector.GetMagnitudeSquared()};

    if (magnitudeSquared < Math::Square(p_minMagnitude))
        return p_vector * (Math::InvSqrt(magnitudeSquared) * p_minMagnitude);
    else
        return p_vector;
}

/**
 * Compute the cross product of two vectors.
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The result of cross product.
 */
FORCEINLINE Vector3 Vector3::Cross                  (Vector3 const& p_a, 
                                                     Vector3 const& p_b)
{
    return Vector3
    (
        p_a.m_y * p_b.m_z - p_a.m_z * p_b.m_y,
        p_a.m_z * p_b.m_x - p_a.m_x * p_b.m_z,
        p_a.m_x * p_b.m_y - p_a.m_y * p_b.m_x
    );
}
/**
 * Converts a vector containing degree values to a vector containing radian values.
 *
 * @param   p_degVector Vector containing degree values
 * @return              Vector containing radian values
 */
FORCEINLINE Vector3 Vector3::DegToRad               (Vector3 const& p_degVector)
{
    return MATH_DEG_2_RAD_F * p_degVector;
}

/**
 * Compute the distance between two points.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The distance between two points.
 */
FORCEINLINE float   Vector3::Distance               (Vector3 const& p_a, 
                                                     Vector3 const& p_b)
{
    return Math::Sqrt(Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y) + Math::Square(p_a.m_z-p_b.m_z));
}

/**
 * Compute the distance in the XY plane only.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The distance between two points in the XY plane.
 */
FORCEINLINE float   Vector3::Distance2D             (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Math::Sqrt(Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y));
}

/**
 * Compute the squared distance between two points.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The squared distance between two points.
 */
FORCEINLINE float   Vector3::DistanceSquared        (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y) + Math::Square(p_a.m_z-p_b.m_z);
}

/**
 * Compute the squared distance in the XY plane only.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The squared distance between two points in the XY plane.
 */
FORCEINLINE float   Vector3::DistanceSquared2D      (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y);
}

/**
 * Compute the dot product of two vectors.
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The result of dot product.
 */
FORCEINLINE float   Vector3::Dot                    (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return p_a.m_x*p_b.m_x + p_a.m_y*p_b.m_y + p_a.m_z*p_b.m_z;

}

/**
 * Computes the unsigned euler angles between two 3D vectors
 *
 * @param p_a   The left handed vector
 * @param p_b   The right handed vector
 *
 * @return      The resulting unsigned euler angles vector in degrees.
 */
FORCEINLINE Vector3 Vector3::EulerAngleDeg          (Vector3 const& p_a, Vector3 const& p_b)
{
    Vector3 result {SignedEulerAngleDeg(p_a, p_b)};

    result.m_x = Math::Repeat(result.m_x,360.f);
    result.m_y = Math::Repeat(result.m_y,360.f);
    result.m_z = Math::Repeat(result.m_z,360.f);

    return result;
}

/**
 * Computes the unsigned euler angles between two 3D vectors
 *
 * @param p_a   The left handed vector
 * @param p_b   The right handed vector
 *
 * @return      The resulting unsigned euler angles vector in radians.
 */
FORCEINLINE Vector3 Vector3::EulerAngleRad          (Vector3 const& p_a, Vector3 const& p_b)
{
    Vector3 result {SignedEulerAngleRad(p_a, p_b)};

    result.m_x = Math::Repeat(result.m_x, MATH_TAU_F);
    result.m_y = Math::Repeat(result.m_y, MATH_TAU_F);
    result.m_z = Math::Repeat(result.m_z, MATH_TAU_F);

    return result;
}

/**
 * @brief Checks if two 3D vectors are colinear
 *
 * @param p_a    The left handed vector
 * @param p_b    The right handed vector
 *
 * @return True  Both vectors are colinear
 * @return False Both vectors are not colinear
 */
FORCEINLINE bool    Vector3::IsCollinear            (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return (p_a ^ p_b).IsNearlyZero();
}

/**
 * Check if two vectors are nearly equal within a specified tolerance.
 *
 * @param p_a           The first vector to check against.
 * @param p_b           The second vector to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vector are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Vector3::IsNearlyEqual          (Vector3 const& p_a,
                                                     Vector3 const& p_b,
                                                     float const    p_tolerance)
{
    return  Math::IsNearlyEqual(p_a.m_x, p_b.m_x, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_y, p_b.m_y, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_z, p_b.m_z, p_tolerance);
}

/**
 * @brief Checks if two 3D vectors are perpendicular
 *
 * @param p_a    The left handed vector
 * @param p_b    The right handed vector
 *
 * @return True  Both vectors are perpendicular
 * @return False Both vectors are not perpendicular
 */
FORCEINLINE bool    Vector3::IsOrthogonal           (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Math::IsNearlyZero(p_a | p_b);
}

/**
 * Linearly interpolates between two 3D vectors (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed vector
 * @param p_b       The right handed vector
 * @param p_alpha   The interpolation value
 *
 * @return          The interpolation of p_a and p_b by p_alpha
 */
FORCEINLINE Vector3 Vector3::Lerp                   (Vector3 const& p_a,
                                                     Vector3 const& p_b,
                                                     float const    p_alpha)
{
    return p_a + p_alpha * (p_b-p_a);
}

/**
 * Moves a point in a straight line towards a target point.
 * Compute a point p_maxDistanceDelta units closer to a target point along a line between current and target. If the target is closer than p_maxDistanceDelta, then the returned value will be equal to target 
 * (ie, the movement will not overshoot the target). 
 * Negative values of p_maxDistanceDelta can be used to push the point away from the target.
 *
 * @param p_current             The current point.
 * @param p_target              The target point.
 * @param p_maxDistanceDelta    The maximum speed
 *
 * @return                      The resulting point of the movement
 */
FORCEINLINE Vector3 Vector3::MoveTowards            (Vector3 const& p_current, 
                                                     Vector3 const& p_target, 
                                                     float const    p_maxDistanceDelta)
{
    Vector3 const   direction   {p_current, p_target};
    float const     distance    {direction.GetMagnitude()};

    if (distance <= p_maxDistanceDelta || Math::IsNearlyZero(distance))
        return p_target;
    
    return p_current + direction/distance * p_maxDistanceDelta;
}

/**
 * Compute a vector made from the smallest  components of two vectors
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The vectors with the smallest  components.
 */
FORCEINLINE Vector3 Vector3::Max                    (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Vector3(Math::Max(p_a.m_x,p_b.m_x), Math::Max(p_a.m_y, p_b.m_y), Math::Max(p_a.m_z, p_b.m_z));
}

/**
 * Compute a vector made from the largest components of two vectors
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The vectors with the largest components.
 */
FORCEINLINE Vector3 Vector3::Min                    (Vector3 const& p_a,
                                                     Vector3 const& p_b)
{
    return Vector3(Math::Min(p_a.m_x,p_b.m_x), Math::Min(p_a.m_y, p_b.m_y), Math::Min(p_a.m_z, p_b.m_z));
}
/**
 * Compute the projected vector of one vector on another one.
 *
 * @param p_vector      The vector to project.
 * @param p_onTarget	The vector to project onto, does not assume it is normalized.

 * @return              The projected vector.
 */
FORCEINLINE Vector3 Vector3::Project                (Vector3 const& p_vector,
                                                     Vector3 const& p_onTarget)
{
    return  ((p_vector | p_onTarget) / p_onTarget.GetMagnitudeSquared()) * p_onTarget;
}

/**
 * Compute the projected vector of one vector on another one.
 *
 * @param p_vector      The vector to project.
 * @param p_onTarget	The vector to project onto, assume it is normalized.
 *
 * @return              The projected vector.
 */
FORCEINLINE Vector3 Vector3::ProjectOnNormal        (Vector3 const& p_vector,
                                                     Vector3 const& p_onNormal)
{
    return  (p_vector | p_onNormal) * p_onNormal;
}

/**
 * Calculate the projection of a vector on the plane defined by PlaneNormal.
 * 
 * @param  p_vector         The vector to project onto the plane.
 * @param  p_planeNormal    The normal of the plane (assumed to be unit length).
 *
 * @return                  Projection of V onto plane.
 */
FORCEINLINE Vector3 Vector3::ProjectOnPlane         (Vector3 const& p_vector,
                                                     Vector3 const& p_planeNormal)
{
    return p_vector - ProjectOnNormal(p_vector, p_planeNormal);
}

/**
 * Converts a vector containing radian values to a vector containing degree values.
 *
 * @param p_radVector	Vector containing radian values
 *
 * @return              Vector containing degree values
 */
FORCEINLINE Vector3 Vector3::RadToDeg               (Vector3 const& p_radVector)
{
    return MATH_RAD_2_DEG_F * p_radVector;
}


/**
 * Compute the reflection of a vector around a normal
 *
 * @param p_direction   The vector to reflect
 * @param p_normal      The vector used as the normal of the surface to reflect around.

 * @return              The reflection vector.
 */
FORCEINLINE Vector3 Vector3::Reflect                (Vector3 const& p_direction,
                                                     Vector3 const& p_normal)
{
    return p_direction - p_normal * (2.0f * (p_direction | p_normal));
}

/**
* Compute the negative reflection of a vector around a normal
*
* @param p_direction   The vector to reflect
* @param p_normal      The vector used as the normal of the surface to reflect around.

* @return              The negative reflection vector.
*/
FORCEINLINE Vector3 Vector3::ReflectInvert          (Vector3 const& p_direction,
                                                     Vector3 const& p_normal)
{
    return  p_normal * (2.0f * (p_direction | p_normal)) - p_direction;
}

FORCEINLINE Vector3 Vector3::SafeScaleReciprocal    (Vector3 const& p_scale,
                                                     float const    p_tolerance)
{
    return Vector3
    (
        (Math::Abs(p_scale.m_x) <= p_tolerance) ? 0.0f : 1.0f / p_scale.m_x,
        (Math::Abs(p_scale.m_y) <= p_tolerance) ? 0.0f : 1.0f / p_scale.m_y,
        (Math::Abs(p_scale.m_z) <= p_tolerance) ? 0.0f : 1.0f/ p_scale.m_z
    );
}

/**
 * Computes the signed angle in degree between two vectors
 *
 * @param p_from    The vector from which the angular difference is measured.
 * @param p_to      The vector to which the angular difference is measured.
 * @param p_axis    A vector around which the other vectors are rotated.
 *
 * @return          The signed angle in degree between p_from and p_to.
 */
FORCEINLINE float   Vector3::SignedAngleDeg         (Vector3 const& p_from,
                                                     Vector3 const& p_to, 
                                                     Vector3 const& p_axis)
{
    return Math::RadToDeg(SignedAngleRad(p_from, p_to, p_axis));
}

/**
 * Computes the signed angle in radian between two vectors
 *
 * @param p_from    The vector from which the angular difference is measured.
 * @param p_to      The vector to which the angular difference is measured.
 * @param p_axis    A vector around which the other vectors are rotated.
 *
 * @return          The signed angle in radian between p_from and p_to.
 */
FORCEINLINE float   Vector3::SignedAngleRad         (Vector3 const& p_from, 
                                                     Vector3 const& p_to, 
                                                     Vector3 const& p_axis)
{
    
    float const sign    {Math::FloatSelect((p_axis | p_from ^ p_to), 1.0f, -1.0f)};
    float const angle   {AngleRad(p_from, p_to)};
   
    return sign * angle;
}

/**
 * Computes the signed euler angles between two 3D vectors
 * 
 * @param p_a   The left handed vector
 * @param p_b   The right handed vector
 *
 * @return      The resulting signed euler angles vector in degrees
 */
FORCEINLINE Vector3 Vector3::SignedEulerAngleDeg    (Vector3 const& p_a, 
                                                     Vector3 const& p_b)
{
    float x {Vector2::SignedAngleDeg(Vector2(p_a.m_y, p_a.m_z), Vector2(p_b.m_y, p_b.m_z))};
    float y {Vector2::SignedAngleDeg(Vector2(p_a.m_z, p_a.m_x), Vector2(p_b.m_z, p_b.m_x))};
    float z {Vector2::SignedAngleDeg(Vector2(p_a.m_x, p_a.m_y), Vector2(p_b.m_x, p_b.m_y))};

    if (Math::IsNaN(x))
        x = 0.0f;

    if (Math::IsNaN(y))
        y = 0.0f;

    if (Math::IsNaN(z))
        z = 0.0f;

    return Vector3 (x,y,z);
}

/**
 * Computes the signed euler angles between two 3D vectors
 * 
 * @param p_a   The left handed vector
 * @param p_b   The right handed vector
 *
 * @return      The resulting signed euler angles vector in radians
 */
FORCEINLINE Vector3 Vector3::SignedEulerAngleRad    (Vector3 const& p_a, 
                                                     Vector3 const& p_b)
{
    float x {Vector2::SignedAngleRad(Vector2(p_a.m_y, p_a.m_z), Vector2(p_b.m_y, p_b.m_z))};
    float y {Vector2::SignedAngleRad(Vector2(p_a.m_z, p_a.m_x), Vector2(p_b.m_z, p_b.m_x))};
    float z {Vector2::SignedAngleRad(Vector2(p_a.m_x, p_a.m_y), Vector2(p_b.m_x, p_b.m_y))};

    if (Math::IsNaN(x))
        x = 0.0f;

    if (Math::IsNaN(y))
        y = 0.0f;

    if (Math::IsNaN(z))
        z = 0.0f;

    return Vector3 (x,y,z);
}

/*==============================[Constructors + Destructor]==============================*/

/** Default constructor, all components are initialized to 0.0f */
FORCEINLINE Vector3::Vector3    ()
:   m_x {0.0f},
    m_y {0.0f},
    m_z {0.0f}
{}

/**
 * Constructor initializing all components to a single float value.
 *
 * @param p_value   The value to set all components to.
 */
FORCEINLINE Vector3::Vector3    (float const    p_value)
:   m_x {p_value},
    m_y {p_value},
    m_z {p_value}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_x   The X coordinate.
 * @param p_y   The Y coordinate.
 * @param p_z   The Z coordinate.
 */
FORCEINLINE Vector3::Vector3    (float const    p_x,
                                 float const    p_y,
                                 float const    p_z)
:   m_x {p_x},
    m_y {p_y},
    m_z {p_z}
{}

/**
 * Constructor using two vectors to create one by subtracting the first to the second.
 * Get the vector direction from a vector to an another one.
 *
 * @param p_to      The start vector.
 * @param p_from    The end vector.
 */
FORCEINLINE Vector3::Vector3    (Vector3 const& p_from,
                                 Vector3 const& p_to)
:   m_x {p_to.m_x - p_from.m_x},
    m_y {p_to.m_y - p_from.m_y},    
    m_z {p_to.m_z - p_from.m_z}
{}

/**
 * Constructor using one vector for the first two components and one value for z component.
 *
 * @param p_vector2 The 2D vector to set first two components.
 * @param p_z       Z coordinate (p_w = 0.0f by default).
 */
FORCEINLINE Vector3::Vector3    (Vector2 const& p_vector2,
                                 float const    p_z)
:   m_x {p_vector2.m_x},
    m_y {p_vector2.m_y},
    m_z {p_z}
{}

/**
 * Copy constructor using initial vector for each component.
 *
 * @param p_copy    The initial vector to copy.
 */
FORCEINLINE Vector3::Vector3    (Vector3 const& p_copy)
:   m_x {p_copy.m_x},
    m_y {p_copy.m_y},
    m_z {p_copy.m_z}
{}

/**
 * Move constructor using initial vector for each component.
 *
 * @param p_move    The initial vector to move.
 */
FORCEINLINE Vector3::Vector3    (Vector3&&      p_move)
:   m_x {std::move(p_move.m_x)},
    m_y {std::move(p_move.m_y)},
    m_z {std::move(p_move.m_z)}
{}
  
/*==============================[Public Local Methods]==============================*/

/**
 * Checks whether all components of this vector are the same, within a tolerance.
 *
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vectors are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Vector3::AllComponentsEquals    (float const    p_tolerance)                const
{
    return  Math::IsNearlyEqual(m_x, m_y, p_tolerance) &&
            Math::IsNearlyEqual(m_x, m_z, p_tolerance) &&
            Math::IsNearlyEqual(m_y, m_z, p_tolerance);
}

/**
 * Gets a specific component of the vector by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The copy of the specified component.
 */
FORCEINLINE float   Vector3::Component              (int32 const    p_index)                    const
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Vector3's index range : [0,2]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else
        return m_z;
}

/**
 * Gets a specific component of the vector by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The reference to the specified component.
 */
FORCEINLINE float&  Vector3::Component              (int32 const    p_index)
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Vector3's index range : [0,2]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else
        return m_z;
}           

/**
 * Get a copy of this vector with absolute value of each component.
 *
 * @return  A copy of this vector with absolute value of each component.
 */
FORCEINLINE Vector3 Vector3::GetAbs                 ()                                          const
{
    return Vector3(Math::Abs(m_x), Math::Abs(m_y), Math::Abs(m_z));
}

/**
 * Get the maximum absolute value of the vector's components.
 *
 * @return The maximum absolute value of the vector's components.
 */
FORCEINLINE float   Vector3::GetAbsMax              ()                                          const
{
    return Math::Max3(Math::Abs(m_x), Math::Abs(m_y), Math::Abs(m_z));

}

/**
 * Get the minimum absolute value of the vector's components.
 *
 * @return The minimum absolute value of the vector's components.
 */
FORCEINLINE float   Vector3::GetAbsMin              ()                                          const
{
    return Math::Min3(Math::Abs(m_x), Math::Abs(m_y), Math::Abs(m_z));

}

/**
 * Get the magnitude (length) of this vector.
 *
 * @return The magnitude of this vector.
 */
FORCEINLINE float   Vector3::GetMagnitude           ()                                          const
{
    return  Math::Sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

/**
 * Get the magnitude of the 2D components of this vector.
 *
 * @return The 2D magnitude of this vector.
 */
FORCEINLINE float   Vector3::GetMagnitude2D         ()                                          const
{
    return Math::Sqrt(m_x*m_x + m_y * m_y);
}

/**
 * Get the squared magnitude of this vector.
 *
 * @return The squared magnitude of this vector.
 */
FORCEINLINE float   Vector3::GetMagnitudeSquared    ()                                          const
{
    return  m_x*m_x + m_y*m_y + m_z*m_z;
}

/**
 * Get the squared magnitude of the 2D components of this vector.
 *
 * @return The squared 2D magnitude of this vector.
 */
FORCEINLINE float   Vector3::GetMagnitudeSquared2D  ()                                          const
{
    return  m_x*m_x + m_y*m_y;
}

/**
 * Get the maximum value of the vector's components.
 *
 * @return  The maximum value of the vector's components.
 */
FORCEINLINE float   Vector3::GetMax                 ()                                          const
{
    return Math::Max3(m_x, m_y, m_z);
}

/**
 * Get the minimum value of the vector's components.
 *
 * @return The minimum value of the vector's components.
 */
FORCEINLINE float   Vector3::GetMin                 ()                                          const
{
    return Math::Min3(m_x, m_y, m_z);
}

/**
 * Get a copy of the vector as sign only.
 * Each component is set to +1 or -1, with the sign of zero treated as +1.
 *
 * @return  A copy of the vector with each component set to +1 or -1
 */
FORCEINLINE Vector3 Vector3::GetSign                ()                                          const
{
    return Vector3
    (
        Math::FloatSelect(m_x, 1.0f, -1.0f),
        Math::FloatSelect(m_y, 1.0f, -1.0f),
        Math::FloatSelect(m_z, 1.0f, -1.0f)
    );
}

/**
 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
 * Returns zero vector if vector length is too small to safely normalize.
 *
 * @param p_tolerance   Minimum squared vector length.

 * @return              A normalized copy if safe, (0,0,0) otherwise.
 */
FORCEINLINE Vector3 Vector3::GetSafeNormalized      (float const    p_tolerance)                const
{
    float const magnitudeSquared    {m_x*m_x + m_y*m_y + m_z*m_z};

    if      (magnitudeSquared == 1.0f)
        return *this;

    else if (magnitudeSquared < p_tolerance)
        return Vector3::Zero;

    float const magnitude           {Math::Sqrt(magnitudeSquared)};
    return Vector3(m_x/ magnitude, m_y / magnitude, m_z/ magnitude);
} 

/**
 * Gets a normalized copy of the 2D components of the vector, checking it is safe to do so. Z is set to zero. 
 * Returns zero vector if vector length is too small to normalize.
 *
 * @param   p_tolerance Minimum squared vector length.

 * @return  Normalized copy if safe, otherwise returns zero vector.
 */
FORCEINLINE Vector3 Vector3::GetSafeNormalized2D    (float const    p_tolerance)                const
{
    float const magnitudeSquared2D  {m_x*m_x + m_y*m_y};
    
    if      (magnitudeSquared2D == 1.0f)
    {
        if (m_z == 0.0f)
            return *this;
        else
            return Vector3(m_x, m_y, 0.0f);
    }

    else if (magnitudeSquared2D < p_tolerance)
        return Vector3::Zero;

    float const magnitude2D {Math::Sqrt(magnitudeSquared2D)};

    return Vector3(m_x / magnitude2D, m_y / magnitude2D, 0.0f);
}

/**
 * Calculates normalized version of vector without checking for zero length.
 *
 * @return Normalized version of vector.
 */
FORCEINLINE Vector3 Vector3::GetUnsafeNormalized    ()                                          const
{
    float const magnitude   {Math::Sqrt(m_x*m_x + m_y*m_y + m_z*m_z)};
   
    return Vector3(m_x / magnitude, m_y / magnitude, m_z / magnitude);
}

/**
 * Checks whether vector is near to zero within a specified tolerance.
 *
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vector is near to zero, false otherwise.
 */
FORCEINLINE bool    Vector3::IsNearlyZero           (float const    p_tolerance)                const
{
   return   Math::IsNearlyZero(m_x, p_tolerance) &&
            Math::IsNearlyZero(m_y, p_tolerance) &&
            Math::IsNearlyZero(m_z, p_tolerance);
}

/**
 * Checks whether vector is normalized.
 *
 * @return  True if Normalized, false otherwise.
 */
FORCEINLINE bool    Vector3::IsNormalized           ()                                          const
{
    return Math::IsNearlyZero(1.0f - GetMagnitudeSquared());
}

/**
 * Check whether X, Y and Z are nearly equal.
 *
 * @param p_tolerance   Specified Tolerance.

 * @return              True if X == Y == Z within the specified tolerance.
 */
FORCEINLINE bool    Vector3::IsUniform              (float const    p_tolerance)                const
{
    return AllComponentsEquals(p_tolerance);
}

/**
 * Check if the vector is of unit length, with specified tolerance.
 *
 * @param p_lengthSquaredTolerance  Tolerance against squared length.
 *
 * @return                          True if the vector is a unit vector within the specified tolerance.
 */
FORCEINLINE bool    Vector3::IsUnit                 (float const    p_lengthSquaredTolerance)   const
{
    return Math::Abs(1.0f - GetMagnitudeSquared()) < p_lengthSquaredTolerance;
}

/**
 * Checks whether all components of the vector are exactly zero.
 *
 * @return  True if the vector is exactly zero, false otherwise.
 */
FORCEINLINE bool    Vector3::IsZero                 ()                                          const
{
    return  m_x == 0.0f &&
            m_y == 0.0f &&
            m_z == 0.0f;
}

/**
 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
 *
 * @param p_tolerance   The minimum squared length of vector for normalization.

 * @return              True if the vector was normalized correctly, false otherwise.
 */
FORCEINLINE bool    Vector3::Normalize              (float const    p_tolerance)
{
    float const magnitudeSquared    {m_x*m_x + m_y*m_y + m_z*m_z};

    if (magnitudeSquared > p_tolerance)
    {
        float const scale   {Math::InvSqrt(magnitudeSquared)};
        m_x *= scale; 
        m_y *= scale; 
        m_z *= scale; 
        return true;
    }
    return false;
}

/**
 * Set the values of the vector directly.
 *
 * @param p_x   The new X coordinate.
 * @param p_y   The new Y coordinate.
 * @param p_z   The new Z coordinate.
 */
FORCEINLINE void    Vector3::Set                    (float const    p_x,
                                                     float const    p_y,
                                                     float const    p_z)
{
    m_x = p_x;
    m_y = p_y;
    m_z = p_z;
}

/**
 * Set the values of the vector directly.
 *
 * @param p_value   The new value of each components.
 *
 */
FORCEINLINE void    Vector3::Set                    (float const    p_value)
{
    m_x = p_value;
    m_y = p_value;
    m_z = p_value;
}

/**
 * Util to convert this vector into a unit direction vector and its original length.
 *
 * @param p_outDirection Reference passed in to store unit direction vector.
 * @param p_outLength    Reference passed in to store length of the vector.
 */
FORCEINLINE void    Vector3::ToDirectionAndLength   (Vector3&       p_outDirection,
                                                     float&         p_outLength)                const
{
    p_outLength = GetMagnitude();
    
    if (Math::IsNearlyZero(p_outLength))
    {
        p_outDirection = Vector3::Zero;
    }
    
    else
    {
        p_outDirection = Vector3(m_x/p_outLength, m_y/p_outLength, m_z/p_outLength);
    }
}

/*==============================[Public Local Operators]==============================*/

/**
 * Check against another vector for equality.
 *
 * @param p_vector  The vector to check against.
 *
 * @return          True    if the vectors are equal.
 * @return          False   if the vectors are different.
 */
FORCEINLINE bool        Vector3::operator== (Vector3 const& p_vector)   const
{
    return m_x == p_vector.m_x && m_y == p_vector.m_y && m_z == p_vector.m_z;
}

/**
  * Check against another vector for inequality.
  *
  * @param p_vector  The vector to check against.
  *
  * @return          True    if the vectors are different.
  * @return          False   if the vectors are equal.
  */
FORCEINLINE bool        Vector3::operator!= (Vector3 const& p_vector)   const
{
    return m_x != p_vector.m_x || m_y != p_vector.m_y || m_z != p_vector.m_z;
}

/**
 * Copy another vector into this one
 *
 * @param p_copy    The other vector to copy.
 *
 * @return          The reference to vector after copy.
 */
FORCEINLINE Vector3&    Vector3::operator=  (Vector3 const& p_copy)
{
    m_x = p_copy.m_x;
    m_y = p_copy.m_y;
    m_z = p_copy.m_z;

    return *this;
}

/**
 * Move another vector into this one
 *
 * @param p_vector  The vector to move.
 *
 * @return          The reference to vector after move.
 */
FORCEINLINE Vector3&    Vector3::operator=  (Vector3&&      p_move)
{
    m_x = std::move(p_move.m_x);
    m_y = std::move(p_move.m_y);
    m_z = std::move(p_move.m_z);

    return *this;
}

/**
 * Gets specific component of the vector by index.
 *
 * @param p_index   The index of vector component. Must be int [0,2] range.
 *
 * @return          The copy of the component corresponding to the index given.
 */
FORCEINLINE float       Vector3::operator[] (int32 const    p_index)    const
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Vector3's index range : [0,2]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else
        return m_z;
}

/**
 * Gets specific component of the vector.
 *
 * @param p_index   The index of vector component. Must be int [0,2] range.
 *
 * @return          The reference to component corresponding to the index given.
 */
FORCEINLINE float&      Vector3::operator[] (int32 const    p_index)
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Vector3's index range : [0,2]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else
        return m_z;
}

/**
 * Gets the result of component-wise addition of this and another vector.
 *
 * @param p_vector  The vector to add to this.
 *
 * @return          The result of vector addition.
 */
FORCEINLINE Vector3     Vector3::operator+  (Vector3 const& p_vector)   const
{
    return Vector3
    (
        m_x + p_vector.m_x, 
        m_y + p_vector.m_y,
        m_z + p_vector.m_z
    );
}

/**
 * Gets the result of adding to each component of the vector.
 *
 * @param p_value   How much to add to each component.
 *
 * @return          The result of addition.
 */
FORCEINLINE Vector3     Vector3::operator+  (float const    p_value)    const
{
    return Vector3
    (
        m_x + p_value, 
        m_y + p_value,
        m_z + p_value
    );
}

/**
 * Adds another vector to this.
 * Uses component-wise addition.
 *
 * @param p_vector  Vector to add to this.
 *
 * @return          Copy of the vector after addition.
 */
FORCEINLINE Vector3&    Vector3::operator+= (Vector3 const& p_vector)
{
    m_x += p_vector.m_x;
    m_y += p_vector.m_y;
    m_z += p_vector.m_z;

    return *this;
}

 /**
  * Adds a value to each component of the vector
  *
  * @param p_value   How much to add to each component.
  *
  * @return          The reference of the vector after addition.
  */
FORCEINLINE Vector3&    Vector3::operator+= (float const    p_value)
{
    m_x += p_value;
    m_y += p_value;
    m_z += p_value;

    return *this;
}

/**
 * Get a negated copy of the vector.
 *
 * @return          A negated copy of the vector.
 */
FORCEINLINE Vector3     Vector3::operator-  ()                          const
{
    return Vector3(-m_x, -m_y, -m_z);
}

/**
 * Gets the result of component-wise subtraction of this by another vector.
 *
 * @param p_vector  The vector to subtract from this.
 *
 * @return          The result of vector subtraction.
 */
FORCEINLINE Vector3     Vector3::operator-  (Vector3 const& p_vector)   const
{
    return Vector3
    (
        m_x - p_vector.m_x, 
        m_y - p_vector.m_y,
        m_z - p_vector.m_z
    );
}

/**
 * Gets the result of subtracting from each component of the vector.
 *
 * @param p_value   How much to subtract from each component.
 *
 * @return          The result of subtraction.
 */
FORCEINLINE Vector3     Vector3::operator-  (float const    p_value)    const
{
    return Vector3
    (
        m_x - p_value, 
        m_y - p_value,
        m_z - p_value
    );
}

/**
 * Subtracts another vector from this.
 * Uses component-wise subtraction.
 *
 * @param p_vector  The vector to subtract from this.
 *
 * @return          The reference of the vector after subtraction.
 */
FORCEINLINE Vector3&    Vector3::operator-= (Vector3 const& p_vector)
{
    m_x -= p_vector.m_x;
    m_y -= p_vector.m_y;
    m_z -= p_vector.m_z;

    return *this;
}

/**
 * Substracts a value to each component of the vector
 *
 * @param p_value   How much to subtract from each component.
 *
 * @return          The reference of the vector after subtraction.
 */
FORCEINLINE Vector3&    Vector3::operator-= (float const    p_value)
{
    m_x -= p_value;
    m_y -= p_value;
    m_z -= p_value;

    return *this;
}

/**
 * Gets the result of component-wise multiplication of this vector by another.
 *
 * @param p_vector  The vector to multiply with.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Vector3     Vector3::operator*  (Vector3 const& p_vector)   const
{
    return Vector3
    (
        m_x * p_vector.m_x, 
        m_y * p_vector.m_y,
        m_z * p_vector.m_z
    );
}

/**
 * Gets the result of scaling the vector (multiplying each component by a value).
 *
 * @param p_scale   What to multiply each component by.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Vector3     Vector3::operator*  (float const    p_scale)    const
{
    return Vector3
    (
        m_x * p_scale, 
        m_y * p_scale,  
        m_z * p_scale
    );    
}

/**
 * Multiplies the vector with another vector, using component-wise multiplication.
 *
 * @param p_vector  What to multiply this vector with.
 *
 * @return          The reference of the vector after multiplication.
 */
FORCEINLINE Vector3&    Vector3::operator*= (Vector3 const& p_vector)
{
    m_x *= p_vector.m_x;
    m_y *= p_vector.m_y;
    m_z *= p_vector.m_z;

    return *this;
}

/**
 * Multiply the vector by a value.
 * Multiply each component of the vector by a number
 *
 * @param p_scale   What to multiply each component by.
 *
 * @return          The reference of the vector after multiplication.
 */
FORCEINLINE Vector3&    Vector3::operator*= (float const    p_scale)
{
    m_x *= p_scale;
    m_y *= p_scale;
    m_z *= p_scale;

    return *this;
}

/**
 * Gets the result of component-wise division of this vector by another.
 *
 * @param p_vector  The vector to divide by.
 *
 * @return          The result of division.
 */
FORCEINLINE Vector3     Vector3::operator/  (Vector3 const& p_vector)   const
{
    return Vector3
    (   
        m_x / p_vector.m_x, 
        m_y / p_vector.m_y,
        m_z / p_vector.m_z
    );
}

/**
 * Gets the result of dividing each component of the vector by a value.
 *
 * @param p_scale   What to divide each component by.
 *
 * @return          The result of division.
 */
FORCEINLINE Vector3     Vector3::operator/  (float const    p_scale)    const
{
    return Vector3
    (
        m_x / p_scale, 
        m_y / p_scale,  
        m_z / p_scale
    );    
}

/**
 * Divides the vector by another vector, using component-wise division.
 *
 * @param p_vector  The vector to divide by.
 *
 * @return          The reference of the vector after division.
 */
FORCEINLINE Vector3&    Vector3::operator/= (Vector3 const& p_vector)
{
    m_x /= p_vector.m_x;
    m_y /= p_vector.m_y;
    m_z /= p_vector.m_z;

    return *this;
}

/**
 * Divides the vector by a number.
 * Divides each component of the vector by a number
 *
 * @param p_scale   What to divide each component by.
 *
 * @return          The reference of the vector after division.
 */
FORCEINLINE Vector3&    Vector3::operator/= (float const    p_scale)
{
    m_x /= p_scale;
    m_y /= p_scale;
    m_z /= p_scale;

    return *this;
}

/**
 * Calculate cross product between this and another vector.
 *
 * @param p_vector  The other vector.
 *
 * @return          The result of cross product.
 */
FORCEINLINE Vector3     Vector3::operator^  (Vector3 const& p_vector)   const
{
    return Vector3
    (   
        m_y * p_vector.m_z - m_z * p_vector.m_y, 
        m_z * p_vector.m_x - m_x * p_vector.m_z, 
        m_x * p_vector.m_y - m_y * p_vector.m_x
    );
}

/**
 * Calculate the dot product between this and another vector.
 *
 * @param p_vector  The other vector.
 *
 * @return          The result of dot product.
 */
FORCEINLINE float       Vector3::operator|  (Vector3 const& p_vector)   const
{
    return m_x*p_vector.m_x + m_y*p_vector.m_y + m_z*p_vector.m_z;
}

/*==============================[Public Globale Operators]==============================*/

/**
 * Gets the result of adding a value to each component of the vector given.
 *
 * @param p_value   The value to add to each component of the vector
 * @param p_vector  The vector which receive the value to add.
 *
 * @return          The result of addition.
 */
FORCEINLINE Vector3 operator+   (float const    p_value,
                                 Vector3 const& p_vector)
{
    return Vector3
    (
        p_vector.m_x + p_value, 
        p_vector.m_y + p_value,
        p_vector.m_z + p_value
    );
}

/**
 * Multiplies a vector by a scaling factor.
 *
 * @param p_scale   The scaling factor.
 * @param p_vector  The vector to scale.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Vector3 operator*   (float const    p_scale,
                                 Vector3 const& p_vector)
{
    return Vector3
    (
        p_vector.m_x * p_scale, 
        p_vector.m_y * p_scale,  
        p_vector.m_z * p_scale
    );
}

#endif // !__MATH_VECTOR_3_INL__