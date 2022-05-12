#ifndef __BOUNDS_INL__
#define __BOUNDS_INL__

// ==============================[Public Static Methods]====================== //

/**
 * Construct a Bounding box with an orgin and an extent given.
 *
 * @param The origin of the bounding box.
 * @param The extent of the bounding box.
 *
 * @return The bounding box created with the parameters given.
 */
FORCEINLINE Bounds  Bounds::Build   (Vector3 const&	p_origin,
							         Vector3 const& p_extent)
{
	return Bounds(p_origin - p_extent, p_origin + p_extent);
}

// ==============================[Public Constructor + Destructor]====================== //

/**
 * Default Constructor (Initialize min and max as Zero vector).
 */
FORCEINLINE Bounds::Bounds  ()
:   m_max   {Vector3::Zero},
    m_min   {Vector3::Zero}
{}

/**
 * Constructor using two vector to initialize min and max component.
 *
 * @param p_min The box's minimum point.
 * @param p_max The box's maximum point.
 */
FORCEINLINE Bounds::Bounds  (Vector3 const& p_min,
                             Vector3 const& p_max)
:   m_max   {p_max},
    m_min   {p_min}
{}

/**
 * Copy Constructor.
 */
FORCEINLINE Bounds::Bounds  (Bounds const&  p_copy)
:   m_max   {p_copy.m_max},
    m_min   {p_copy.m_min}
{}

/**
 * Move Constructor.
 */
FORCEINLINE Bounds::Bounds  (Bounds&&		p_move)
:   m_max   {std::move(p_move.m_max)},
    m_min   {std::move(p_move.m_min)}
{}

// ==============================[Public Local Methods]====================== //

/**
 * Increase the size of the bounding box.
 *
 * @param p_value   The size value to use to compute the increased bounding box.
 */
FORCEINLINE void    Bounds::Expand              (float const    p_value)
{
    m_min -= p_value; 
    m_max += p_value;
}

/**
 * Increase the size of the bounding box.
 *
 * @param p_value   The size vector to use to compute the increased bounding box.
 */
FORCEINLINE void    Bounds::Expand              (Vector3 const& p_value)
{
    m_min -= p_value;
    m_max += p_value;
}

/**
 * Increase the size of the bounding box.
 *
 * @param p_minExpand The size to applied on min vector. (Positive value move the vector outwards)
 * @param p_maxExpand The size to applied on max vector. (Positive value move the vector outwards)
 */
FORCEINLINE void    Bounds::Expand              (Vector3 const& p_minExpand,
                                                 Vector3 const& p_maxExpand)
{
    m_min -= p_minExpand;
    m_max += p_maxExpand;
}

/**
 * Gets the center and the extens of this bounding box.
 *
 * @param p_center  The center of this bounding box [OUT]
 * @param p_extemt  The extent of this bounding vox [OUT]
 */
FORCEINLINE void    Bounds::GetCenterAndExtent  (Vector3&       p_center,
                                                 Vector3&       p_extent)   const
{
    p_extent = GetExtent();
    p_center = m_min + p_extent;
    
}

/**
 * Computes the closest point on or inside the bounding box to a givent point in space.
 *
 * @param p_point   The point is space.
 *
 * @return The closest point on or inside the bounding box.
 */
FORCEINLINE Vector3 Bounds::GetClosestPoint     (Vector3 const& p_point)    const
{
    return Vector3
    (
        Math::Clamp(p_point.m_x, m_min.m_x, m_max.m_x),
        Math::Clamp(p_point.m_y, m_min.m_y, m_max.m_y),
        Math::Clamp(p_point.m_z, m_min.m_z, m_max.m_z)
    );
}

/**
 * Computes the shortest distance from a point to the bounding box.
 * 
 * @param The point to compute the distance with.
 *
 * @return The distance from the point given to the bounding box.
 */
FORCEINLINE float   Bounds::GetDistance         (Vector3 const& p_point)    const
{
    return Math::Sqrt(GetDistanceSquared(p_point));
}

/**
 * Computes the shortest squared distance from a point to the bounding box.
 * 
 * @param The point to compute the squared distance with.
 *
 * @return The squared distance from the point given to the bounding box.
 */
