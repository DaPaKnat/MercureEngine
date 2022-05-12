#include "PCH.hpp"

#include "Mathematic/Transform.hpp"

// ==============================[Public Static Properties]============================== //

Transform const Transform::Identity {Vector3::Zero, Quaternion::Identity, Vector3::One};

// ==============================[Public Local Methods]============================== //

/**
 * Computes the relative transform from a Transform given into the referential transform given.
 *
 * @param p_referential The referental transform.
 * @param p_transform   The transform to translate in the referential of p_referential.
 *
 * @return The p_transform in the referential of p_referential.
 */
Transform   Transform::GetRelative                      (Transform const&   p_referential,
                                                         Transform const&   p_transform)
{
    Transform result;

    if (p_referential.HasScaleWithNegativeValue() || p_transform.HasScaleWithNegativeValue())
    {
        result = GetRelativeUsingMatrixWithScale(p_referential, p_transform);
    }

    else
    {
        Vector3 const       safeScaleReciprocal {Vector3::SafeScaleReciprocal(p_referential.m_scale)};
        Quaternion const    inverseRefRotation  {p_referential.m_rotation.GetInverse()};

        result.m_scale          = p_transform.m_scale * safeScaleReciprocal;
        result.m_rotation       = inverseRefRotation * p_transform.m_rotation;
        result.m_translation    = (inverseRefRotation * (p_transform.m_translation - p_referential.m_translation)) * safeScaleReciprocal;
    }

    return result;
}