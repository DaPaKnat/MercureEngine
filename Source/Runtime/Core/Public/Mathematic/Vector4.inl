#ifndef __MATH_VECTOR_4_INL__
#define __MATH_VECTOR_4_INL__

/*==============================[Public Static Methods]==============================*/

/**
 * Compute the distance between two points.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The distance between two points.
 */
FORCEINLINE float   Vector4::Distance           (Vector4 const& p_a, 
                                                 Vector4 const& p_b)
{
    return Math::Sqrt(Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y) + Math::Square(p_a.m_z-p_b.m_z) + Math::Square(p_a.m_w-p_b.m_w));
}

/**
 * Compute the squared distance between two points.
 *
 * @param p_a   The first point.
 * @param p_b   The second point.
 *
 * @return      The squared distance between two points.
 */
FORCEINLINE float   Vector4::DistanceSquared    (Vector4 const& p_a,
                                                 Vector4 const& p_b)
{
    return Math::Square(p_a.m_x-p_b.m_x) + Math::Square(p_a.m_y-p_b.m_y) + Math::Square(p_a.m_z-p_b.m_z) + Math::Square(p_a.m_w-p_b.m_w);
}

/**
 * Compute the dot product of two vectors.
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The result of dot product.
 */
FORCEINLINE float   Vector4::Dot                (Vector4 const& p_a,
                                                 Vector4 const& p_b)
{
    return p_a.m_x*p_b.m_x + p_a.m_y*p_b.m_y + p_a.m_z*p_b.m_z + p_a.m_w*p_b.m_w;

}

/**
 * Check if two vectors are nearly equal within a specified tolerance.
 *
 * @param p_vector      The first vector to check against.
 * @param p_vector      The second vector to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vector are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Vector4::IsNearlyEqual      (Vector4 const& p_a,
                                                 Vector4 const& p_b,
                                                 float const    p_tolerance)
{
    return  Math::IsNearlyEqual(p_a.m_x, p_b.m_x, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_y, p_b.m_y, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_z, p_b.m_z, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_w, p_b.m_w, p_tolerance);
}

/**
 * @brief Linearly interpolates between two 3D vectors (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed vector
 * @param p_b       The right handed vector
 * @param p_alpha   The interpolation value
 *
 * @return          The interpolation of p_a and p_b by p_alpha
 */
FORCEINLINE Vector4 Vector4::Lerp               (Vector4 const& p_a,
                                                 Vector4 const& p_b,
                                                 float const    p_alpha)
{
    return p_a + p_alpha * (p_b-p_a);
}

/**
 * Compute a vector made from the smallest  components of two vectors
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The vectors with the smallest  components.
 */
FORCEINLINE Vector4 Vector4::Max                (Vector4 const& p_a,
                                                 Vector4 const& p_b)
{
    return Vector4(Math::Max(p_a.m_x,p_b.m_x), Math::Max(p_a.m_y, p_b.m_y), Math::Max(p_a.m_z, p_b.m_z), Math::Max(p_a.m_w, p_b.m_w));
}

/**
 * Compute a vector made from the largest components of two vectors
 *
 * @param p_a   The first vector.
 * @param p_b   The second vector.
 *
 * @return      The vectors with the largest components.
 */
