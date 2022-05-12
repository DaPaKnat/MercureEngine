#ifndef __QUATERNION_INL__
#define __QUATERNION_INL__

/*==============================[Public Static Methods]==============================*/

/** 
 * Find the angular distance between two rotation quaternions (in degree) 
 *
 * @param p_a   The first quaternion.
 * @param p_b   The second quaternion.
 *
 * @return      The angle between p_a and p_b in degree.
 */
FORCEINLINE float       Quaternion::AngleDeg            (Quaternion const&  p_a,
                                                         Quaternion const&  p_b)
{
    return Math::RadToDeg(AngleRad(p_a,p_b));
}

/**
 * Find the angular distance between two rotation quaternions (in radians)
 *
 * @param p_a   The first quaternion.
 * @param p_b   The second quaternion.
 *
 * @return      The angle between p_a and p_b in radian.
 */
FORCEINLINE float       Quaternion::AngleRad            (Quaternion const&  p_a,
                                                         Quaternion const&  p_b)
{
    return Math::Acos(2*Math::Square(p_a|p_b) - 1);
}
/**
 * Computes a quaternion from the a rotation around the given axis.
 *
 * @param p_axis        The vector to rotate around. (The axis is assumed to be a normalized vector)
 * @param p_angleRad    angle to rotate above the given axis (in Degree)
 */
FORCEINLINE Quaternion  Quaternion::AxisAngleDeg        (Vector3 const&     p_axis,
                                                         float const        p_angleDeg)
{
    return AxisAngleRad(p_axis, Math::DegToRad(p_angleDeg));
}

/**
 * Computes a quaternion from the a rotation around the given axis.
 *
 * @param p_axis        The vector to rotate around. (The axis is assumed to be a normalized vector)
 * @param p_angleRad    angle to rotate above the given axis (in radian)
 */
FORCEINLINE Quaternion  Quaternion::AxisAngleRad        (Vector3 const&     p_axis,
                                                         float const        p_angleRad)
{
    float const halfAngleRad    {p_angleRad * 0.5f};
    float const sinHalfAngle    {Math::Sin(halfAngleRad)};

    return Quaternion
    (
        sinHalfAngle * p_axis.m_x,
        sinHalfAngle * p_axis.m_y,
        sinHalfAngle * p_axis.m_z,
        Math::Cos(halfAngleRad)
    );
}

/**
 * Compute the dot product of two quaternions.
 *
 * @param p_a   The first quaternion.
 * @param p_b   The second quaternion.
 *
 * @return      The result of dot product.
 */
FORCEINLINE float       Quaternion::Dot                 (Quaternion const&  p_a,
                                                         Quaternion const&  p_b)
{
    return p_a | p_b;
}
/**
 * Computes the smallest rotation betweem two normals (Assumed to be unit length)
 *
 * @param p_fromDirection   The start direction before rotation (Assumed to be unit length).
 * @param p_toDirection     The final direction after rotation (Assumed to be unit length).
 *
 * @return                  The rotation which rotate from p_fromDirection to p_toDirection.
 */
FORCEINLINE Quaternion  Quaternion::FindBetweenNormals  (Vector3 const&  p_fromDirection,
                                                             Vector3 const&  p_toDirection)
{
    float const w           {1 + (p_fromDirection | p_toDirection)};

    Quaternion  result;

    if (w >= MATH_EPSILON_F)
    {
       result.Set(  p_fromDirection.m_y * p_toDirection.m_z - p_fromDirection.m_z * p_toDirection.m_y,
					p_fromDirection.m_z * p_toDirection.m_x - p_fromDirection.m_x * p_toDirection.m_z,
					p_fromDirection.m_x * p_toDirection.m_y - p_fromDirection.m_y * p_toDirection.m_x,
					w);
    }

    else // p_fromDirection & p_toDirection point in opposite direction
    {
        (Math::Abs(p_fromDirection.m_x) > Math::Abs(p_fromDirection.m_y))
            ? result.Set(-p_fromDirection.m_z, 0.0f, p_fromDirection.m_x, 0.0f)
            : result.Set(0.0f, -p_fromDirection.m_z, p_fromDirection.m_y, 0.0f);
    }

    result.Normalize();
    return result;
}

/**
* Computes the smallest rotation betweem two vectors of arbitrary length.
*
* @param p_fromDirection   The start direction before rotation.
* @param p_toDirection     The final direction after rotation.
*
* @return                  The rotation which rotate from p_fromDirection to p_toDirection.
*/
FORCEINLINE Quaternion  Quaternion::FindBetweenVectors  (Vector3 const&  p_fromDirection,
                                                         Vector3 const&  p_toDirection)
{
    float const normFromTo  {Math::Sqrt(p_fromDirection.GetMagnitudeSquared() * p_toDirection.GetMagnitudeSquared())};
    float const w           {normFromTo + (p_fromDirection | p_toDirection)};

    Quaternion  result;

    if (w >= MATH_EPSILON_F * normFromTo)
    {
       result.Set(  p_fromDirection.m_y * p_toDirection.m_z - p_fromDirection.m_z * p_toDirection.m_y,
					p_fromDirection.m_z * p_toDirection.m_x - p_fromDirection.m_x * p_toDirection.m_z,
					p_fromDirection.m_x * p_toDirection.m_y - p_fromDirection.m_y * p_toDirection.m_x,
					w);
    }

    else // p_fromDirection & p_toDirection point in opposite direction
    {
        (Math::Abs(p_fromDirection.m_x) > Math::Abs(p_fromDirection.m_y))
            ? result.Set(-p_fromDirection.m_z, 0.0f, p_fromDirection.m_x, 0.0f)
            : result.Set(0.0f, -p_fromDirection.m_z, p_fromDirection.m_y, 0.0f);
    }

    result.Normalize();
    return result;
}

