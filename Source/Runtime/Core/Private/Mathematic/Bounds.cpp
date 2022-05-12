#include "PCH.hpp"

#include "Mathematic/Bounds.hpp"

// ==============================[Public Local Methods]====================== //

/**
 * Compute a bounding box overlapping two other bounding box
 *
 * @return  The overlappping bounding box.
 */
Bounds  Bounds::Overlap (Bounds const&  p_a,
                         Bounds const&  p_b)
{
    Vector3 max;
    Vector3 min;

    max.m_x = Math::Max(p_a.m_max.m_x, p_b.m_max.m_x);
    max.m_y = Math::Max(p_a.m_max.m_y, p_b.m_max.m_y);
    max.m_z = Math::Max(p_a.m_max.m_z, p_b.m_max.m_z);

    min.m_x = Math::Min(p_a.m_min.m_x, p_b.m_min.m_x);
    min.m_y = Math::Min(p_a.m_min.m_y, p_b.m_min.m_y);
    min.m_z = Math::Min(p_a.m_min.m_z, p_b.m_min.m_z);

    return Bounds(min, max);
}