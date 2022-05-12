#ifndef __PLANE_INL__
#define __PLANE_INL__

/*==============================[Public Static Methods]==============================*/

/**
 * Check if two Plane are nearly equal within a specified tolerance.
 *
 * @param p_a           The first vector to check against.
 * @param p_b           The second vector to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the planes are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool    Plane::IsNearlyEqual    (Plane const&   p_a,
                                         Plane const&   p_b,
                                         float const    p_tolerance)
{
    return  Vector3::IsNearlyEqual(p_a.m_normal, p_b.m_normal, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_distance, p_b.m_distance, p_tolerance);

}

/*==============================[Constructors + Destructor]==============================*/

/** Default constructor (No initialization) */
FORCEINLINE Plane::Plane    ()
:   m_normal    {},
    m_distance  {}
{}

/**
 * Constructor initializing all components to a single float value.
 *
 * @param p_xNormal     The value corresponding to X component of the normal.
 * @param p_yNormal     The value corresponding to Y component of the normal.
 * @param p_zNormal     The value corresponding to Z component of the normal.
 * @param p_distance    The value corresponding the distance from the plane to the origin of the space.
 */
FORCEINLINE Plane::Plane    (float const    p_xNormal,
                             float const    p_yNormal,
                             float const    p_zNormal,
                             float const    p_distance)
:   m_normal    {p_xNormal, p_yNormal, p_zNormal},
    m_distance  {p_distance}
{}

/**
 * Constructor using a Vector4 where X,Y,Z corresponding to the component of the normal and W as the distance from the plane to the origin of the space.
 *
 * @param p_vector4     The Vector4 used as normal and distance value.
 */
FORCEINLINE Plane::Plane    (Vector4 const& p_vector4)
:   m_normal    {p_vector4.m_x, p_vector4.m_y, p_vector4.m_z},
    m_distance  {p_vector4.m_w}
{} 

/**
 * Constructor using three Vector3 as point to create a plane (This points will belong to this Plane).
 *
 * @param p_pointA  The first point in the plane.
 * @param p_pointB  The S=second point in the plane.
 * @param p_pointC  The third point in the plane.
 *
 */
FORCEINLINE Plane::Plane    (Vector3 const& p_pointA,
                             Vector3 const& p_pointB,
                             Vector3 const& p_pointC)
:   m_normal    {((p_pointB - p_pointA) ^ (p_pointC - p_pointA)).GetSafeNormalized()},
    m_distance  {-(m_normal | p_pointA)}
{}

/**
 * Constructor using one Vector3 as the normal of the plane (Assumed to be unit)
 * and an another one as a point belonging to the plan.
 *
 * @param p_normal      The vector corresponding to the normal of the plane (Assumed to be unit).
 * @param p_distance    The value corresponding the distance from the plane to the origin of the space.
 *
 */
FORCEINLINE Plane::Plane    (Vector3 const& p_normal,
                             Vector3 const& p_point)
:   m_normal    {p_normal},
    m_distance  {-(p_normal | p_point)}
{}

/**
 * Constructor using a Vector3 as the normal of the plane (Assumed to be unit)
 * and a float point value as distance from the plane to the origin of the space.
 *
 * @param p_normal      The vector corresponding to the normal of the plane (Assumed to be unit).
 * @param p_distance    The value corresponding the distance from the plane to the origin of the space.
 *
 */
FORCEINLINE Plane::Plane    (Vector3 const& p_normal,
                             float const    p_distance)
:   m_normal    {p_normal},
    m_distance  {p_distance}
{}

/**
 * Copy constructor using initial plane.
 *
 * @param p_copy    The initial plane to copy.
 */
FORCEINLINE Plane::Plane    (Plane const&   p_copy)
:   m_normal    {p_copy.m_normal},
    m_distance  {p_copy.m_distance}
{}

/**
 * Move constructor using initial plane for each component.
 *
 * @param p_move    The initial plane to move.
 */