/**
 * Check if two quaternions are nearly equal within a specified tolerance.
 * (Both quaternion can represent the same rotation but be not Equal, 
 * this function check if the components are nearly equal).
 *
 * @param p_a           The first quaternion to check against.
 * @param p_b           The second quaternion to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the quaternions are equal within tolerance limits, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsNearlyEqual       (Quaternion const&  p_a,
                                                         Quaternion const&  p_b,
                                                         float const        p_tolerance)
{
    return  Math::IsNearlyEqual(p_a.m_x, p_b.m_x, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_y, p_b.m_y, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_z, p_b.m_z, p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_w, p_b.m_w, p_tolerance);
}

/**
 * Check if two quaternions represent the same rotation with a tolerance.
 * 
 *
 * @param p_a           The first quaternion to check against.
 * @param p_b           The second quaternion to check against.
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the quaternions represent the same rotation, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsNearlySame        (Quaternion const&  p_a,
                                                         Quaternion const&  p_b,
                                                         float const        p_tolerance)
{
    return Math::IsNearlyZero(1.0f - Math::Abs(p_a | p_b), p_tolerance); 
}

/**
 * Check if two quaternions represent the same rotation.
 *
 * @param p_a           The first quaternion to check against.
 * @param p_b           The second quaternion to check against.
 *
 * @return              True if the quaternions represent the same rotation within tolerance limits, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsSame              (Quaternion const&  p_a,
                                                         Quaternion const&  p_b)
{
    return Math::Abs((p_a | p_b)) == 1.0f; 
}

/**
 * Linearly Quaternion interpolates between (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed quaternion
 * @param p_b       The right handed quaternion
 * @param p_alpha   The interpolation value.

 * return           The interpolation of p_a and p_b by p_alpha.The result is not normalized !
 */
FORCEINLINE Quaternion  Quaternion::Lerp                (Quaternion const&  p_a,
                                                         Quaternion const&  p_b,
                                                         float const        p_alpha)
{
    float const dot     {p_a|p_b};
    float const bias    {Math::FloatSelect(p_a|p_b, 1.0f, -1.0f)};
    return (p_b* p_alpha) + (p_a * (bias * (1.0f - p_alpha)));
}

/**
 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
 * 
 * @param p_eulerDegree The Euler angles in degress.
 *
 * @return              The quaternions corresponding to the Euler angles rotations.
 */
FORCEINLINE Quaternion  Quaternion::MakeFromEulerDeg    (Vector3 const&     p_eulerDegree)
{
    Vector3 halfAngle   {Math::DegToRad(p_eulerDegree.m_x * 0.5f),
                         Math::DegToRad(p_eulerDegree.m_y * 0.5f),
                         Math::DegToRad(p_eulerDegree.m_z * 0.5f)};

    float   cosX  {Math::Cos(halfAngle.m_x)};
    float   cosY  {Math::Cos(halfAngle.m_y)};
    float   cosZ  {Math::Cos(halfAngle.m_z)};

    float   sinX  {Math::Sin(halfAngle.m_x)};
    float   sinY  {Math::Sin(halfAngle.m_y)};
    float   sinZ  {Math::Sin(halfAngle.m_z)};

    return Quaternion
    (
        sinX * cosY * cosZ + cosX * sinY * sinZ,
        cosX * sinY * cosZ - sinX * cosY * sinZ,
        cosX * cosY * sinZ - sinX * sinY * cosZ,
        cosX * cosY * cosZ + sinX * sinY * sinZ
    );
}

/**
 * Convert a vector of floating-point Euler angles (in degrees) into a Quaternion.
 * 
 * @param p_xAngleDegree    The euler angle around X Axis (in degrees)
 * @param p_yAngleDegree    The euler angle around Y Axis (in degrees)
 * @param p_zAngleDegree    The euler angle around Z Axis (in degrees)
 *
 * @return                  The quaternions corresponding to the Euler angles rotations.

 */
FORCEINLINE Quaternion  Quaternion::MakeFromEulerDeg    (float const        p_xAngleDegree,
                                                         float const        p_yAngleDegree,
                                                         float const        p_zAngleDegree)
{
    Vector3 halfAngle   {Math::DegToRad(p_xAngleDegree* 0.5f),
                         Math::DegToRad(p_yAngleDegree* 0.5f),
                         Math::DegToRad(p_zAngleDegree* 0.5f)};

    float   cosX  {Math::Cos(halfAngle.m_x)};
    float   cosY  {Math::Cos(halfAngle.m_y)};
    float   cosZ  {Math::Cos(halfAngle.m_z)};

    float   sinX  {Math::Sin(halfAngle.m_x)};
    float   sinY  {Math::Sin(halfAngle.m_y)};
    float   sinZ  {Math::Sin(halfAngle.m_z)};

    return Quaternion
    (
        sinX * cosY * cosZ + cosX * sinY * sinZ,
        cosX * sinY * cosZ - sinX * cosY * sinZ,
        cosX * cosY * sinZ - sinX * sinY * cosZ,
        cosX * cosY * cosZ + sinX * sinY * sinZ
    );
}