FORCEINLINE	float	Bounds::GetDistanceSquared	(Vector3 const& p_point)	const
{
	float distanceSquared	{0.0f};

	if		(p_point.m_x < m_min.m_x)
	{
		distanceSquared += Math::Square(p_point.m_x - m_min.m_x);
	}

	else if (p_point.m_x > m_max.m_x)
	{
		distanceSquared += Math::Square(p_point.m_x - m_max.m_x);
	}

	if		(p_point.m_y < m_min.m_y)
	{
		distanceSquared += Math::Square(p_point.m_y - m_min.m_y);
	}

	else if (p_point.m_y > m_max.m_y)
	{
		distanceSquared += Math::Square(p_point.m_y - m_max.m_y);
	}

	if		(p_point.m_z < m_min.m_z)
	{
		distanceSquared += Math::Square(p_point.m_z - m_min.m_z);
	}

	else if (p_point.m_z > m_max.m_z)
	{
		distanceSquared += Math::Square(p_point.m_z - m_max.m_z);
	}

	return distanceSquared;
}

/**
 * Gets the center point of this bounding box.
 *
 * @return  The center point of this.
 */
FORCEINLINE Vector3 Bounds::GetCenter           ()  const
{
    return (m_min + m_max) * 0.5f;
}

/**
 * Computes a bounding box with the size increased with a value given.
 *
 * @param p_value   The size value to use to compute the increased bounding box.
 *
 * @return The bounding box with increased value applied.
 */
FORCEINLINE Bounds  Bounds::GetExpanded         (float const    p_value)    const
{
    return Bounds(m_min - p_value, m_max + p_value);
}

/**
* Computes a bounding box with the size increased with a value given.
 *
 * @param p_value   The size vector to use to compute the increased bounding box.
 *
 * @return The bounding box with increased value applied.
 */
FORCEINLINE Bounds  Bounds::GetExpanded         (Vector3 const& p_value)    const
{
    return Bounds(m_min - p_value, m_max + p_value);
}

/**
* Computes a bounding box with the size increased with a value given.
 *
 * @param p_minExpand The size to applied on min vector. (Positive value move the vector outwards)
 * @param p_maxExpand The size to applied on max vector. (Positive value move the vector outwards)
 *
 * @return The bounding box with increased value applied.
 */
FORCEINLINE Bounds  Bounds::GetExpanded         (Vector3 const& p_minExpand,
                                                 Vector3 const& p_maxExpand)    const
{
    return Bounds(m_min - p_minExpand, m_max + p_maxExpand);
}

/**
 * Get the extents of this bounding box.
 * (Corresponds to the half of the size of this bounding box)
 *
 * @return The extens of this.
 */
FORCEINLINE Vector3 Bounds::GetExtent           ()  const
{
    return (m_max - m_min) * 0.5f;
}

/**
 * Computes this bounding box moved in another location.
 *
 * @param p_destination The destination desired.
 *
 * @return The bounding box moved to the desired location.
 */
FORCEINLINE Bounds  Bounds::GetMoved            (Vector3 const& p_destination) const
{
    Vector3 const offset    {p_destination - GetCenter()};
    
    return Bounds(m_min + offset, m_max + offset);
}

/**
 * Get the size of this of this bounding box.
 *
 * @return The size of this.
 */
FORCEINLINE Vector3 Bounds::GetSize             ()  const
{
    return m_max - m_min;
}

/**
 * Computes the volume of this bounding box.
 *
 * @return The volume of this.
 */
FORCEINLINE float   Bounds::GetVolume           ()  const
{
    return (m_max.m_x - m_min.m_x) * (m_max.m_y - m_min.m_y) * (m_max.m_z - m_min.m_z);
}

/**
 * Checks wheter the given location is inside this bounding box.
 *
 * @param p_point   The location to check with.
 *
 * @return True if the location is inside the bounding box, otherwise false.
 */
FORCEINLINE bool    Bounds::IsInside            (Vector3 const& p_point)    const
{
    return  (p_point.m_x > m_min.m_x) && (p_point.m_x < m_max.m_x) &&
            (p_point.m_y > m_min.m_y) && (p_point.m_y < m_max.m_y) &&
            (p_point.m_z > m_min.m_z) && (p_point.m_z < m_max.m_z);
}