FORCEINLINE Plane::Plane    (Plane&&        p_move)
:   m_normal    {std::move(p_move.m_normal)},
    m_distance  {std::move(p_move.m_distance)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 *  Make the plane face in the opposite direction.
 */
FORCEINLINE void    Plane::Flip                     ()
{
    m_normal    = -m_normal;
    m_distance  = -m_distance;
}

/**
 * Computes the point projected on this plane.
 *
 * @param p_vector  The point to project.
 *
 * @return          The point projected on this plane.
 */
FORCEINLINE Vector3 Plane::GetProjectedPoint        (Vector3 const& p_point)        const
{
    return p_point - (m_normal * ((m_normal | p_point) + m_distance));
}

/**
 * Computes the vector projected on this plane.
 *
 * @param p_vector  The vector to project.
 *
 * @return          The vector projected on this plane.
 */
FORCEINLINE Vector3 Plane::GetProjectedVector       (Vector3 const& p_vector)       const
{
    return Vector3::ProjectOnPlane(p_vector, m_normal);
}

/**
 * Computes the shortest distance between a point and this plane.
 * This distance is signed:
 * - distance > 0 : The point is above the plane
 * - distance = 0 : The point is included in the plane
 * - distance < 0 : The point is below the plane
 *
 * @param p_point   The point with which we compute the distance.
 *
 * @return          The signed distance between the point and this plane.
 */
FORCEINLINE float   Plane::GetPointSignedDistance   (Vector3 const& p_point)        const
{
    return (p_point | m_normal) + m_distance;
}

/**
 * Computes the shortest distance between a point and this plane.
 *
 * @param p_point   The point with which we compute the distance.
 *
 * @return          The shortest distance between the point and this plane.
 */
FORCEINLINE float   Plane::GetPointDistance         (Vector3 const& p_point)        const
{
	return Math::Abs(m_normal | p_point) / m_normal.GetMagnitude();
}

/**
 * Compute the plane facing in the opposite direction of this plane.
 *
 * @return The plane facing in the opposite direction of this.
 */
FORCEINLINE Plane   Plane::GetFlipped               ()                              const
{
    return Plane (-m_normal, -m_distance);
}

/**
 * Compute the translated plane of this.
 *
 * @param p_translation The translation vector.
 *
 * @return              The translated plane.
 */
FORCEINLINE Plane   Plane::GetTranslated            (Vector3 const& p_translation)  const
{
    return Plane
    (
        m_normal,
        m_distance + (m_normal | p_translation)
    );
}

/**
 * Check if two points are in the same side of the plane or include.
 *
 * @return True if the two points are in the same side or include, false otherwise.
 */
FORCEINLINE bool    Plane::IsSameSide               (Vector3 const& p_pointA,
                                                     Vector3 const& p_pointB)       const
{
    float const distanceA   {(p_pointA | m_normal) + m_distance};
    float const distanceB   {(p_pointB | m_normal) + m_distance};

    return (distanceA > 0.0f && distanceB > 0.0f) || (distanceA <= 0.0f && distanceB <= 0.0f);
}

/**
 * Translate this plane.
 *
 * @param p_translation The translation vector.
 */
FORCEINLINE void    Plane::Translate                (Vector3 const& p_translation)
{
    m_distance += (m_normal | p_translation);
}

/*==============================[Public Local Operators]==============================*/

FORCEINLINE bool    Plane::operator==   (Plane const&   p_plane)        const
{
    return  m_normal == p_plane.m_normal &&
            m_distance == p_plane.m_distance;
}

FORCEINLINE bool    Plane::operator!=   (Plane const&   p_plane)        const
{
    return  m_normal != p_plane.m_normal ||
            m_distance != p_plane.m_distance;
}

FORCEINLINE Plane&  Plane::operator=    (Plane const&   p_plane)
{
	if (p_plane == *this)
		return *this;

    m_normal    = p_plane.m_normal;
    m_distance  = p_plane.m_distance;

    return *this;
}

FORCEINLINE Plane&  Plane::operator=    (Plane&&        p_move)
{
	if (p_move == *this)
		return *this;

    m_normal    = std::move(p_move.m_normal);
    m_distance  = std::move(p_move.m_distance);

	return *this;
}

#endif // !__PLANE_INL__