/**
 * Convert a vector of floating-point Euler angles (in radians) into a Quaternion.
 * 
 * @param p_eulerDegree The Euler angles in radians.
 *
 * @return              The quaternions corresponding to the Euler angles rotations.
 */
FORCEINLINE Quaternion  Quaternion::MakeFromEulerRad    (Vector3 const&     p_eulerRadian)
{
    Vector3 halfAngle   {p_eulerRadian.m_x * 0.5f,
                         p_eulerRadian.m_y * 0.5f,
                         p_eulerRadian.m_z * 0.5f};

    float   cosX  {Math::Cos(halfAngle.m_x)};
    float   cosY  {Math::Cos(halfAngle.m_y)};
    float   cosZ  {Math::Cos(halfAngle.m_z)};

    float   sinX  {Math::Sin(halfAngle.m_x)};
    float   sinY  {Math::Sin(halfAngle.m_y)};
    float   sinZ  {Math::Sin(halfAngle.m_z)};

    return Quaternion
    (
        sinX * cosY * cosZ + cosX * sinY * sinZ,
        cosX * sinY * cosZ - sinX * cosY * sinZ,
        cosX * cosY * sinZ - sinX * sinY * cosZ,
        cosX * cosY * cosZ + sinX * sinY * sinZ
    );
}

/**
 * Convert a vector of floating-point Euler angles (in radians) into a Quaternion.
 * 
 * @param p_xAngleDegree    The euler angle around X Axis (in radians)
 * @param p_yAngleDegree    The euler angle around Y Axis (in radians)
 * @param p_zAngleDegree    The euler angle around Z Axis (in radians)
 *
 * @return                  The quaternions corresponding to the Euler angles rotations.
 */
FORCEINLINE Quaternion  Quaternion::MakeFromEulerRad    (float const        p_xAngleRadian,
                                                         float const        p_yAngleRadian,
                                                         float const        p_zAngleRadian)
{
    Vector3 halfAngle   {p_xAngleRadian* 0.5f,
                         p_yAngleRadian* 0.5f,
                         p_zAngleRadian* 0.5f};

    float   cosX  {Math::Cos(halfAngle.m_x)};
    float   cosY  {Math::Cos(halfAngle.m_y)};
    float   cosZ  {Math::Cos(halfAngle.m_z)};

    float   sinX  {Math::Sin(halfAngle.m_x)};
    float   sinY  {Math::Sin(halfAngle.m_y)};
    float   sinZ  {Math::Sin(halfAngle.m_z)};

    return Quaternion
    (
        sinX * cosY * cosZ + cosX * sinY * sinZ,
        cosX * sinY * cosZ - sinX * cosY * sinZ,
        cosX * cosY * sinZ - sinX * sinY * cosZ,
        cosX * cosY * cosZ + sinX * sinY * sinZ
    );
}

/**
 * Linearly Quaternion interpolates between (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed quaternion
 * @param p_b       The right handed quaternion
 * @param p_alpha   The interpolation value.

 * return           The interpolation of p_a and p_b by p_alpha.The result is normalized !
 */
FORCEINLINE Quaternion  Quaternion::NLerp               (Quaternion const&  p_a,
                                                         Quaternion const&  p_b,
                                                         float const        p_alpha)
{
    return Quaternion::Lerp(p_a,p_b,p_alpha).GetSafeNormalized();
}
/**
 * Spherical Quaternion interpolates between (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed quaternion
 * @param p_b       The right handed quaternion
 * @param p_alpha   The interpolation value.
 *
 * return           The spherical interpolation of p_a and p_b by p_alpha.The result is not normalized !
 */
FORCEINLINE Quaternion  Quaternion::NSlerp              (Quaternion const&  p_a,
                                                         Quaternion const&  p_b,
                                                         float const        p_alpha)
{
    return Quaternion::Slerp(p_a, p_b, p_alpha).GetSafeNormalized();
}

/**
 * Rotates a rotation p_from towards p_to.
 * The from quaternion is rotated towards to by an angular step of maxDegreesDelta (but note that the rotation will not overshoot).
 * Negative values of maxDegreesDelta will move away from to until the rotation is exactly the opposite direction.
 *
 * @param p_from            The initial rotation.
 * @param p_to              The final rotation.
 * @param p_maxDegreeDelta  The delta angle in degree.
 *
 * @return                  The new rotation with delta applied.
 */
FORCEINLINE Quaternion  Quaternion::RotateTowardsDeg    (Quaternion const&  p_from,
                                                         Quaternion const&  p_to,
                                                         float const        p_maxDegreeDelta)
{
    return RotateTowardsRad(p_from, p_to, Math::DegToRad(p_maxDegreeDelta));
}

/**
 * Rotates a rotation p_from towards p_to.
 * The from quaternion is rotated towards to by an angular step of maxDegreesDelta (but note that the rotation will not overshoot). 
 * Negative values of maxDegreesDelta will move away from to until the rotation is exactly the opposite direction.
 *
 * @param p_from            The initial rotation.
 * @param p_to              The final rotation.
 * @param p_maxDegreeDelta  The delta angle in radian.
 *
 * @return                  The new rotation with delta applied.        
 */