FORCEINLINE Vector4 Vector4::Min                (Vector4 const& p_a,
                                                 Vector4 const& p_b)
{
    return Vector4(Math::Min(p_a.m_x,p_b.m_x), Math::Min(p_a.m_y, p_b.m_y), Math::Min(p_a.m_z, p_b.m_z), Math::Min(p_a.m_w, p_b.m_w));
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
FORCEINLINE Vector4 Vector4::MoveTowards        (Vector4 const& p_current, 
                                                 Vector4 const& p_target, 
                                                 float const    p_maxDistanceDelta)
{
    Vector4 const   direction   {p_current, p_target};
    float const     distance    {direction.GetMagnitude()};

    if (distance <= p_maxDistanceDelta || Math::IsNearlyZero(distance))
        return p_target;
    
    return p_current + direction/distance * p_maxDistanceDelta;
}

/**
 * Compute the projected vector of one vector on another one.
 *
 * @param p_vector      The vector to project.
 * @param p_onTarget	The vector to project onto, does not assume it is normalized.

 * @return              The projected vector.
 */
FORCEINLINE Vector4 Vector4::Project            (Vector4 const& p_vector,
                                                 Vector4 const& p_onTarget)
{
    return  ((p_vector | p_onTarget) / p_onTarget.GetMagnitudeSquared()) * p_onTarget;
}

/**
 * Compute the projected vector of one vector on another one.
 *
 * @param p_vector              The vector to project.
 * @param p_onTargetNormalized	The vector to project onto, assume it is normalized.

 * @return                      The projected vector.
 */
FORCEINLINE Vector4 Vector4::ProjectOnNormal    (Vector4 const& p_vector,
                                                 Vector4 const& p_onTargetNormalized)
{
    return  (p_vector | p_onTargetNormalized) * p_onTargetNormalized;
}


/*==============================[Constructors + Destructor]==============================*/

/** Default constructor, all components are initialized to 0.0f */
FORCEINLINE Vector4::Vector4    ()
:   m_x {0.0f},
    m_y {0.0f},
    m_z {0.0f},
    m_w {0.0f}
{}

/**
 * Constructor initializing all components to a single float value.
 *
 * @param p_value   The value to set all components to.
 */
FORCEINLINE Vector4::Vector4    (float const    p_value)
:   m_x {p_value},
    m_y {p_value},
    m_z {p_value},
    m_w {p_value}
{}

/**
 * Constructor using initial values for each component.
 *
 * @param p_x   The X coordinate.
 * @param p_y   The Y coordinate.
 * @param p_z   The Z coordinate.
 * @param p_z   The W coordinate.
 */
FORCEINLINE Vector4::Vector4    (float const    p_x,
                                 float const    p_y,
                                 float const    p_z,
                                 float const    p_w)
:   m_x {p_x},
    m_y {p_y},
    m_z {p_z},
    m_w {p_w}
{}

/**
 * Constructor using one vector for the first three components and two values for Y and W component.
 *
 * @param p_vector2 The 3D vector to set first three components.
 * @param p_y       Y coordinate (p_y = 0.0f by default).
 * @param p_w       W coordinate (p_w = 0.0f by default).
 */
FORCEINLINE Vector4::Vector4    (Vector2 const& p_vector2,
                                 float const    p_z,
                                 float const    p_w)
:   m_x {p_vector2.m_x},
    m_y {p_vector2.m_y},
    m_z {p_z},
    m_w {p_w}
{}

/**
 * Constructor using one vector for the first three components and one value for W component.
 *
 * @param p_vector3 The 3D vector to set first three components.
 * @param p_w       W coordinate (p_w = 0.0f by default).
 */
FORCEINLINE Vector4::Vector4    (Vector3 const& p_vector3,
                                 float const    p_w)
:   m_x {p_vector3.m_x},
    m_y {p_vector3.m_y},
    m_z {p_vector3.m_z},
    m_w {p_w}
{}

/**
 * Constructor using two vectors to create one by subtracting the first to the second.
 * Get the vector direction from a vector to an another one.
 *
 * @param p_to      The start vector.
 * @param p_from    The end vector.
 */
FORCEINLINE Vector4::Vector4    (Vector4 const& p_from,
                                 Vector4 const& p_to)
:   m_x {p_to.m_x - p_from.m_x},
    m_y {p_to.m_y - p_from.m_y},    
    m_z {p_to.m_z - p_from.m_z}
{}

/**
 * Creates and initializes a new vector from the specified 2D vectors.
 *
 * @param p_xy  A 2D vector holding the X and Y components.
 * @param p_zw  A 2D vector holding the Z and W components.
 */
FORCEINLINE Vector4::Vector4    (Vector2 const& p_xy,
                                 Vector2 const& p_zw)
:   m_x {p_xy.m_x},
    m_y {p_xy.m_y},    
    m_z {p_zw.m_x},
    m_w {p_zw.m_y}
{}

/**
 * Copy constructor using initial vector for each component.
 *
 * @param p_copy    The initial vector to copy.
 */
FORCEINLINE Vector4::Vector4    (Vector4 const& p_copy)
:   m_x {p_copy.m_x},
    m_y {p_copy.m_y},
    m_z {p_copy.m_z},
    m_w {p_copy.m_w}
{}

/**
 * Move constructor using initial vector for each component.
 *
 * @param p_copy    The initial vector to copy.
 */
FORCEINLINE Vector4::Vector4    (Vector4&&      p_move)
:   m_x {std::move(p_move.m_x)},
    m_y {std::move(p_move.m_y)},
    m_z {std::move(p_move.m_z)},
    m_w {std::move(p_move.m_w)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 * Checks whether all components of this vector are the same, within a tolerance.
 *
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vectors are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Vector4::AllComponentsEquals    (float const    p_tolerance)                const
{
    return  Math::IsNearlyEqual(m_x, m_y, p_tolerance) &&
            Math::IsNearlyEqual(m_x, m_z, p_tolerance) &&
            Math::IsNearlyEqual(m_x, m_w, p_tolerance) &&
            Math::IsNearlyEqual(m_y, m_z, p_tolerance) &&
            Math::IsNearlyEqual(m_y, m_w, p_tolerance) &&
            Math::IsNearlyEqual(m_z, m_w, p_tolerance);
}

/**
 * Gets a specific component of the vector by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The copy of the specified component.
 */
FORCEINLINE float   Vector4::Component              (int32 const    p_index)                    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Vector4's index range : [0,3]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else if (p_index == 2)
        return m_z;
    else
        return m_w;
}

/**
 * Gets a specific component of the vector by index.
 *
 * @param p_index   The index of the component required.
 *
 * @return          The reference to the specified component.
 */
FORCEINLINE float&  Vector4::Component              (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Vector4's index range : [0,3]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else if (p_index == 2)
        return m_z;
    else
        return m_w;
}           

/**
 * Computes the homogenize of this vector (The three first components are divided by w)
 *
 * @return The homogenized vector of this.
 */
FORCEINLINE Vector4 Vector4::GetHomogenized         ()                                          const
{
    return Vector4
    (
        m_x / m_w,
        m_y / m_w,
        m_z / m_w,
        1.0f
    );
}

/**
 * Computes the homogenize of this vector (The three first components are divided by w)
 *
 * @return The homogenized vector of this into a Vector3.
 */
FORCEINLINE Vector3 Vector4::GetHomogenized3D       ()                                          const
{
    return Vector3
    (
        m_x / m_w,
        m_y / m_w,
        m_z / m_w
    );
}

/**
 * Get the magnitude (length) of this vector.
 *
 * @return The magnitude of this vector.
 */
FORCEINLINE float   Vector4::GetMagnitude           ()                                          const
{
    return  Math::Sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w * m_w);
}