/**
 * Checks whether a given bounding box is fully encapsulated by this bounding box.
 *
 * @param p_other   The bounding box to check if it's fully ecapsulated.
 */
FORCEINLINE bool    Bounds::IsInside            (Bounds const&  p_other)    const
{
    return IsInside(p_other.m_min) && IsInside(p_other.m_max);
}

/**
 * Checks wheter the given location is inside of on this bounding box.
 *
 * @param p_point   The location to check with.
 *
 * @return True if the location is inside or on the bounding box, otherwise false.
 */
FORCEINLINE bool    Bounds::IsInsideOrOn        (Vector3 const& p_point)    const
{
    return  (p_point.m_x >= m_min.m_x) && (p_point.m_x <= m_max.m_x) &&
            (p_point.m_y >= m_min.m_y) && (p_point.m_y <= m_max.m_y) &&
            (p_point.m_z >= m_min.m_z) && (p_point.m_z <= m_max.m_z);
}

/**
 * Checks wheter the given bounding box intersects this bounding box.
 *
 * @param p_other   The bounding box to intersect with.
 *
 * @return True if the bounding boxes intersect, false otherwise.
 */
FORCEINLINE bool    Bounds::IsIntersecting      (Bounds const&  p_other)    const
{
    return !(
                (m_min.m_x > p_other.m_max.m_x) || (p_other.m_min.m_x > m_max.m_x) ||
                (m_min.m_y > p_other.m_max.m_y) || (p_other.m_min.m_y > m_max.m_y) ||
                (m_min.m_z > p_other.m_max.m_z) || (p_other.m_min.m_z > m_max.m_z) 
            );
}

/**
 * Check if this is a valid bounding box.
 *
 * @return True, if Max > Min
 */
FORCEINLINE bool    Bounds::IsValid             ()                          const
{
    return  m_max.m_x > m_min.m_x ||
            m_max.m_y > m_min.m_y ||
            m_max.m_z > m_min.m_z;
}

/**
 * Moves this bounding box to a desired location.
 *
 * @param p_destination The desired location of this bounding box.
 */
FORCEINLINE void    Bounds::Move                (Vector3 const& p_destination)
{
    Vector3 const offset    {p_destination - GetCenter()};
    
    m_max += offset;
    m_min += offset;
}

/**
 * Shift the location of this bounding box
 *
 * @param p_offset  The vector to shift the bounding box with.
 */
FORCEINLINE void    Bounds::Shift               (Vector3 const& p_offset)
{
    m_max += p_offset;
    m_min += p_offset;
}

// ==============================[Public Local Operators]====================== //

/**
 * Checks if both bounding boxes are equivalent.
 *
 * @param p_other   The other bounding box to compare with.
 *
 * @return True if both bounding boxes are equivalent.
 */
FORCEINLINE bool    Bounds::operator==  (Bounds const&  p_other)    const 
{
    return m_max == p_other.m_max && m_min == p_other.m_min;
}

/**
 * Checks if both bounding boxes are different.
 *
 * @param p_other   The other bounding box to compare with.
 *
 * @return True if both bounding boxes are different.
 */
FORCEINLINE bool    Bounds::operator!=  (Bounds const&  p_other)    const 
{
    return m_max != p_other.m_max || m_min != p_other.m_min;
}

/**
 * Copy an another bounding box into this one.
 *
 * @param p_other   The other bounding box to copy.
 *
 * @return The reference of this after copy.
 */
FORCEINLINE Bounds& Bounds::operator=   (Bounds const&  p_copy)
{
    m_max = p_copy.m_max;
    m_min = p_copy.m_min;

    return *this;
}

/**
 * Move an another bounding box into this one.
 *
 * @param p_other   The other bounding box to move.
 *
 * @return The reference of this after move.
 */
FORCEINLINE Bounds& Bounds::operator=   (Bounds&&       p_move)
{
    m_max = std::move(p_move.m_max);
    m_min = std::move(p_move.m_min);

    return *this;
}

#endif // !__BOUNDS_INL__