FORCEINLINE Quaternion  Quaternion::RotateTowardsRad    (Quaternion const&  p_from,
                                                         Quaternion const&  p_to,
                                                         float const        p_maxRadianDelta)
{
    float const angle   {AngleRad(p_from, p_to)};
    if (Math::IsNearlyZero(angle))
        return p_to;

    return Slerp(p_from, p_to, Math::Min(1.0f, p_maxRadianDelta / angle));
}

/**
 * Spherical Quaternion interpolates between (p_alpha is not clamped between 0 and 1)
 *
 * @param p_a       The left handed quaternion
 * @param p_b       The right handed quaternion
 * @param p_alpha   The interpolation value.
 *
 * return           The spherical interpolation of p_a and p_b by p_alpha.The result is normalized !
 */
FORCEINLINE Quaternion  Quaternion::Slerp               (Quaternion const& p_a,
                                                         Quaternion const& p_b,
                                                         float const       p_alpha)
{
    float const rawCosTheta {p_a | p_b};
    float const cosTheta    {Math::FloatSelect(rawCosTheta, rawCosTheta , -rawCosTheta)};

    float       scale0;
    float       scale1;

    if (cosTheta < (1.0f - MATH_EPSILON_F))
    {
        float const theta       {Math::Acos(cosTheta)};
        float const invSinTheta {1.0f/Math::Sin(theta)};

        scale0 = Math::Sin((1.f - p_alpha) * theta) * invSinTheta;
        scale1 = Math::Sin(p_alpha * theta) * invSinTheta;
    }
    else
    {
        // Use linear interpolation.
        scale0 = 1.0f - p_alpha;
        scale1 = p_alpha;
    }
    
    // In keeping with our flipped Cosom:
    scale1 = Math::FloatSelect(rawCosTheta, scale1, -scale1);

    return  p_a * scale0 + p_b * scale1;
} 

/*==============================[Constructors + Destructor]==============================*/

/** Default constructor, all components are initialized to 0.0f */
FORCEINLINE Quaternion::Quaternion  ()
:   m_x {0.0f},
    m_y {0.0f},
    m_z {0.0f},
    m_w {0.0f}
{}

/**
 * Constructor using values for each component.
 *
 * @param p_x   X component of the quaternion
 * @param p_y   Y component of the quaternion
 * @param p_z   Z component of the quaternion
 * @param p_w   W component of the quaternion
 */
FORCEINLINE Quaternion::Quaternion  (float const        p_x,
                                     float const        p_y,
                                     float const        p_z,
                                     float const        p_w)
:   m_x {p_x},
    m_y {p_y},
    m_z {p_z},
    m_w {p_w}
{}

/**
 * Creates and initializes a new quaternion from the a rotation around the given axis.
 *
 * @param p_axis        The vector to rotate around. (The axis is assumed to be a normalized vector)
 * @param p_angleRad    angle to rotate above the given axis (in radians)
 */
FORCEINLINE Quaternion::Quaternion  (Vector3 const&     p_axis,
                                     float const        p_angleRad)
{
    float const halfAngleRad    {p_angleRad * 0.5f};
    float const sinHalfAngle    {Math::Sin(halfAngleRad)};

    m_x = sinHalfAngle * p_axis.m_x;
    m_y = sinHalfAngle * p_axis.m_y;
    m_z = sinHalfAngle * p_axis.m_z;
    m_w = Math::Cos(halfAngleRad);
}

/**
 * Copy constructor.
 *
 * @param p_copy    A Quaternion object to use to create new quaternion from.
 */
FORCEINLINE Quaternion::Quaternion  (Quaternion const&  p_copy)
:   m_x {p_copy.m_x},
    m_y {p_copy.m_y},
    m_z {p_copy.m_z},
    m_w {p_copy.m_w}
{}

/**
 * Move constructor.
 *
 * @param p_copy    A Quaternion object to use to create new quaternion from.
 */
FORCEINLINE Quaternion::Quaternion  (Quaternion&&       p_move)
:   m_x {std::move(p_move.m_x)},
    m_y {std::move(p_move.m_y)},
    m_z {std::move(p_move.m_z)},
    m_w {std::move(p_move.m_w)}
{}

/*==============================[Public Local Methods]==============================*/

/**
 * Conjugate this quaternion.
 * If the magnitude of this quaternion is 1, the conjugate is equal to the inverse;
 *
 * Tips:
 *
 * Quaternion * Quaternion.GetConjugate() = Quaternion::Identity (Only if Quaternion.GetMagnitude() == 1)
 * Quaternion.GetConjugate() * Quaternion = Quaternion::Identity (Only if Quaternion.GetMagnitude() == 1)
 */
FORCEINLINE void        Quaternion::Conjugate           ()
{
    m_x = -m_x;
    m_y = -m_y;
    m_z = -m_z;
}

/** 
 * Compute the angle of this quaternion in degree.
 *
 *  return  Angle of this quaternion in degree.
 */
FORCEINLINE float       Quaternion::GetAngleDeg         ()                              const
{
    return Math::RadToDeg(2.0f * Math::Acos(m_w));
}

/**
 * Compute the angle of this quaternion in radian.
 *
 * @return  Angle of this quaternion in radian.
 */