/**
 * Get the squared magnitude of this vector.
 *
 * @return The squared magnitude of this vector.
 */
FORCEINLINE float   Vector4::GetMagnitudeSquared    ()                                          const
{
    return  m_x*m_x + m_y*m_y + m_z*m_z + m_w * m_w;
}

/**
 * Gets a normalized copy of the vector, checking it is safe to do so based on the length.
 * Returns zero vector if vector length is too small to safely normalize.
 *
 * @param p_tolerance   Minimum squared vector length.

 * @return              A normalized copy if safe, (0,0,0,0) otherwise.
 */
FORCEINLINE Vector4 Vector4::GetSafeNormalized      (float const    p_tolerance)                const
{
    float const magnitudeSquared    {m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w};

    if      (magnitudeSquared == 1.0f)
        return *this;

    else if (magnitudeSquared < p_tolerance)
        return Vector4::Zero;

    float const magnitude           {Math::Sqrt(magnitudeSquared)};
    return Vector4(m_x/ magnitude, m_y / magnitude, m_z/ magnitude, m_w/magnitude);
} 

/**
 * Calculates normalized version of vector without checking for zero length.
 *
 * @return Normalized version of vector.
 */
FORCEINLINE Vector4 Vector4::GetUnsafeNormalized    ()                                          const
{
    float const magnitude   {Math::Sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w)};
   
    return Vector4(m_x / magnitude, m_y / magnitude, m_z / magnitude, m_w / magnitude);
}