FORCEINLINE float       Quaternion::GetAngleRad         ()                              const
{
    return 2.0f * Math::Acos(m_w);
}

/** Get the right direction (+X axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetAxisX            ()                              const
{
    return RotateVector(Vector3::Right);
}

/** Get the up direction (+Y axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetAxisY            ()                              const
{
    return RotateVector(Vector3::Up);
}

/** Get the forward direction (+Z axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetAxisZ            ()                              const
{
    return RotateVector(Vector3::Forward);
}

/** Get the backward direction (-Z axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetBackward         ()                              const
{
    return -GetForward();
}

/**
 * Get the conjugate of this quaternion.
 * If the magnitude of this quaternion is 1, the conjugate is equal to the inverse;
 * 
 * Tips:
 *
 * Quaternion * Quaternion.GetConjugate() = Quaternion::Identity (Only if Quaternion.GetMagnitude() == 1)
 * Quaternion.GetConjugate() * Quaternion = Quaternion::Identity (Only if Quaternion.GetMagnitude() == 1)
 *
 * @return  The conjugate of this quaternion
 */
FORCEINLINE Quaternion  Quaternion::GetConjugate        ()                              const
{
    return Quaternion(-m_x, -m_y, -m_z, m_w);
}

/** Get the down direction (-Y axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetDown             ()                              const
{
    return -GetUp();
}


/** Convert a Quaternion into floating-point Euler angles (in degrees). */
FORCEINLINE Vector3     Quaternion::GetEulerDeg         ()                              const
{
    float const xSquared    {Math::Square(m_x)};
    float const ySquared    {Math::Square(m_y)};
    float const zSquared    {Math::Square(m_z)};

    return Vector3
    (
        Math::RadToDeg(Math::Asin((2.0f * (m_x*m_w - m_y*m_z)))),
        Math::RadToDeg(Math::Atan2(2.0f * (m_x*m_z + m_y*m_w), 1.0f - 2.0f * (xSquared + ySquared))),
        Math::RadToDeg(Math::Atan2(2.0f * (m_x*m_y + m_z*m_w), 1.0f - 2.0f * (xSquared + zSquared)))
    );
}

/** Convert a Quaternion into floating-point Euler angles (in radians). */
FORCEINLINE Vector3     Quaternion::GetEulerRad         ()                              const
{
    float const xSquared    {Math::Square(m_x)};
    float const ySquared    {Math::Square(m_y)};
    float const zSquared    {Math::Square(m_z)};

    return Vector3
    (
        Math::Asin((2.0f * (m_x*m_w - m_y * m_z))),
        Math::Atan2(2.0f * (m_x*m_z + m_y*m_w), 1.0f - 2.0f * (xSquared + ySquared)),
        Math::Atan2(2.0f * (m_x*m_y + m_z*m_w), 1.0f - 2.0f * (xSquared + zSquared))
    );
}

/** Get the forward direction (+Z axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetForward          ()                              const
{
    return GetAxisZ();
}

/**
 * Get the inverse of this quaternion.
 * If the magnitude of this quaternion is 1, the inverse is equal to the conjugate
 * 
 * Tips: 
 * 
 * Quaternion * Quaternion.GetInverse() = Quaternion::Identity
 * Quaternion.GetInverse() * Quaternion = Quaternion::Identity
 *
 * @return  The inverse of this quaternion
 */
FORCEINLINE Quaternion  Quaternion::GetInverse          ()                              const
{
    return GetConjugate() / GetMagnitudeSquared();
}

/** Get the down direction (-X axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetLeft             ()                              const
{
    return -GetRight();
}

/**
 * Get the magnitude of this quaternion.
 *
 * @return  The magnitude of this quaternion.
 */
FORCEINLINE float       Quaternion::GetMagnitude        ()                              const
{
    return Math::Sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);
}

/**
 * Get the magnitude squared of this quaternion.
 *
 * @return  The magnitude squared of this quaternion.
 */
FORCEINLINE float       Quaternion::GetMagnitudeSquared ()                              const
{
    return m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w;
}

/** Get the right direction (+X axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetRight            ()                              const
{
    return GetAxisX();
}

/**
 * Get the rotation axis of this quaternion.
 *
 * @return the rotation axis of this quaternion.
 */
FORCEINLINE Vector3     Quaternion::GetRotationAxis     ()                              const
{
	float const sqrt = Math::Sqrt(Math::Max(1.0f - (m_w*m_w), 0.0f));

	if (!Math::IsNearlyZero(sqrt))
	{ 
		return Vector3(m_x / sqrt, m_y / sqrt, m_z / sqrt);
	} 

	return Vector3::Right;
}

/**
 * Gets a normalized copy of the quaternion, checking it is safe to do so based on the length.
 * Returns identity quaternion if quaternion magnitude is too small to safely normalize.
 *
 * @param p_tolerance   Minimum squared quaternion length.

 * @return              A normalized copy if safe, (0,0,0,1) otherwise.
 */
FORCEINLINE Quaternion  Quaternion::GetSafeNormalized   (float const    p_tolerance)    const
{
    float const magnitudeSquared {GetMagnitudeSquared()};
    
    if (magnitudeSquared >= p_tolerance)
    {
        float const magnitudeInvSqrt {Math::InvSqrt(magnitudeSquared)};
        return Quaternion(m_x*magnitudeInvSqrt, m_y*magnitudeInvSqrt, m_z*magnitudeInvSqrt, m_w*magnitudeInvSqrt);
    }

    else
        return Quaternion(0,0,0,1);
}

/**
 * COmputes normalized version of quaternion without checking for zero length.
 *
 * @return Normalized version of quaternion.
 */
FORCEINLINE Quaternion  Quaternion::GetUnsafeNormalized ()                              const
{
    float const magnitude {GetMagnitude()};

    return Quaternion(m_x/magnitude, m_y/magnitude, m_z/magnitude, m_w/magnitude);
}

/** Get the up direction (+Y axis) after it has been rotated by this Quaternion. */
FORCEINLINE Vector3     Quaternion::GetUp               ()                              const
{
    return GetAxisY();
}

/**
 * Inverse this quaternion
 * If the magnitude of this quaternion is 1, the inverse is equal to the conjugate
 *
 * Tips:
 *
 * Quaternion * Quaternion.GetInverse() = Quaternion::Identity
 * Quaternion.GetInverse() * Quaternion = Quaternion::Identity
 */
FORCEINLINE void        Quaternion::Inverse             ()
{
    float const magnitudeSquared    {GetMagnitudeSquared()};

    m_x /= -magnitudeSquared;
    m_y /= -magnitudeSquared;
    m_z /= -magnitudeSquared;
    m_w /= magnitudeSquared;
}

/**
 * Checks whether this Quaternion is an Identity Quaternion.
 *
 * @param p_tolerance   Error tolerance for comparison with Identity Quaternion.
 * @return              True if Quaternion is a normalized Identity Quaternion.
 */
FORCEINLINE bool        Quaternion::IsIdentity          (float const    p_tolerance)    const
{
    return Quaternion::IsNearlyEqual(Quaternion::Identity, *this, p_tolerance);
}

/**
 * Checks whether quaternion is near to zero within a specified tolerance.
 *
 * @param p_tolerance   The error tolerance.
 *
 * @return              True if the quaternion is near to zero, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsNearlyZero        (float const    p_tolerance)    const
{
    return  Math::IsNearlyZero(m_x, p_tolerance) &&
            Math::IsNearlyZero(m_y, p_tolerance) &&
            Math::IsNearlyZero(m_z, p_tolerance) &&
            Math::IsNearlyZero(m_w, p_tolerance);

}

/**
 * Checks whether quaternion is normalized.
 *
 * @return  True if Normalized, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsNormalized        ()                              const
{
    return Math::IsNearlyZero(1.0f - GetMagnitudeSquared());
}

/**
 * Checks whether all components of the quaternion are exactly zero.
 *
 * @return  True if the quaternion is exactly zero, false otherwise.
 */
FORCEINLINE bool        Quaternion::IsZero              ()                              const
{
    return  m_x == 0.0f &&
            m_y == 0.0f &&
            m_z == 0.0f &&
            m_w == 0.0f;
}

/**
 * Normalize this quaternion if it is larger than a given tolerance. Leaves it unchanged if not.
 *
 * @param p_tolerance   The minimum squared length of quaternion for normalization.

 * @return              True if the quaternion was normalized correctly, false otherwise.
 */
FORCEINLINE bool        Quaternion::Normalize           (float const    p_tolerance)
{
    float const magnitudeSquared {GetMagnitudeSquared()};
    
    if (magnitudeSquared >= p_tolerance)
    {
        float const magnitudeInvSqrt {Math::InvSqrt(magnitudeSquared)};
        m_x *= magnitudeInvSqrt;
        m_y *= magnitudeInvSqrt;
        m_z *= magnitudeInvSqrt; 
        m_w *= magnitudeInvSqrt;
        return true;
    }

    else
        return false;
}

/**
 * Rotate a vector by this quaternion.
 *
 * @param p_vector  The vector to be rotated
 *
 * @return          The vector after rotation.
 */
FORCEINLINE Vector3     Quaternion::RotateVector        (Vector3 const& p_vector)       const
{
    /**
     *  http://people.csail.mit.edu/bkph/articles/axisuaternions.pdf
     *  result      = p_vector + 2w(axis x p_vector) + (2axis x (axis x p_vector))
     *
	 *  refactor:
     *
	 *  result      = p_vector + w(2(axis x p_vector)) + (axis x (2(axis x p_vector)))
	 *  T           = 2(axis x p_vector);
	 *  p_vector    = p_vector + w*(T) + (axis x T)
     */
    Vector3 const axis  {m_x, m_y, m_z};
	Vector3 const T     {2.0f * (axis ^ p_vector)};

	return p_vector + (m_w * T) + (axis ^ T);
}

/**
 * Set components with 4 values given.
 *
 * @param p_x   New X component of the quaternion
 * @param p_y   New Y component of the quaternion
 * @param p_z   New Z component of the quaternion
 * @param p_w   New W component of the quaternion
 */