/** Homogenize this vector (The three first components are divided by w) */
FORCEINLINE void    Vector4::Homogenize             ()
{
    m_x /= m_w;
    m_y /= m_w;
    m_z /= m_w;
    m_w =  1.0f;   
}

/**
 * Checks whether vector is near to zero within a specified tolerance.
 *
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the vector is near to zero, false otherwise.
 */
FORCEINLINE bool    Vector4::IsNearlyZero           (float const    p_tolerance)                const
{
   return   Math::IsNearlyZero(m_x, p_tolerance) &&
            Math::IsNearlyZero(m_y, p_tolerance) &&
            Math::IsNearlyZero(m_z, p_tolerance) &&
            Math::IsNearlyZero(m_w, p_tolerance);
}

/**
 * Checks whether vector is normalized.
 *
 * @return  True if Normalized, false otherwise.
 */
FORCEINLINE bool    Vector4::IsNormalized           ()                                          const
{
    return Math::IsNearlyZero(1.0f - GetMagnitudeSquared());
}

/**
 * Check if the vector is of unit length, with specified tolerance.
 *
 * @param p_lengthSquaredTolerance  Tolerance against squared length.
 *
 * @return                          True if the vector is a unit vector within the specified tolerance.
 */
FORCEINLINE bool    Vector4::IsUnit                 (float const    p_lengthSquaredTolerance)   const
{
    return Math::Abs(1.0f - GetMagnitudeSquared()) < p_lengthSquaredTolerance;
}

/**
 * Checks whether all components of the vector are exactly zero.
 *
 * @return  True if the vector is exactly zero, false otherwise.
 */
FORCEINLINE bool    Vector4::IsZero                 ()                                          const
{
    return  m_x == 0.0f &&
            m_y == 0.0f &&
            m_z == 0.0f &&
            m_w == 0.0f;
}

/**
 * Normalize this vector in-place if it is larger than a given tolerance. Leaves it unchanged if not.
 *
 * @param p_tolerance   The minimum squared length of vector for normalization.

 * @return              True if the vector was normalized correctly, false otherwise.
 */