FORCEINLINE void        Quaternion::Set                 (float const    p_x,
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
 * Get the axis and angle of rotation of this quaternion : assumes normalized quaternions.
 *
 * @param p_outAxis     The rotation axis of this quaternion
 * @param p_outAngleDeg The rotation angle of this quaternion (In Degree)
 */
FORCEINLINE void        Quaternion::ToAxisAndAngleDeg   (Vector3&       p_outAxis,
                                                         float&         p_outAngleDeg)   const
{
    p_outAxis       = GetRotationAxis();
    p_outAngleDeg   = GetAngleDeg();
}

/**
 * Get the axis and angle of rotation of this quaternion : assumes normalized quaternions.
 *
 * @param p_outAxis     The rotation axis of this quaternion
 * @param p_outAngleDeg The rotation angle of this quaternion (In Radian)
 */
FORCEINLINE void        Quaternion::ToAxisAndAngleRad   (Vector3&       p_outAxis,
                                                         float&         p_outAngleRad)   const
{
    p_outAxis       = GetRotationAxis();
    p_outAngleRad   = GetAngleRad();
}

/**
 * Rotate a vector by the inverse this quaternion.
 *
 * @param p_vector  The vector to be rotated
 *
 * @return          The vector after rotation by the inverse of this quaternion.
 */
FORCEINLINE Vector3     Quaternion::UnrotateVector      (Vector3 const& p_vector)       const
{
    Vector3 const axis  {-m_x, -m_y, -m_z};
	Vector3 const T     {2.0f * (axis ^ p_vector)};

	return p_vector + (m_w * T) + (axis ^ T);
}

/*==============================[Public Local Operators]==============================*/

/**
 * Checks whether two quaternions are identical.
 * (Both quaternion can represent the same rotation but be not Equal,
 * this function check if the components are nearly equal).
 *
 * @param p_quaternion The other quaternion.
 *
 * @return             True if two quaternion are identical, otherwise false.
 */
FORCEINLINE bool        Quaternion::operator==  (Quaternion const&  p_quaternion)   const
{
    return  m_x == p_quaternion.m_x && 
            m_y == p_quaternion.m_y && 
            m_z == p_quaternion.m_z && 
            m_w == p_quaternion.m_w;
}

/**
 * Checks whether two quaternions are not identical.
 *
 * @param p_quaternion The other quaternion.
 *
 * @return             True if two quaternion are not identical, otherwise false.
 */
FORCEINLINE bool        Quaternion::operator!=  (Quaternion const&  p_quaternion)   const
{
    return  m_x != p_quaternion.m_x || 
            m_y != p_quaternion.m_y || 
            m_z != p_quaternion.m_z || 
            m_w != p_quaternion.m_w;
}

/**
 * Copy another Quaternion into this one
 *
 * @return  reference to this Quaternion after move.
 */
FORCEINLINE Quaternion& Quaternion::operator=   (Quaternion const&  p_other)
{
    m_x = p_other.m_x;
    m_y = p_other.m_y;
    m_z = p_other.m_z;
    m_w = p_other.m_w;

    return *this;
}

/**
 * Move another Quaternion into this one
 *
 * @return  reference to this Quaternion after move.
 */
FORCEINLINE Quaternion& Quaternion::operator=   (Quaternion&&       p_other)
{
    m_x = std::move(p_other.m_x);
    m_y = std::move(p_other.m_y);
    m_z = std::move(p_other.m_z);
    m_w = std::move(p_other.m_w);

    return *this;
}

/**
 * Gets specific component of the quaternion by index.
 *
 * @param p_index   The index of quaternion component. Must be int [0,3] range.
 *
 * @return          The copy of the component corresponding to the index given.
 */
FORCEINLINE float       Quaternion::operator[] (int32 const         p_index)    const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Quaternion's index range : [0,3]");

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
 * Gets specific component of the quaternion.
 *
 * @param p_index   The index of quaternion component. Must be int [0,3] range.
 *
 * @return          The reference to component corresponding to the index given.
 */
FORCEINLINE float&      Quaternion::operator[] (int32 const         p_index)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Quaternion's index range : [0,3]");

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
 * Gets the result of adding a Quaternion to this.
 * This is a component-wise addition; composing quaternions should be done via multiplication.
 *
 * @param p_quaternion  The Quaternion to add.
 *
 * @return              The result of addition.
 */
FORCEINLINE Quaternion  Quaternion::operator+   (Quaternion const&  p_quaternion)   const
{
    return Quaternion
    (
        m_x + p_quaternion.m_x,
        m_y + p_quaternion.m_y,
        m_z + p_quaternion.m_z,
        m_w + p_quaternion.m_w
    );
}

/**
 * Adds to this quaternion.
 * This is a component-wise addition; composing quaternions should be done via multiplication.
 *
 * @param p_quaternion  The quaternion to add to this.
 *
 * @return              Result after addition.
 */
FORCEINLINE Quaternion& Quaternion::operator+=  (Quaternion const&  p_quaternion)
{
    m_x += p_quaternion.m_x;
    m_y += p_quaternion.m_y;
    m_z += p_quaternion.m_z;
    m_w += p_quaternion.m_w;

    return *this;
}

/**
 * Get a negated copy of the quaternion.
 *
 * @return          A negated copy of the quaternion.
 */
FORCEINLINE Quaternion  Quaternion::operator-   ()                                  const
{
    return Quaternion(-m_x, -m_y, -m_z, -m_w);
}

/**
 * Gets the result of subtracting a Quaternion to this.
 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
 *
 * @param p_quaternion  The Quaternion to subtract.
 *
 * @return              The result of subtraction.
 */
FORCEINLINE Quaternion  Quaternion::operator-   (Quaternion const&  p_quaternion)   const
{
    return Quaternion
    (
        m_x - p_quaternion.m_x,
        m_y - p_quaternion.m_y,
        m_z - p_quaternion.m_z,
        m_w - p_quaternion.m_w
    );
}

/**
 * Subtracts another quaternion from this.
 * This is a component-wise subtraction; composing quaternions should be done via multiplication.
 *
 * @param p_quaternion  The other quaternion.
 *
 * @return              reference to this after subtraction.
 */
FORCEINLINE Quaternion& Quaternion::operator-=  (Quaternion const&  p_quaternion)
{
    m_x -= p_quaternion.m_x;
    m_y -= p_quaternion.m_y;
    m_z -= p_quaternion.m_z;
    m_w -= p_quaternion.m_w;

    return *this;
}

/**
 * Gets the result of multiplying this by another quaternion (this * Q).
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 *
 * @param p_quaternion  The Quaternion to multiply this by.
 *
 * @return              The result of multiplication (this * Q).
 */
FORCEINLINE Quaternion  Quaternion::operator*   (Quaternion const&  p_quaternion)   const
{
    return Quaternion
    (
        m_w*p_quaternion.m_x + m_x*p_quaternion.m_w + m_y*p_quaternion.m_z - m_z*p_quaternion.m_y,
        m_w*p_quaternion.m_y + m_y*p_quaternion.m_w + m_z*p_quaternion.m_x - m_x*p_quaternion.m_z,
        m_w*p_quaternion.m_z + m_z*p_quaternion.m_w + m_x*p_quaternion.m_y - m_y*p_quaternion.m_x,
        m_w*p_quaternion.m_w - m_x*p_quaternion.m_x - m_y*p_quaternion.m_y - m_z*p_quaternion.m_z
    );
}

/**
 * Get the result of scaling this quaternion.
 *
 * @param p_scale   The scaling factor.
 *
 * @return          The result of scaling.
 */
FORCEINLINE Quaternion  Quaternion::operator*   (float const        p_scale)        const
{
    return Quaternion
    (
        m_x * p_scale,
        m_y * p_scale,
        m_z * p_scale,
        m_w * p_scale
    );
}

/**
 * Rotate a vector by this quaternion.
 * Same as RotateVector.
 *
 * @param p_vector  The vector to be rotated
 *
 * @return          The vector after rotation.
 */
FORCEINLINE Vector3     Quaternion::operator*   (Vector3 const&     p_vector)       const
{
    return RotateVector(p_vector);
}

/**
 * Multiply this by a quaternion (this = this * Q).
 *
 * Order matters when composing quaternions: C = A * B will yield a quaternion C that logically
 * first applies B then A to any subsequent transformation (right first, then left).
 *
 * @param p_quaternion  The quaternion to multiply this by.
 *
 * @return              The result of multiplication (this * Q).
 */
FORCEINLINE Quaternion& Quaternion::operator*=  (Quaternion const&  p_quaternion)
{
    *this = std::move(*this * p_quaternion);
    
    return *this;
}

/**
 * Multiply this quaternion by a scaling factor.
 *
 * @param p_scale   The scaling factor.
 *
 * @return          A reference to this after scaling.
 */
FORCEINLINE Quaternion& Quaternion::operator*=  (float const        p_scale)
{
    m_x *= p_scale;
    m_y *= p_scale;
    m_z *= p_scale;
    m_w *= p_scale;
    
    return *this;
}

/**
 * Divide this quaternion by scale.
 *
 * @param p_scale   What to divide by.
 *
 * @return          New Quaternion of this after division by scale.
 */
FORCEINLINE Quaternion  Quaternion::operator/   (float const        p_scale)        const
{
    return Quaternion
    (
        m_x / p_scale,
        m_y / p_scale,
        m_z / p_scale,
        m_w / p_scale
    );
}

/**
 * Divide this quaternion by scale.
 *
 * @param p_scale   What to divide by.
 *
 * @return          A reference to this after scaling.
 */
FORCEINLINE Quaternion& Quaternion::operator/=  (float const        p_scale)
{
    m_x /= p_scale;
    m_y /= p_scale;
    m_z /= p_scale;
    m_w /= p_scale;
    
    return *this;
}

/**
 * Computes dot product of two quaternions.
 *
 * @param p_quaternion  The other quaternions.
 *
 * @return              The dot product.
 */
FORCEINLINE float       Quaternion::operator|   (Quaternion const&  p_quaternion)   const
{
    return m_x * p_quaternion.m_x + m_y * p_quaternion.m_y + m_z * p_quaternion.m_z + m_w * p_quaternion.m_w;
}

/*==============================[Public Globale Operators]==============================*/

/**
 * Get the result of scaling of quaternion given.
 *
 * @param p_scale       The scaling factor.
 * @param p_quaternion  The quaternion to scale.
 *
 * @return              The result of scaling.
 */
FORCEINLINE Quaternion  operator*   (float const        p_scale,
                                     Quaternion const&  p_quaternion)
{
    return Quaternion
    (
        p_quaternion.m_x * p_scale,
        p_quaternion.m_y * p_scale,
        p_quaternion.m_z * p_scale,
        p_quaternion.m_w * p_scale
    );
}

#endif // !__QUATERNION_INL__