FORCEINLINE bool    Vector4::Normalize              (float const    p_tolerance)
{
    float const magnitudeSquared    {m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w};

    if (magnitudeSquared > p_tolerance)
    {
        float const scale   {Math::InvSqrt(magnitudeSquared)};
        m_x *= scale; 
        m_y *= scale; 
        m_z *= scale;
        m_w *= scale;
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
 * @param p_w   The new W coordinate.
 */
FORCEINLINE void    Vector4::Set                    (float const    p_x,
                                                     float const    p_y,
                                                     float const    p_z,
                                                     float const    p_w)
{
    m_x = p_x;
    m_y = p_y;
    m_z = p_z;
    m_w = p_w;
}

/**
 * Set the values of the vector directly.
 *
 * @param p_vector2 The Vector2 to copy (X,Y value).
 * @param p_z       The new Z coordinate (Default values = 0.0f).
 * @param p_w       The new W coordinate (Default values = 0.0f).
 */
FORCEINLINE void    Vector4::Set                    (Vector2 const& p_vector2,
                                                     float const    p_z,
                                                     float const    p_w)
{
    m_x = p_vector2.m_x;
    m_y = p_vector2.m_y;
    m_z = p_z;
    m_w = p_w;
}

/**
 * Set the values of the vector directly.
 *
 * @param p_vector3 The Vector3 to copy (X,Y,Z value).
 * @param p_w       The new W coordinate (Default values = 0.0f).
 */
FORCEINLINE void    Vector4::Set                    (Vector3 const& p_vector3,
                                                     float const    p_w)
{
    m_x = p_vector3.m_x;
    m_y = p_vector3.m_y;
    m_z = p_vector3.m_z;
    m_w = p_w;
}

/**
 * Set the values of the vector directly.
 *
 * @param p_value   The new value of each components.
 *
 */
FORCEINLINE void    Vector4::Set                    (float const    p_value)
{
    m_x = p_value;
    m_y = p_value;
    m_z = p_value;
    m_w = p_value;
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
FORCEINLINE bool        Vector4::operator== (Vector4 const& p_vector)   const
{
    return m_x == p_vector.m_x && m_y == p_vector.m_y && m_z == p_vector.m_z && m_w == p_vector.m_w;
}

/**
  * Check against another vector for inequality.
  *
  * @param p_vector  The vector to check against.
  *
  * @return          True    if the vectors are different.
  * @return          False   if the vectors are equal.
  */
FORCEINLINE bool        Vector4::operator!= (Vector4 const& p_vector)   const
{
    return m_x != p_vector.m_x || m_y != p_vector.m_y || m_z != p_vector.m_z || m_w != p_vector.m_w;
}

/**
 * Copy another vector into this one
 *
 * @param p_copy    The other vector to copy.
 *
 * @return          The reference to vector after copy.
 */
FORCEINLINE Vector4&    Vector4::operator=  (Vector4 const& p_copy)
{
    m_x = p_copy.m_x;
    m_y = p_copy.m_y;
    m_z = p_copy.m_z;
    m_w = p_copy.m_w;

    return *this;
}

/**
 * Copy a Vector3 into this one (W is set to 0.0f)
 *
 * @param p_copy    The Vector3 to copy.
 *
 * @return          The reference to vector after copy.
 */
FORCEINLINE Vector4&    Vector4::operator=  (Vector3 const& p_copy)
{
    m_x = p_copy.m_x;
    m_y = p_copy.m_y;
    m_z = p_copy.m_z;
    m_w = 0.0f;

    return *this;
}

/**
 * Copy a Vector2 into this one (Z and W is set to 0.0f)
 *
 * @param p_copy    The Vector3 to copy.
 *
 * @return          The reference to vector after copy.
 */
FORCEINLINE Vector4&    Vector4::operator=  (Vector2 const& p_copy)
{
    m_x = p_copy.m_x;
    m_y = p_copy.m_y;
    m_z = 0.0f;
    m_w = 0.0f;

    return *this;
}

/**
 * Move another vector into this one
 *
 * @param p_vector  The vector to move.
 *
 * @return          The reference to vector after move.
 */
FORCEINLINE Vector4&    Vector4::operator=  (Vector4&&      p_move)
{
    m_x = std::move(p_move.m_x);
    m_y = std::move(p_move.m_y);
    m_z = std::move(p_move.m_z);
    m_w = std::move(p_move.m_w);

    return *this;
}

/**
 * Gets specific component of the vector by index.
 *
 * @param p_index   The index of vector component. Must be int [0,3] range.
 *
 * @return          The copy of the component corresponding to the index given.
 */
FORCEINLINE float       Vector4::operator[] (int32 const    p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Vector4's index range : [0,3]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else if (p_index == 2)
        return m_z;
    else
        return m_w;
}

/**
 * Gets specific component of the vector.
 *
 * @param p_index   The index of vector component. Must be int [0,3] range.
 *
 * @return          The reference to component corresponding to the index given.
 */
FORCEINLINE float&      Vector4::operator[] (int32 const    p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Vector4's index range : [0,3]");

    if      (p_index == 0)
        return m_x;
    else if (p_index == 1)
        return m_y;
    else if (p_index == 2)
        return m_z;
    else
        return m_w;
}

/**
 * Gets the result of component-wise addition of this and another vector.
 *
 * @param p_vector  The vector to add to this.
 *
 * @return          The result of vector addition.
 */
FORCEINLINE Vector4     Vector4::operator+  (Vector4 const& p_vector)   const
{
    return Vector4
    (
        m_x + p_vector.m_x, 
        m_y + p_vector.m_y,
        m_z + p_vector.m_z,
        m_w + p_vector.m_w
    );
}

/**
 * Gets the result of adding to each component of the vector.
 *
 * @param p_value   How much to add to each component.
 *
 * @return          The result of addition.
 */
FORCEINLINE Vector4     Vector4::operator+  (float const    p_value)    const
{
    return Vector4
    (
        m_x + p_value, 
        m_y + p_value,
        m_z + p_value,
        m_w + p_value
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
FORCEINLINE Vector4&    Vector4::operator+= (Vector4 const& p_vector)
{
    m_x += p_vector.m_x;
    m_y += p_vector.m_y;
    m_z += p_vector.m_z;
    m_w += p_vector.m_w;

    return *this;
}

 /**
  * Adds a value to each component of the vector
  *
  * @param p_value   How much to add to each component.
  *
  * @return          The reference of the vector after addition.
  */
FORCEINLINE Vector4&    Vector4::operator+= (float const    p_value)
{
    m_x += p_value;
    m_y += p_value;
    m_z += p_value;
    m_w += p_value;

    return *this;
}

/**
 * Get a negated copy of the vector.
 *
 * @return          A negated copy of the vector.
 */
FORCEINLINE Vector4     Vector4::operator-  ()                          const
{
    return Vector4(-m_x, -m_y, -m_z, -m_w);
}

/**
 * Gets the result of component-wise subtraction of this by another vector.
 *
 * @param p_vector  The vector to subtract from this.
 *
 * @return          The result of vector subtraction.
 */
FORCEINLINE Vector4     Vector4::operator-  (Vector4 const& p_vector)   const
{
    return Vector4
    (
        m_x - p_vector.m_x, 
        m_y - p_vector.m_y,
        m_z - p_vector.m_z,
        m_w - p_vector.m_w
    );
}

/**
 * Gets the result of subtracting from each component of the vector.
 *
 * @param p_value   How much to subtract from each component.
 *
 * @return          The result of subtraction.
 */
FORCEINLINE Vector4     Vector4::operator-  (float const    p_value)    const
{
    return Vector4
    (
        m_x - p_value, 
        m_y - p_value,
        m_z - p_value,
        m_w - p_value
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
FORCEINLINE Vector4&    Vector4::operator-= (Vector4 const& p_vector)
{
    m_x -= p_vector.m_x;
    m_y -= p_vector.m_y;
    m_z -= p_vector.m_z;
    m_w -= p_vector.m_w;

    return *this;
}

/**
 * Substracts a value to each component of the vector
 *
 * @param p_value   How much to subtract from each component.
 *
 * @return          The reference of the vector after subtraction.
 */
FORCEINLINE Vector4&    Vector4::operator-= (float const    p_value)
{
    m_x -= p_value;
    m_y -= p_value;
    m_z -= p_value;
    m_w -= p_value;

    return *this;
}

/**
 * Gets the result of component-wise multiplication of this vector by another.
 *
 * @param p_vector  The vector to multiply with.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Vector4     Vector4::operator*  (Vector4 const& p_vector)   const
{
    return Vector4
    (
        m_x * p_vector.m_x, 
        m_y * p_vector.m_y,
        m_z * p_vector.m_z,
        m_w * p_vector.m_w
    );
}

/**
 * Gets the result of scaling the vector (multiplying each component by a value).
 *
 * @param p_scale   What to multiply each component by.
 *
 * @return          The result of multiplication.
 */
FORCEINLINE Vector4     Vector4::operator*  (float const    p_scale)    const
{
    return Vector4
    (
        m_x * p_scale, 
        m_y * p_scale,  
        m_z * p_scale,
        m_w * p_scale
    );    
}

/**
 * Multiplies the vector with another vector, using component-wise multiplication.
 *
 * @param p_vector  What to multiply this vector with.
 *
 * @return          The reference of the vector after multiplication.
 */
FORCEINLINE Vector4&    Vector4::operator*= (Vector4 const& p_vector)
{
    m_x *= p_vector.m_x;
    m_y *= p_vector.m_y;
    m_z *= p_vector.m_z;
    m_w *= p_vector.m_w;

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
FORCEINLINE Vector4&    Vector4::operator*= (float const    p_scale)
{
    m_x *= p_scale;
    m_y *= p_scale;
    m_z *= p_scale;
    m_w *= p_scale;

    return *this;
}

/**
 * Gets the result of component-wise division of this vector by another.
 *
 * @param p_vector  The vector to divide by.
 *
 * @return          The result of division.
 */
FORCEINLINE Vector4     Vector4::operator/  (Vector4 const& p_vector)   const
{
    return Vector4
    (   
        m_x / p_vector.m_x, 
        m_y / p_vector.m_y,
        m_z / p_vector.m_z,
        m_w / p_vector.m_w
    );
}

/**
 * Gets the result of dividing each component of the vector by a value.
 *
 * @param p_scale   What to divide each component by.
 *
 * @return          The result of division.
 */
FORCEINLINE Vector4     Vector4::operator/  (float const    p_scale)    const
{
    return Vector4
    (
        m_x / p_scale, 
        m_y / p_scale,  
        m_z / p_scale,
        m_w / p_scale
    );    
}

/**
 * Divides the vector by another vector, using component-wise division.
 *
 * @param p_vector  The vector to divide by.
 *
 * @return          The reference of the vector after division.
 */
FORCEINLINE Vector4&    Vector4::operator/= (Vector4 const& p_vector)
{
    m_x /= p_vector.m_x;
    m_y /= p_vector.m_y;
    m_z /= p_vector.m_z;
    m_w /= p_vector.m_w;

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
FORCEINLINE Vector4&    Vector4::operator/= (float const    p_scale)
{
    m_x /= p_scale;
    m_y /= p_scale;
    m_z /= p_scale;
    m_w /= p_scale;

    return *this;
}

/**
 * Calculate the dot product between this and another vector.
 *
 * @param p_vector  The other vector.
 *
 * @return          The result of dot product.
 */
FORCEINLINE float       Vector4::operator|  (Vector4 const& p_vector)   const
{
    return m_x*p_vector.m_x + m_y*p_vector.m_y + m_z*p_vector.m_z + m_w * p_vector.m_w;
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
FORCEINLINE Vector4 operator+   (float const    p_value,
                                 Vector4 const& p_vector)
{
    return Vector4
    (
        p_vector.m_x + p_value, 
        p_vector.m_y + p_value,
        p_vector.m_z + p_value,
        p_vector.m_w + p_value
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
FORCEINLINE Vector4 operator*   (float const    p_scale,
                                 Vector4 const& p_vector)
{
    return Vector4
    (
        p_vector.m_x * p_scale, 
        p_vector.m_y * p_scale,  
        p_vector.m_z * p_scale,
        p_vector.m_w * p_scale
    );
}

#endif // !__MATH_VECTOR_4_INL__