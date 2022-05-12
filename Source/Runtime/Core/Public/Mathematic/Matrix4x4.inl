#ifndef __MATRIX_4x4_INL__
#define __MATRIX_4x4_INL__

#include <iostream>

/*==============================[Public Static Methods]==============================*/

/**
 *  Checks if two matrix are nearly equal.
 *
 *  @param p_a              First matrix to compare
 *  @param p_b              Second matrix to compare
 *  @param p_errorTolerance Maximum allowed difference for considering them as 'nearly equal'
 *
 *  @return                 True  if A and B are nearly equal
 *  @return                 False if A and B aren't nearly equal
 */
FORCEINLINE bool        Matrix4x4::IsNearlyEqual    (Matrix4x4 const&   p_a,
                                                     Matrix4x4 const&   p_b,
                                                     float const        p_tolerance)
{
    return  Math::IsNearlyEqual(p_a.m_values[0],  p_b.m_values[0] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[1],  p_b.m_values[1] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[2],  p_b.m_values[2] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[3],  p_b.m_values[3] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[4],  p_b.m_values[4] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[5],  p_b.m_values[5] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[6],  p_b.m_values[6] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[7],  p_b.m_values[7] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[8],  p_b.m_values[8] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[9],  p_b.m_values[9] , p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[10], p_b.m_values[10], p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[11], p_b.m_values[11], p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[12], p_b.m_values[12], p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[13], p_b.m_values[13], p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[14], p_b.m_values[14], p_tolerance) &&
            Math::IsNearlyEqual(p_a.m_values[15], p_b.m_values[15], p_tolerance);
}

/**
 * Creates a view matrix given an eye position, a position to look at, and an up vector.
 * Corresponds to Matrix4x4::TRS(p_from, Quaternion::LooRotation(p_to-p_from), Vector::One)
 *
 * @param p_from    The eye position.
 * @param p_to      The position to look at.
 * @param p_up      The vector describing the up direction.
 *
 * @return          The look at matrix.
 */
FORCEINLINE Matrix4x4   Matrix4x4::LookAt           (Vector3 const&     p_from,
                                                     Vector3 const&     p_to,
                                                     Vector3 const&     p_up)
{
    Vector3 const forward   {(p_to-p_from).GetSafeNormalized()};
    Vector3 const right     {(p_up ^ forward).GetSafeNormalized()};
    Vector3 const up        {forward ^ right};

    return Matrix4x4
    (
        right.m_x,  up.m_x, forward.m_x,    p_from.m_x,
        right.m_y,  up.m_y, forward.m_y,    p_from.m_y,
        right.m_z,  up.m_z, forward.m_z,    p_from.m_z,
        0.0f,       0.0f,   0.0f,           1.0f
    );
}

/**
 * Creates an orthogonal projection matrix
 * 
 * @param p_left    The left value
 * @param p_right   The right value
 * @param p_bottom  The bottom value
 * @param p_top     The top value
 * @param p_zNear   The near value
 * @param p_zFar    The far value
 *
 * @return          The resulting orthogonal projection matrix
 */
FORCEINLINE Matrix4x4   Matrix4x4::Ortho            (float const        p_left,
                                                     float const        p_right,
                                                     float const        p_bottom,
                                                     float const        p_top,
                                                     float const        p_near,
                                                     float const        p_far)
{
    Matrix4x4 result(Matrix4x4::Identity);

    result(0, 0) = 2.0f / (p_right - p_left);
    result(1, 1) = 2.0f / (p_top   - p_bottom);
    result(2, 2) = 1.0f / (p_far   - p_near);
    result(0, 3) = - (p_right + p_left)   / (p_right - p_left);
    result(1, 3) = - (p_top   + p_bottom) / (p_top   - p_bottom);
    result(2, 3) = - p_near / (p_far - p_near);

    return result;
}

/**
 * Creates an perspective projection matrix
 *
 * @param p_fov     The field of view in radian.
 * @param p_aspect  The aspect ratio.
 * @param p_near    The distance of the near plane
 * @param p_far     The distance of the far plane
 *
 * @return          The resulting orthogonal projection matrix
 */
FORCEINLINE Matrix4x4   Matrix4x4::Perspective      (float const        p_fov,
                                                     float const        p_aspect,
                                                     float const        p_near,
                                                     float const        p_far)
{
    float const tanHalfFovy = Math::Tan(p_fov / 2.0f);

    Matrix4x4 result;

    result(0, 0) = 1.0f / (p_aspect * tanHalfFovy);
    result(1, 1) = 1.0f / (tanHalfFovy);
    result(2, 2) = p_far / (p_far - p_near);
    result(3, 2) = 1.0f;
    result(2, 3) = -(p_far * p_near) / (p_far - p_near);

    return result;
}

/**
 * Creates an perspective projection matrix
 *
 * @param p_fov     The field of view in radian.
 * @param p_aspect  The aspect ratio.
 * @param p_near    The distance of the near plane
 * @param p_far     The distance of the far plane
 *
 * @return          The resulting orthogonal projection matrix
 */
FORCEINLINE Matrix4x4   Matrix4x4::Perspective      (float const        p_fov,
                                                     float const        p_width,
                                                     float const        p_height,
                                                     float const        p_near,
                                                     float const        p_far)
{
    return Perspective(p_fov, p_width / p_height, p_near, p_far);
}

/**
* Creates a rotation Matrix4x4 with around the X axis (Angle in Degree)
*
* @param p_angleRadian The rotation angle in degree.
*
* @return      The rotatiom matrix4x4 around the X axis.
*/
FORCEINLINE Matrix4x4   Matrix4x4::RotationXDeg     (float const        p_angleDegree)
{
    return RotationXRad(Math::DegToRad(p_angleDegree));
}
                                              
/**
 * Creates a rotation Matrix4x4 with around the X axis (Angle in Radian)
 *
 * @param p_angleRadian The rotation angle in radian.
 * 
 * @return      The rotatiom matrix4x4 around the X axis.
 */                                   
FORCEINLINE Matrix4x4   Matrix4x4::RotationXRad     (float const        p_angleRadian)
{
    float const cos {Math::Cos(p_angleRadian)};
    float const sin {Math::Sin(p_angleRadian)};

    return Matrix4x4
    (
        1,      0,      0,      0,
        0,      cos,    -sin,   0,
        0,      sin,    cos,    0,
        0,      0,      0,      1
    );
}
                                              
/**
* Creates a rotation Matrix4x4 with around the Y axis (Angle in Degree)
*
* @param p_angleRadian The rotation angle in degree.
*
* @return      The rotatiom matrix4x4 around the Y axis.
*/                                   
FORCEINLINE Matrix4x4   Matrix4x4::RotationYDeg     (float const        p_angleDegree)
{
    return RotationYRad(Math::DegToRad(p_angleDegree));
}
                                              
/**
 * Creates a rotation Matrix4x4 with around the Y axis (Angle in Radian)
 *
 * @param p_angleRadian The rotation angle in radian.
 * 
 * @return      The rotatiom matrix4x4 around the Y axis.
 */                                   
FORCEINLINE Matrix4x4   Matrix4x4::RotationYRad     (float const        p_angleRadian)
{
    float const cos {Math::Cos(p_angleRadian)};
    float const sin {Math::Sin(p_angleRadian)};

    return Matrix4x4
    (
        cos,    0,      sin,    0,
        0,      1,      0,      0,
        -sin,   0,      cos,    0,
        0,      0,      0,      1
    );
}
                                              
/**
* Creates a rotation Matrix4x4 with around the Z axis (Angle in Degree)
*
* @param p_angleRadian The rotation angle in degree.
*
* @return      The rotatiom matrix4x4 around the Z axis.
*/                                   
FORCEINLINE Matrix4x4   Matrix4x4::RotationZDeg     (float const        p_angleDegree)
{
    return RotationZRad(Math::DegToRad(p_angleDegree));
}
                                              
/**
 * Creates a rotation Matrix4x4 with around the Z axis (Angle in Radian)
 *
 * @param p_angleRadian The rotation angle in radian.
 * 
 * @return      The rotatiom matrix4x4 around the Z axis.
 */                                   
FORCEINLINE Matrix4x4   Matrix4x4::RotationZRad     (float const        p_angleRadian)
{
    float const cos {Math::Cos(p_angleRadian)};
    float const sin {Math::Sin(p_angleRadian)};

    return Matrix4x4
    (
        cos,    -sin,   0,      0,
        sin,    cos,    0,      0,
        0,      0,      1,      0,
        0,      0,      0,      1
    );
}

/**
 * Creates a rotation Matrix4x4 with a Quaternion
 *
 * @param p_q    The quaternion corresponding to a rotation.
 * 
 * @return      The rotatiom matrix4x4
 */
FORCEINLINE Matrix4x4   Matrix4x4::Rotate           (Quaternion const&  p_q)
{
    float const sqrX  (Math::Square(p_q.m_x));
    float const sqrY  (Math::Square(p_q.m_y));
    float const sqrZ  (Math::Square(p_q.m_z));

    return Matrix4x4
    (
        1-2*(sqrY+sqrZ)                     , 2*(p_q.m_x*p_q.m_y-p_q.m_z*p_q.m_w)   , 2*(p_q.m_x*p_q.m_z+p_q.m_y*p_q.m_w)   , 0.0f,
        2*(p_q.m_x*p_q.m_y+p_q.m_z*p_q.m_w) , 1-2*(sqrX+sqrZ)                       , 2*(p_q.m_y*p_q.m_z-p_q.m_x*p_q.m_w)   , 0.0f,
        2*(p_q.m_x*p_q.m_z-p_q.m_y*p_q.m_w) , 2*(p_q.m_y*p_q.m_z+p_q.m_x*p_q.m_w)   , 1-2*(sqrX+sqrY)                       , 0.0f,
        0.0f                                , 0.0f                                  , 0.0f                                  , 1.0f
    );
}

/**
 * Creates a scale Matrix4x4 with a scale vector.
 *
 * @param p_scale    The scale vector where each component corresponds to a scale on an axis.
 *
 * @return           The scale matrix4x4.
 */
FORCEINLINE Matrix4x4   Matrix4x4::Scale            (Vector3 const&     p_scale)
{
    return Matrix4x4
    (
        p_scale.m_x,    0.0f,           0.0f,           0.0f,
        0.0f,           p_scale.m_y,    0.0f,           0.0f,
        0.0f,           0.0f,           p_scale.m_z,    0.0f,
        0.0f,           0.0f,           0.0f,           1.0f
    );
}

/**
 * Creates a translation Matrix4x4 with a vector corresponding to a translation.
 *
 * @param p_translation  The translation vector where each component corresponds to a translation on an axis.
 *
 * @return               The translation matrix4x4.
 */
FORCEINLINE Matrix4x4   Matrix4x4::Translate        (Vector3 const&     p_translation)
{
    return Matrix4x4
    (
        1.0f, 0.0f, 0.0f, p_translation.m_x,
        0.0f, 1.0f, 0.0f, p_translation.m_y,
        0.0f, 0.0f, 1.0f, p_translation.m_z,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

/**
 * Creates a translation, rotation and scaling matrix.
 *
 * @param p_translation     The translation vector where each component corresponds to a translation on an axis.
 * @param p_q               The quaternion corresponding to a rotation.
 * @param p_scale           The scale vector where each component corresponds to a scale on an axis.
 *
 * @return               The TRS matrix4x4.
 */
FORCEINLINE Matrix4x4   Matrix4x4::TRS              (Vector3 const&     p_translation,
                                                     Quaternion const&  p_q,
                                                     Vector3 const&     p_scale)
{

    float const sqrX  (Math::Square(p_q.m_x));
    float const sqrY  (Math::Square(p_q.m_y));
    float const sqrZ  (Math::Square(p_q.m_z));

    return Matrix4x4
    (
        (1-2*(sqrY+sqrZ)) * p_scale.m_x                     , 2*(p_q.m_x*p_q.m_y-p_q.m_z*p_q.m_w) * p_scale.m_y , 2*(p_q.m_x*p_q.m_z+p_q.m_y*p_q.m_w) * p_scale.m_z , p_translation.m_x,
        2*(p_q.m_x*p_q.m_y+p_q.m_z*p_q.m_w) * p_scale.m_x   , (1-2*(sqrX+sqrZ)) * p_scale.m_y                   , 2*(p_q.m_y*p_q.m_z-p_q.m_x*p_q.m_w) * p_scale.m_z , p_translation.m_y,
        2*(p_q.m_x*p_q.m_z-p_q.m_y*p_q.m_w) * p_scale.m_x   , 2*(p_q.m_y*p_q.m_z+p_q.m_x*p_q.m_w) * p_scale.m_y , (1-2*(sqrX+sqrY)) * p_scale.m_z                   , p_translation.m_z,
        0.0f                                                , 0.0f                                              , 0.0f                                              , 1.0f
    );
}

FORCEINLINE Matrix4x4   Matrix4x4::View             (Vector3 const&     p_from,
                                                     Vector3 const&     p_to,
                                                     Vector3 const&     p_up)
{
    Vector3 const forward   {(p_to-p_from).GetSafeNormalized()};
    Vector3 const right     {(p_up ^ forward).GetSafeNormalized()};
    Vector3 const up        {forward ^ right};

    return Matrix4x4
    (
        right  .m_x,    right  .m_y,    right  .m_z,    -Vector3::Dot(right,   p_from),
        up     .m_x,    up     .m_y,    up     .m_z,    -Vector3::Dot(up,      p_from),
        forward.m_x,    forward.m_y,    forward.m_z,    -Vector3::Dot(forward, p_from),
        0.0f,           0.0f,           0.0f,           1.0f
    );
}

/*==============================[Constructors + Destructor]==============================*/

/**
 * Default constructor, all components are initialized to 0.0f 
 *
 *  |   0.0f,   0.0f,   0.0f,   0.0f    |
 *  |   0.0f,   0.0f,   0.0f,   0.0f    |
 *  |   0.0f,   0.0f,   0.0f,   0.0f    |
 *  |   0.0f,   0.0f,   0.0f,   0.0f    |
 *
 */
FORCEINLINE Matrix4x4::Matrix4x4 	()
{
	m_values[0] = 0.0f; m_values[4] = 0.0f; m_values[8]  = 0.0f; m_values[12] = 0.0f;
    m_values[1] = 0.0f; m_values[5] = 0.0f; m_values[9]  = 0.0f; m_values[13] = 0.0f;
    m_values[2] = 0.0f; m_values[6] = 0.0f; m_values[10] = 0.0f; m_values[14] = 0.0f;
    m_values[3] = 0.0f; m_values[7] = 0.0f; m_values[11] = 0.0f; m_values[15] = 0.0f;
}


/**
 * Constructor using initial values for each component.
 *
 *  |   p_m00,   p_m01,   p_m02,   p_m01|
 *  |   p_m10,   p_m11,   p_m12,   p_m11|
 *  |   p_m20,   p_m21,   p_m22,   p_m21|
 *  |   p_m30,   p_m31,   p_m32,   p_m31|
 *
 */       
FORCEINLINE Matrix4x4::Matrix4x4 	(float const        p_m00, 
                                     float const        p_m01,    
                                     float const        p_m02,   
                                     float const        p_m03,
                                     float const        p_m10, 
                                     float const        p_m11,    
                                     float const        p_m12,   
                                     float const        p_m13,  
                                     float const        p_m20, 
                                     float const        p_m21,    
                                     float const        p_m22,   
                                     float const        p_m23, 
                                     float const        p_m30, 
                                     float const        p_m31,    
                                     float const        p_m32,    
                                     float const        p_m33)
{
	m_values[0] = p_m00; m_values[4] = p_m01; m_values[8]  = p_m02; m_values[12] = p_m03;
    m_values[1] = p_m10; m_values[5] = p_m11; m_values[9]  = p_m12; m_values[13] = p_m13;
    m_values[2] = p_m20; m_values[6] = p_m21; m_values[10] = p_m22; m_values[14] = p_m23;
    m_values[3] = p_m30; m_values[7] = p_m31; m_values[11] = p_m32; m_values[15] = p_m33;
}

/**
 * Constructor using initial values for each component.
 *
 *  |   p_columnX.m_x,  p_columnY.m_x,  p_columnZ.m_x,  p_columnW.m_x   |
 *  |   p_columnX.m_y,  p_columnY.m_y,  p_columnZ.m_y,  p_columnW.m_y   |
 *  |   p_columnX.m_z,  p_columnY.m_z,  p_columnZ.m_z,  p_columnW.m_z   |
 *  |   0.0f,           0.0f,           0.0f,           1.0f            |
 *
 * @param p_columnX The vector corresponding to X axis (Right) 
 * @param p_columnY The vector corresponding to Y axis (Up)
 * @param p_columnZ The vector corresponding to Z axis (ForwarD)
 * @param p_columnW The vector corresponding to the origin.
 */       
FORCEINLINE Matrix4x4::Matrix4x4 	(Vector3 const&     p_columnX,
                                     Vector3 const&     p_columnY,
                                     Vector3 const&     p_columnZ,
                                     Vector3 const&     p_columnW)
{
    m_values[0] = p_columnX.m_x;    m_values[4] = p_columnY.m_x;    m_values[8]  = p_columnZ.m_x;   m_values[12] = p_columnW.m_x;
    m_values[1] = p_columnX.m_y;    m_values[5] = p_columnY.m_y;    m_values[9]  = p_columnZ.m_y;   m_values[13] = p_columnW.m_y;
    m_values[2] = p_columnX.m_z;    m_values[6] = p_columnY.m_z;    m_values[10] = p_columnZ.m_z;   m_values[14] = p_columnW.m_z;
    m_values[3] = 0.0f; 		    m_values[7] = 0.0f; 		    m_values[11] = 0.0f; 		    m_values[15] = 1.0f;
}

/**
 * Constructor using initial values for each component.
 *
 *  |   p_columnX.m_x,  p_columnY.m_x,  p_columnZ.m_x,  p_columnW.m_x   |
 *  |   p_columnX.m_y,  p_columnY.m_y,  p_columnZ.m_y,  p_columnW.m_y   |
 *  |   p_columnX.m_z,  p_columnY.m_z,  p_columnZ.m_z,  p_columnW.m_z   |
 *  |   p_columnX.m_w,  p_columnY.m_w,  p_columnZ.m_w,  p_columnY.m_w   |
 *
 * @param p_columnX The vector corresponding to X axis (Right)
 * @param p_columnY The vector corresponding to Y axis (Up)
 * @param p_columnZ The vector corresponding to Z axis (ForwarD)
 * @param p_columnW The vector corresponding to the origin.
 */
FORCEINLINE Matrix4x4::Matrix4x4    (Vector4 const&     p_columnX,
                                     Vector4 const&     p_columnY,
                                     Vector4 const&     p_columnZ,
                                     Vector4 const&     p_columnW)
{
    m_values[0] = p_columnX.m_x;    m_values[4] = p_columnY.m_x;    m_values[8]  = p_columnZ.m_x;   m_values[12] = p_columnW.m_x;
    m_values[1] = p_columnX.m_y;    m_values[5] = p_columnY.m_y;    m_values[9]  = p_columnZ.m_y;   m_values[13] = p_columnW.m_y;
    m_values[2] = p_columnX.m_z;    m_values[6] = p_columnY.m_z;    m_values[10] = p_columnZ.m_z;   m_values[14] = p_columnW.m_z;
    m_values[3] = p_columnX.m_w;    m_values[7] = p_columnY.m_w;    m_values[11] = p_columnZ.m_w;   m_values[15] = p_columnW.m_w;
}

/**
 * Copy constructor using initial matrix4x4 for each component.
 *
 * @param p_copy    The initial matrix4x4 to copy.
 */
FORCEINLINE Matrix4x4::Matrix4x4    (Matrix4x4 const&   p_copy)
{
    m_values[0] = p_copy.m_values[0]; m_values[4] = p_copy.m_values[4]; m_values[8]  = p_copy.m_values[8]  ; m_values[12] = p_copy.m_values[12];
    m_values[1] = p_copy.m_values[1]; m_values[5] = p_copy.m_values[5]; m_values[9]  = p_copy.m_values[9]  ; m_values[13] = p_copy.m_values[13];
    m_values[2] = p_copy.m_values[2]; m_values[6] = p_copy.m_values[6]; m_values[10] = p_copy.m_values[10] ; m_values[14] = p_copy.m_values[14];
    m_values[3] = p_copy.m_values[3]; m_values[7] = p_copy.m_values[7]; m_values[11] = p_copy.m_values[11] ; m_values[15] = p_copy.m_values[15];
}

/**
 * Move constructor using initial matrix4x4 for each component.
 *
 * @param p_copy    The initial matrix4x4 to move.
 */
FORCEINLINE Matrix4x4::Matrix4x4    (Matrix4x4&&        p_move)
{
    m_values[0] = std::move(p_move.m_values[0]); m_values[4] = std::move(p_move.m_values[4]); m_values[8]  = std::move(p_move.m_values[8] ) ; m_values[12] = std::move(p_move.m_values[12]);
    m_values[1] = std::move(p_move.m_values[1]); m_values[5] = std::move(p_move.m_values[5]); m_values[9]  = std::move(p_move.m_values[9] ) ; m_values[13] = std::move(p_move.m_values[13]);
    m_values[2] = std::move(p_move.m_values[2]); m_values[6] = std::move(p_move.m_values[6]); m_values[10] = std::move(p_move.m_values[10]) ; m_values[14] = std::move(p_move.m_values[14]);
    m_values[3] = std::move(p_move.m_values[3]); m_values[7] = std::move(p_move.m_values[7]); m_values[11] = std::move(p_move.m_values[11]) ; m_values[15] = std::move(p_move.m_values[15]);
}

/*==============================[Public Local Methods]==============================*/

/**
 * Remove any scaling from this matrix (The magnitude of each column is 1) and return the scale vector with a error tolerance
 *
 * @param p_tolerance   The error tolerance considering the column magnitude as zero.
 *
 * @return The scale vector of this matrix.
 */
FORCEINLINE Vector3     Matrix4x4::ExtractScale             (float const    p_tolerance)
{
    Vector3     scale;

    float const squareX {Math::Square(m_values[0]) + Math::Square(m_values[1]) + Math::Square(m_values[2])};
    float const squareY {Math::Square(m_values[4]) + Math::Square(m_values[5]) + Math::Square(m_values[6])};
    float const squareZ {Math::Square(m_values[8]) + Math::Square(m_values[9]) + Math::Square(m_values[10])};

    if (squareX > p_tolerance)
    {
        scale.m_x = Math::Sqrt(squareX);
        m_values[0] /= scale.m_x;
        m_values[1] /= scale.m_x;
        m_values[2] /= scale.m_x;
    }

    if (squareY > p_tolerance)
    {
        scale.m_y = Math::Sqrt(squareY);
        m_values[4] /= scale.m_y;
        m_values[5] /= scale.m_y;
        m_values[6] /= scale.m_y;
    }

    if (squareZ > p_tolerance)
    {
        scale.m_z = Math::Sqrt(squareZ);
        m_values[8]  /= scale.m_z;
        m_values[9]  /= scale.m_z;
        m_values[10] /= scale.m_z;
    }

    return scale;
}

/**
 * Get the column of this matrix
 *
 * @param p_index   The column's index. Must be in [0,2] range.
 *
 * @return          The vector of the column.
 */
FORCEINLINE Vector3     Matrix4x4::GetAxis                  (int32 const    p_index)        const
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 axes count: [0,2]");

    return Vector3
    (
        m_values[p_index * 4],
        m_values[1 + p_index * 4],
        m_values[2 + p_index * 4]
    );
}

/**
 * Gets the unit axis -Z of this matrix
 *
 * @return The unit axis -Z.
 */
FORCEINLINE Vector3     Matrix4x4::GetBackward              ()                              const
{
    return GetScaledBackward().GetSafeNormalized();
}

/**
 * Computes the cofactor matrix of this matrix4x4
 *
 * @return  The cofactor of this matrix4x4
 */
FORCEINLINE Matrix4x4   Matrix4x4::GetCofactor              ()                              const
{
     Matrix4x4   result;
    
    float       d12     {m_values[2]  *  m_values[7]  - m_values[3]  *  m_values[6] };
    float       d13     {m_values[2]  *  m_values[11] - m_values[3]  *  m_values[10]};
    float       d14     {m_values[2]  *  m_values[15] - m_values[3]  *  m_values[14]};
    float       d23     {m_values[6]  *  m_values[11] - m_values[7]  *  m_values[10]};
    float       d24     {m_values[6]  *  m_values[15] - m_values[7]  *  m_values[14]};
    float       d34     {m_values[10] *  m_values[15] - m_values[11] *  m_values[14]};

    result.m_values[0]  =  (m_values[5] * d34 - m_values[9] * d24 + m_values[13] * d23);
    result.m_values[4]  = -(m_values[1] * d34 - m_values[9] * d14 + m_values[13] * d13);
    result.m_values[8]  =  (m_values[1] * d24 - m_values[5] * d14 + m_values[13] * d12);
    result.m_values[12] = -(m_values[1] * d23 - m_values[5] * d13 + m_values[9]  * d12);

    result.m_values[1]  = -(m_values[4] * d34 - m_values[8] * d24 + m_values[12] * d23);
    result.m_values[5]  =  (m_values[0] * d34 - m_values[8] * d14 + m_values[12] * d13);
    result.m_values[9]  = -(m_values[0] * d24 - m_values[4] * d14 + m_values[12] * d12);
    result.m_values[13] =  (m_values[0] * d23 - m_values[4] * d13 + m_values[8]  * d12);

    d12                 = m_values[0] * m_values[5]  - m_values[1] * m_values[4];
    d13                 = m_values[0] * m_values[9]  - m_values[1] * m_values[8];
    d14                 = m_values[0] * m_values[13] - m_values[1] * m_values[12];
    d23                 = m_values[4] * m_values[9]  - m_values[5] * m_values[8];
    d24                 = m_values[4] * m_values[13] - m_values[5] * m_values[12];
    d34                 = m_values[8] * m_values[13] - m_values[9] * m_values[12];

    result.m_values[2]  =  (m_values[7] * d34 - m_values[11] * d24 + m_values[15] * d23);
    result.m_values[6]  = -(m_values[3] * d34 - m_values[11] * d14 + m_values[15] * d13);
    result.m_values[10] =  (m_values[3] * d24 - m_values[7]  * d14 + m_values[15] * d12);
    result.m_values[14] = -(m_values[3] * d23 - m_values[7]  * d13 + m_values[11] * d12);

    result.m_values[3] = -(m_values[6] * d34 - m_values[10] * d24 + m_values[14] * d23);
    result.m_values[7] =  (m_values[2] * d34 - m_values[10] * d14 + m_values[14] * d13);
    result.m_values[11] = -(m_values[2] * d24 - m_values[6]  * d14 + m_values[14] * d12);
    result.m_values[15] =  (m_values[2] * d23 - m_values[6]  * d13 + m_values[10] * d12);

    return result;
}

/**
 * Get the column of this matrix
 *
 * @param p_index   The column's index. Must be in [0,3] range
 *
 * @return          The vector of the column.
 */
FORCEINLINE Vector4     Matrix4x4::GetColumn                (int32 const    p_index)        const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 column count: [0,3]");

    return Vector4
    (
        m_values[p_index * 4],
        m_values[1 + p_index * 4],
        m_values[2 + p_index * 4],
        m_values[3 + p_index * 4]
    );
}

/**
 * Computes the determinant of this matrix4x4
 *
 * @return  The determinant of this.
 */
FORCEINLINE float       Matrix4x4::GetDeterminant           ()                              const
{
    return  m_values[0] * 
            (
                m_values[5]  * (m_values[10] * m_values[15] - m_values[11] * m_values[14]) -
                m_values[9]  * (m_values[6]  * m_values[15] - m_values[7]  * m_values[14]) +
                m_values[13] * (m_values[6]  * m_values[11] - m_values[7]  * m_values[10])
            ) -

            m_values[4] *
            (
                m_values[1]  * (m_values[10] * m_values[15] - m_values[11] * m_values[14]) -
                m_values[9]  * (m_values[2]  * m_values[15] - m_values[3]  * m_values[14]) +
                m_values[13] * (m_values[2]  * m_values[11] - m_values[3]  * m_values[10])
            ) +
            
            m_values[8] *
            (
                m_values[1]  * (m_values[6] * m_values[15] - m_values[7] * m_values[14]) -
                m_values[5]  * (m_values[2] * m_values[15] - m_values[3] * m_values[14]) +
                m_values[13] * (m_values[2] * m_values[7]  - m_values[3] * m_values[6])
            ) - 
           
            m_values[12] * 
            (
             	m_values[1] * (m_values[6] * m_values[11] - m_values[7] * m_values[10]) -
			    m_values[5] * (m_values[2] * m_values[11] - m_values[3] * m_values[10]) +
			    m_values[9] * (m_values[2] * m_values[7]  - m_values[3] * m_values[6])
            );
}

/**
 * Gets the unit axis -Y of this matrix
 *
 * @return The unit axis -Y.
 */
FORCEINLINE Vector3     Matrix4x4::GetDown                  ()                              const
{
    return GetScaledDown().GetSafeNormalized();
}

/**
 * Gets the unit axis Z of this matrix
 *
 * @return The unit axis Z.
 */
FORCEINLINE Vector3     Matrix4x4::GetForward               ()                              const
{
    return GetScaledForward().GetSafeNormalized();
}

/**
 * Computes the inverse of this matrix4x4
 *
 * @return  If the determinant of this matrix4x4 is equal to 0.0f, return Matrix::Identity, otherwise the inverse of this matrix4x4
 */
FORCEINLINE Matrix4x4   Matrix4x4::GetInverse               ()                              const
{
     Matrix4x4   result;
    
    float       d12         {m_values[2]  *  m_values[7]  - m_values[3]  *  m_values[6] };
    float       d13         {m_values[2]  *  m_values[11] - m_values[3]  *  m_values[10]};
    float       d14         {m_values[2]  *  m_values[15] - m_values[3]  *  m_values[14]};
    float       d23         {m_values[6]  *  m_values[11] - m_values[7]  *  m_values[10]};
    float       d24         {m_values[6]  *  m_values[15] - m_values[7]  *  m_values[14]};
    float       d34         {m_values[10] *  m_values[15] - m_values[11] *  m_values[14]};

    result.m_values[0]      =  (m_values[5] * d34 - m_values[9] * d24 + m_values[13] * d23);
    result.m_values[1]      = -(m_values[1] * d34 - m_values[9] * d14 + m_values[13] * d13);
    result.m_values[2]      =  (m_values[1] * d24 - m_values[5] * d14 + m_values[13] * d12);
    result.m_values[3]      = -(m_values[1] * d23 - m_values[5] * d13 + m_values[9]  * d12);
          
    float       determinant {m_values[0]*result.m_values[0] + m_values[4]*result.m_values[1] + m_values[8] * result.m_values[2] + m_values[12] * result.m_values[3]};

    if (determinant == 0)
    {
        result = Matrix4x4::Identity;
    }

    else
    {
        result.m_values[4]  = -(m_values[4] * d34 - m_values[8] * d24 + m_values[12] * d23);
        result.m_values[5]  =  (m_values[0] * d34 - m_values[8] * d14 + m_values[12] * d13);
        result.m_values[6]  = -(m_values[0] * d24 - m_values[4] * d14 + m_values[12] * d12);
        result.m_values[7]  =  (m_values[0] * d23 - m_values[4] * d13 + m_values[8]  * d12);

        d12                 = m_values[0] * m_values[5]  - m_values[1] * m_values[4];
        d13                 = m_values[0] * m_values[9]  - m_values[1] * m_values[8];
        d14                 = m_values[0] * m_values[13] - m_values[1] * m_values[12];
        d23                 = m_values[4] * m_values[9]  - m_values[5] * m_values[8];
        d24                 = m_values[4] * m_values[13] - m_values[5] * m_values[12];
        d34                 = m_values[8] * m_values[13] - m_values[9] * m_values[12];

        result.m_values[8]  =  (m_values[7] * d34 - m_values[11] * d24 + m_values[15] * d23);
        result.m_values[9]  = -(m_values[3] * d34 - m_values[11] * d14 + m_values[15] * d13);
        result.m_values[10] =  (m_values[3] * d24 - m_values[7]  * d14 + m_values[15] * d12);
        result.m_values[11] = -(m_values[3] * d23 - m_values[7]  * d13 + m_values[11] * d12);

        result.m_values[12] = -(m_values[6] * d34 - m_values[10] * d24 + m_values[14] * d23);
        result.m_values[13] =  (m_values[2] * d34 - m_values[10] * d14 + m_values[14] * d13);
        result.m_values[14] = -(m_values[2] * d24 - m_values[6]  * d14 + m_values[14] * d12);
        result.m_values[15] =  (m_values[2] * d23 - m_values[6]  * d13 + m_values[10] * d12);
    }
 
    return result * (1/determinant);
}

/**
 * Gets the unit axis -X of this matrix
 *
 * @return The unit axis -X.
 */
FORCEINLINE Vector3     Matrix4x4::GetLeft                  ()                              const
{
    return GetScaledLeft().GetSafeNormalized();
}

/**
 * Gets this matrix without scaling with error tolerance.
 *
 * @param p_tolerance   The error tolerance considering the column magnitude as zero.
 *
 */
FORCEINLINE Matrix4x4   Matrix4x4::GetMatrixWithoutScale    (float const    p_tolerance)    const
{
    Matrix4x4 result    {*this};
    
    result.RemoveScaling(p_tolerance);

    return result;
}

/** 
 * Get the magnitude of the axis with the higher magnitude.
 *
 * @return The magnitude of the axis with the higher magnitude. 
 */
FORCEINLINE float       Matrix4x4::GetMaximumAxisScale      ()                              const
{
    float const maximumAxisScale {Math::Max3(GetScaledRight().GetMagnitudeSquared(), GetScaledUp().GetMagnitudeSquared(), GetScaledForward().GetMagnitudeSquared())};

    return Math::Sqrt(maximumAxisScale);
}

/**
 * Get the translation part of this matrix.
 *
 * @return  The origin of the coordinate system.
 */
FORCEINLINE Vector3	    Matrix4x4::GetOrigin	            ()                              const
{
	return Vector3 (m_values[12], m_values[13], m_values[14]);
}

/**
 * Gets the unit axis X of this matrix
 *
 * @return The unit axis X.
 */
FORCEINLINE Vector3     Matrix4x4::GetRight                 ()                              const
{
    return GetScaledRight().GetSafeNormalized();
}

/**
 * Get the rotation part of this matrix.
 *
 * @return The rotation part of this matrix.
 */
FORCEINLINE Quaternion  Matrix4x4::GetRotation              ()                              const
{
    Quaternion result;

    if (GetRight().IsNearlyZero() || GetUp().IsNearlyZero() || GetForward().IsNearlyZero())
    {
        return Quaternion::Identity;
    }

    float   m00     {operator()(0,0)};
    float   m11     {operator()(1,1)};
    float   m22     {operator()(2,2)};
    
    float   trace   {m00 + m11 + m22};

    float   m01     {operator()(0,1)};
    float   m02     {operator()(0,2)};
    float   m10     {operator()(1,0)};
    float   m12     {operator()(1,2)};
    float   m20     {operator()(2,0)};
    float   m21     {operator()(2,1)};

    if (trace > 0)
    {
        float s = Math::Sqrt(trace + 1) * 2; 
        result.Set
        (
            (m21 - m12) / s,
            (m02 - m20) / s,
            (m10 - m01) / s,
            0.25f * s
        );
    }


    else if (m00 > m11 && m00 > m22)
    {
        float s = Math::Sqrt(1.0f + m00 - m11 + m22) * 2.0f; 
        result.Set
        (
            0.25f * s,
            (m01 + m10) / s, 
            (m02 + m20) / s, 
            (m21 - m12) / s
        );
    }
    
    else if (m11 > m22) 
    { 
        float s = Math::Sqrt(1.0f + m11 - m00 - m22) * 2.0f; // S=4*qy
        
        result.Set
        (
            (m01 + m10) / s, 
            0.25f * s,
            (m12 + m21) / s, 
            (m02 - m20) / s
        );
    }   

    else 
    { 
        float s = Math::Sqrt(1.0f + m22 - m00 - m11) * 2.0f; // S=4*qz
        result.Set
        (
            (m02 + m20) / s,
            (m12 + m21) / s,
            0.25f * s,
            (m10 - m01) / s
        );
    }

    return result;
}

/**
 * Computes the determinant of rotation 3x3 matrix.
 *
 * @return  The determinant of rotation 3x3 matrix.
 */
FORCEINLINE float       Matrix4x4::GetRotationDeterminant   ()                              const
{
    return  m_values[0] * (m_values[5] * m_values[10] - m_values[6] * m_values[9]) -
            m_values[1] * (m_values[1] * m_values[10] - m_values[2] * m_values[9]) +
            m_values[2] * (m_values[1] * m_values[6]  - m_values[2] * m_values[5]);
}

/**
 * Get the row of this matrix
 *
 * @param p_index   The row's index.
 *
 * @return          The vector of the row.
 */
FORCEINLINE Vector4     Matrix4x4::GetRow                   (int32 const    p_index)        const
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 row count: [0,3]");

    return Vector4
    (
        m_values[p_index],
        m_values[p_index + 4],
        m_values[p_index + 8],
        m_values[p_index + 12]
    );
}

/**
 * Compute a vector where each component is the magnitude of a column matrix with a tolerance.
 *
 * @param p_tolerance   The tolerance for the magnitude of a column.
 *
 * @return              The determinant of rotation 3x3 matrix.
 */
FORCEINLINE Vector3     Matrix4x4::GetScale                 (float const    p_tolerance)    const
{
    Vector3     scale   {Vector3::One};

    float const magnitudeSquaredX   {Math::Square(m_values[0]) + Math::Square(m_values[1]) + Math::Square(m_values[2])};
    float const magnitudeSquaredY   {Math::Square(m_values[4]) + Math::Square(m_values[5]) + Math::Square(m_values[6])};
    float const magnitudeSquaredZ   {Math::Square(m_values[8]) + Math::Square(m_values[9]) + Math::Square(m_values[10])};

    if (magnitudeSquaredX > p_tolerance)
        scale.m_x = Math::Sqrt(magnitudeSquaredX);
    if (magnitudeSquaredY > p_tolerance)
        scale.m_y = Math::Sqrt(magnitudeSquaredY);
    if (magnitudeSquaredZ > p_tolerance)
        scale.m_z = Math::Sqrt(magnitudeSquaredZ);

    return scale;
}

/**
 * Get axes of this matrix scaled by the scale of the matrix
 *
 * @param p_outXAxis The X scaled axis returned to this param.
 * @param p_outYAxis The Y scaled axis returned to this param.
 * @param p_outZAxis The Z scaled axis returned to this param.
 */
FORCEINLINE void        Matrix4x4::GetScaledAxes            (Vector3&       p_outXAxis,
                                                             Vector3&       p_outYAxis,
                                                             Vector3&       p_outZAxis)     const
{
    p_outXAxis.m_x = m_values[0] ; p_outYAxis.m_x = m_values[4] ; p_outZAxis.m_x = m_values[8];
    p_outXAxis.m_y = m_values[1] ; p_outYAxis.m_y = m_values[5] ; p_outZAxis.m_y = m_values[9];
    p_outXAxis.m_z = m_values[2] ; p_outYAxis.m_z = m_values[6] ; p_outZAxis.m_z = m_values[10];
}

/**
 * Gets scaled axis -Z of this matrix
 *
 * @return The scaled axis -Z.
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledBackward        ()                              const
{
    return -GetScaledForward();
}

/**
 * Gets scaled axis -Y of this matrix
 *
 * @return The scaled axis -Y.
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledDown            ()                              const
{
    return -GetScaledUp();
}

/**
 * Gets scaled axis Z of this matrix
 *
 * @return The scaled axis Z.
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledForward         ()                              const
{
    return Vector3
    (
        m_values[8],
        m_values[9],
        m_values[10]
    );
}

/**
 * Gets scaled axis -X of this matrix
 *
 * @return The scaled axis -X.
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledLeft            ()                              const
{
    return -GetScaledRight();
}

/**
 * Gets scaled axis X of this matrix
 *
 * @return The scaled axis X.
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledRight           ()                              const
{
    return Vector3
    (
        m_values[0],
        m_values[1],
        m_values[2]
    );
}

/**
 * Gets scaled axis Y of this matrix
 *
 * @return The scaled axis Y .
 */
FORCEINLINE Vector3     Matrix4x4::GetScaledUp              ()                              const
{
    return Vector3
    (
        m_values[4],
        m_values[5],
        m_values[6]
    );
}

/**
 * Computes the transposed of this matrix4x4
 *
 * @return  The transposed of this.
 */
FORCEINLINE Matrix4x4   Matrix4x4::GetTransposed            ()                              const
{
    return Matrix4x4
    (
        m_values[0] , m_values[1] , m_values[2] , m_values[3],
        m_values[4] , m_values[5] , m_values[6] , m_values[7],
        m_values[8] , m_values[9] , m_values[10], m_values[11],
        m_values[12], m_values[13], m_values[14], m_values[15]
    );
}

/**
 * Gets the unit axis Y of this matrix
 *
 * @return The unit axis Y.
 */
FORCEINLINE Vector3     Matrix4x4::GetUp                    ()                              const
{
    return GetScaledUp().GetSafeNormalized();
}

/**
 * Gets the raw values of this matrix.
 * 
 * @return The value as an array of float.
 */
FORCEINLINE float*      Matrix4x4::GetValues                ()
{
    return m_values.data();
}

/**
 * Transform a point by this Matrix.
 * The translation part is taken into account. The W component is used.
 *
 * @param p_point       The point to transform.
 *
 * @return              The point transformed.
 */
FORCEINLINE Vector3     Matrix4x4::MultiplyPoint            (Vector3 const& p_point)        const
{
    float const w   {m_values[3] * p_point.m_x + m_values[7] * p_point.m_y + m_values[11] * p_point.m_z + m_values[15]};

    return Vector3
    (
        (m_values[0] * p_point.m_x + m_values[4] * p_point.m_y + m_values[8]  * p_point.m_z + m_values[12]) / w,
        (m_values[1] * p_point.m_x + m_values[5] * p_point.m_y + m_values[9]  * p_point.m_z + m_values[13]) / w,
        (m_values[2] * p_point.m_x + m_values[6] * p_point.m_y + m_values[10] * p_point.m_z + m_values[14]) / w
    );
}

/**
 * Transform a point by this Matrix.
 * The translation part is taken into account. The W component is count as 1.
 *
 * @param p_point       The point to transform.
 *
 * @return              The point transformed.
 */
FORCEINLINE Vector3     Matrix4x4::MultiplyPoint3x4         (Vector3 const& p_point)        const
{
    return Vector3
    (
        m_values[0] * p_point.m_x + m_values[4] * p_point.m_y + m_values[8]  * p_point.m_z + m_values[12],
        m_values[1] * p_point.m_x + m_values[5] * p_point.m_y + m_values[9]  * p_point.m_z + m_values[13],
        m_values[2] * p_point.m_x + m_values[6] * p_point.m_y + m_values[10] * p_point.m_z + m_values[14]
    );
}

/**
 * Transform a direction vector by this Matrix.
 * The translation part isn't taken into account (Only rotation and scale).
 *
 * @param p_point       The direction vector to transform.
 *
 * @return              The direction vector transformed.
 */
FORCEINLINE Vector3     Matrix4x4::MultiplyVector           (Vector3 const& p_point)        const
{
    return Vector3
    (
        m_values[0] * p_point.m_x + m_values[4] * p_point.m_y + m_values[8]  * p_point.m_z,
        m_values[1] * p_point.m_x + m_values[5] * p_point.m_y + m_values[9]  * p_point.m_z,
        m_values[2] * p_point.m_x + m_values[6] * p_point.m_y + m_values[10] * p_point.m_z
    );
}

/**
 * Remove any scaling from this matrix (The magnitude of each column is 1) with a error tolerance
 *
 * @param p_tolerance   The error tolerance considering the column magnitude as zero.
 */
FORCEINLINE void        Matrix4x4::RemoveScaling            (float const    p_tolerance)
{
    float const squareX {Math::Square(m_values[0]) + Math::Square(m_values[1]) + Math::Square(m_values[2])};
    float const squareY {Math::Square(m_values[4]) + Math::Square(m_values[5]) + Math::Square(m_values[6])};
    float const squareZ {Math::Square(m_values[8]) + Math::Square(m_values[9]) + Math::Square(m_values[10])};

    const float scaleX  {Math::FloatSelect(squareX - p_tolerance, Math::InvSqrt(squareX), 1.0f)};
    const float scaleY  {Math::FloatSelect(squareY - p_tolerance, Math::InvSqrt(squareY), 1.0f)};
    const float scaleZ  {Math::FloatSelect(squareZ - p_tolerance, Math::InvSqrt(squareZ), 1.0f)};

    m_values[0] *= scaleX;  m_values[4] *= scaleY;  m_values[8]  *= scaleZ;
    m_values[1] *= scaleX;  m_values[5] *= scaleY;  m_values[9]  *= scaleZ;
    m_values[2] *= scaleX;  m_values[6] *= scaleY;  m_values[10] *= scaleZ;
}

/**
 * Set an axis of this matrix
 *
 * @param p_index   The index of the axis of this matrix. Must be in [0,3] range.
 * @param p_axis    The vector of the axis
 */
FORCEINLINE void        Matrix4x4::SetAxis                  (int32 const    p_index,
                                                             Vector3 const& p_axis)
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 axes count: [0,2]");

    m_values[p_index * 4]       = p_axis.m_x;
    m_values[1 + p_index * 4]   = p_axis.m_y;
    m_values[2 + p_index * 4]   = p_axis.m_z;
}

/**
 * Set an axis of this matrix
 *
 * @param p_index   The index of the axis of this matrix. Must be in [0,3] range.
 * @param p_x       The new X coordinate of the axis.
 * @param p_y       The new Y coordinate of the axis.
 * @param p_Z       The new Z coordinate of the axis.
 */
FORCEINLINE void        Matrix4x4::SetAxis                  (int32 const    p_index,
                                                             float const    p_x,
                                                             float const    p_y,
                                                             float const    p_z)
{
    if (p_index < 0 || 2 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 axes count: [0,2]");

    m_values[p_index * 4]       = p_x;
    m_values[1 + p_index * 4]   = p_y;
    m_values[2 + p_index * 4]   = p_z;
}

/**
 * Set the X axis of this matrix.
 *
 * @param p_axis    The value of this matrix
 */
FORCEINLINE void        Matrix4x4::SetAxisX                 (Vector3 const& p_axis)
{
    m_values[0] = p_axis.m_x;
    m_values[1] = p_axis.m_y;
    m_values[2] = p_axis.m_z;
}

/**
 * Set the X axis of this matrix.
 *
 * @param p_x   The new X coordinate of the axis.
 * @param p_y   The new Y coordinate of the axis.
 * @param p_z   The new Z coordinate of the axis.
 */
FORCEINLINE void        Matrix4x4::SetAxisX                 (float const    p_x,
                                                             float const    p_y,
                                                             float const    p_z)
{
    m_values[0] = p_x;
    m_values[1] = p_y;
    m_values[2] = p_z;
}

/**
 * Set the Y axis of this matrix.
 *
 * @param p_axis    The value of this matrix
 */
FORCEINLINE void        Matrix4x4::SetAxisY                 (Vector3 const& p_axis)
{
    m_values[4] = p_axis.m_x;
    m_values[5] = p_axis.m_y;
    m_values[6] = p_axis.m_z;
}

/**
 * Set the Y axis of this matrix.
 *
 * @param p_x   The new X coordinate of the axis.
 * @param p_y   The new Y coordinate of the axis.
 * @param p_z   The new Z coordinate of the axis.
 */
FORCEINLINE void        Matrix4x4::SetAxisY                 (float const    p_x,
                                                             float const    p_y,
                                                             float const    p_z)
{
    m_values[4] = p_x;
    m_values[5] = p_y;
    m_values[6] = p_z;
}

/**
 * Set the Z axis of this matrix.
 *
 * @param p_axis    The value of this matrix
 */
FORCEINLINE void        Matrix4x4::SetAxisZ                 (Vector3 const& p_axis)
{
    m_values[8]  = p_axis.m_x;
    m_values[9]  = p_axis.m_y;
    m_values[10] = p_axis.m_z;
}

/**
 * Set the Z axis of this matrix.
 *
 * @param p_x   The new X coordinate of the axis.
 * @param p_y   The new Y coordinate of the axis.
 * @param p_z   The new Z coordinate of the axis.
 */
FORCEINLINE void        Matrix4x4::SetAxisZ                 (float const    p_x,
                                                             float const    p_y,
                                                             float const    p_z)
{
    m_values[8]  = p_x;
    m_values[9]  = p_y;
    m_values[10] = p_z;
}

/**
 * Set thecolumn of this matrix.
 *
 * @param p_index   The index of the column of this matrix. Must be in [0,3] range.
 * @param p_column  The vector of this column.
 */
FORCEINLINE void        Matrix4x4::SetColumn                (int32 const    p_index,
                                                             Vector4 const& p_column)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 column count: [0,3]");

    m_values[p_index * 4]       = p_column.m_x;
    m_values[1 + p_index * 4]   = p_column.m_y;
    m_values[2 + p_index * 4]   = p_column.m_z;
    m_values[3 + p_index * 4]   = p_column.m_w;
}

/** Set this to the identity matrix */
FORCEINLINE void        Matrix4x4::SetIdentity              ()
{
	m_values[0] = 1.0f; m_values[4] = 0.0f; m_values[8]  = 0.0f; m_values[12] = 0.0f;
    m_values[1] = 0.0f; m_values[5] = 1.0f; m_values[9]  = 0.0f; m_values[13] = 0.0f;
    m_values[2] = 0.0f; m_values[6] = 0.0f; m_values[10] = 1.0f; m_values[14] = 0.0f;
    m_values[3] = 0.0f; m_values[7] = 0.0f; m_values[11] = 0.0f; m_values[15] = 1.0f;
}

/**
 * Set thecolumn of this matrix.
 *
 * @param p_index   The index of the column of this matrix. Must be in [0,3] range.
 * @param p_index  The vector of this row.
 */
FORCEINLINE void        Matrix4x4::SetRow                   (int32 const    p_index,
                                                             Vector4 const& p_column)
{
    if (p_index < 0 || 3 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4 row count: [0,3]");

    m_values[p_index]       = p_column.m_x;
    m_values[p_index + 4]   = p_column.m_y;
    m_values[p_index + 8]   = p_column.m_z;
    m_values[p_index + 12]  = p_column.m_w;
}

/**
 *  Set the origin of the coordinate system to the given vector
 *
 * @param p_newX The new X coordinate of the system
 * @param p_newY The new Y coordinate of the system
 * @param p_newZ The new Z coordinate of the system
 */
FORCEINLINE void	    Matrix4x4::SetOrigin                (float const    p_newX,    
                                                             float const    p_newY,
                                                             float const    p_newZ)
{
    m_values[12] = p_newX;
    m_values[13] = p_newY;
    m_values[14] = p_newZ;
}

/**
 *  Set the origin of the coordinate system to the given vector
 *
 * @param p_newOrigin   The new coordinate of the system.
 *
 */
FORCEINLINE void	    Matrix4x4::SetOrigin                (Vector3 const& p_newOrigin)
{
    m_values[12] = p_newOrigin.m_x;
    m_values[13] = p_newOrigin.m_y;
    m_values[14] = p_newOrigin.m_z;
}

/*==============================[Public Local Operators]==============================*/

/**
 * Checks whether two matrix are identical.
 *
 * @param p_matrix4x4   The other matrix.
 *
 * @return              True if two matrix are identical, otherwise false.
 */
FORCEINLINE bool	    Matrix4x4::operator==	(Matrix4x4 const&   p_matrix4x4)    const
{
    return  m_values[0]  == p_matrix4x4.m_values[0]  &&  
            m_values[1]  == p_matrix4x4.m_values[1]  &&  
            m_values[2]  == p_matrix4x4.m_values[2]  &&  
            m_values[3]  == p_matrix4x4.m_values[3]  &&  
            m_values[4]  == p_matrix4x4.m_values[4]  &&
            m_values[5]  == p_matrix4x4.m_values[5]  &&
            m_values[6]  == p_matrix4x4.m_values[6]  &&
            m_values[7]  == p_matrix4x4.m_values[7]  &&
            m_values[8]  == p_matrix4x4.m_values[8]  && 
            m_values[9]  == p_matrix4x4.m_values[9]  && 
            m_values[10] == p_matrix4x4.m_values[10] && 
            m_values[11] == p_matrix4x4.m_values[11] &&
            m_values[12] == p_matrix4x4.m_values[12] &&
            m_values[13] == p_matrix4x4.m_values[13] &&
            m_values[14] == p_matrix4x4.m_values[14] &&
            m_values[15] == p_matrix4x4.m_values[15];
}

/**
 * Checks whether two matrix are different.
 *
 * @param p_matrix4x4   The other matrix.
 *
 * @return              True if two matrix are different, otherwise false.
 */
FORCEINLINE bool	    Matrix4x4::operator!=	(Matrix4x4 const&   p_matrix4x4)    const
{
    return  m_values[0]  != p_matrix4x4.m_values[0]  ||  
            m_values[1]  != p_matrix4x4.m_values[1]  ||  
            m_values[2]  != p_matrix4x4.m_values[2]  ||  
            m_values[3]  != p_matrix4x4.m_values[3]  ||  
            m_values[4]  != p_matrix4x4.m_values[4]  ||
            m_values[5]  != p_matrix4x4.m_values[5]  ||
            m_values[6]  != p_matrix4x4.m_values[6]  ||
            m_values[7]  != p_matrix4x4.m_values[7]  ||
            m_values[8]  != p_matrix4x4.m_values[8]  || 
            m_values[9]  != p_matrix4x4.m_values[9]  || 
            m_values[10] != p_matrix4x4.m_values[10] || 
            m_values[11] != p_matrix4x4.m_values[11] ||
            m_values[12] != p_matrix4x4.m_values[12] ||
            m_values[13] != p_matrix4x4.m_values[13] ||
            m_values[14] != p_matrix4x4.m_values[14] ||
            m_values[15] != p_matrix4x4.m_values[15];
}

/**
 * Copy another matrix4x4 into this one
 *
 * @param p_vector  The other matrix4x4 to copy.
 *
 * @return          The reference to matrix4x4 after copy.
 */
FORCEINLINE Matrix4x4&  Matrix4x4::operator=    (Matrix4x4 const&   p_copy)
{
    m_values[0]  = p_copy.m_values[0];  
    m_values[1]  = p_copy.m_values[1];  
    m_values[2]  = p_copy.m_values[2];  
    m_values[3]  = p_copy.m_values[3];  
    m_values[4]  = p_copy.m_values[4];
    m_values[5]  = p_copy.m_values[5];
    m_values[6]  = p_copy.m_values[6];
    m_values[7]  = p_copy.m_values[7];
    m_values[8]  = p_copy.m_values[8]; 
    m_values[9]  = p_copy.m_values[9]; 
    m_values[10] = p_copy.m_values[10]; 
    m_values[11] = p_copy.m_values[11];
    m_values[12] = p_copy.m_values[12];
    m_values[13] = p_copy.m_values[13];
    m_values[14] = p_copy.m_values[14];
    m_values[15] = p_copy.m_values[15];
    
    return *this;
}

/**
 * Move another matrix4x4 into this one
 *
 * @param p_vector  The other matrix4x4 to move.
 *
 * @return          The reference to matrix4x4 after move.
 */
FORCEINLINE Matrix4x4&  Matrix4x4::operator=    (Matrix4x4&&        p_move)
{
    m_values[0] = std::move(p_move.m_values[0]); m_values[4] = std::move(p_move.m_values[4]); m_values[8]  = std::move(p_move.m_values[8] ) ; m_values[12] = std::move(p_move.m_values[12]);
    m_values[1] = std::move(p_move.m_values[1]); m_values[5] = std::move(p_move.m_values[5]); m_values[9]  = std::move(p_move.m_values[9] ) ; m_values[13] = std::move(p_move.m_values[13]);
    m_values[2] = std::move(p_move.m_values[2]); m_values[6] = std::move(p_move.m_values[6]); m_values[10] = std::move(p_move.m_values[10]) ; m_values[14] = std::move(p_move.m_values[14]);
    m_values[3] = std::move(p_move.m_values[3]); m_values[7] = std::move(p_move.m_values[7]); m_values[11] = std::move(p_move.m_values[11]) ; m_values[15] = std::move(p_move.m_values[15]);
    
    return *this;
}

/**
 * Gets specific element at [row, column].
 *
 * @param p_rowIndex    The row index. Must be in [0,3] range.
 * @param p_columnIndex The column of vector component. Must be int [0,3] range.
 *
 * @return              The copy of the element corresponding to the indexes [p_rowIndex, p_columnIndex] given.
 */
FORCEINLINE float       Matrix4x4::operator()   (int32 const        p_rowIndex,
                                                 int32 const        p_columnIndex)  const
{
    if (p_rowIndex < 0 || 3 < p_rowIndex)
        throw std::out_of_range("p_index is out of Matrix4x4's row index range : [0,3]");

    if (p_columnIndex < 0 || 3 < p_columnIndex)
        throw std::out_of_range("p_index is out of Matrix4x4's column index range : [0,3]");

    return m_values[p_rowIndex + p_columnIndex * 4];
}

/**
 * Gets specific element at [row, column].
 *
 * @param p_rowIndex    The row index. Must be in [0,3] range.
 * @param p_columnIndex The column of vector component. Must be int [0,3] range.
 *
 * @return              The reference of the element corresponding to the indexes [p_rowIndex, p_columnIndex] given.
 */
FORCEINLINE float&      Matrix4x4::operator()   (int32 const        p_rowIndex,
                                                 int32 const        p_columnIndex)
{
    return m_values[p_rowIndex + p_columnIndex * 4];
} 
/**
 * Gets specific element at sequential index.
 *
 * @param p_index       The index value. Must be in [0,15] range.
 *
 * @return              The copy of the element corresponding to the indexes [p_rowIndex, p_columnIndex] given.
 */
FORCEINLINE float       Matrix4x4::operator()   (int32 const        p_index)        const
{
    if (p_index < 0 || 15 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4's index range : [0,15]");

    return m_values[p_index];
}

/**
 * Gets specific element at sequential index.
 *
 * @param p_index       The index value. Must be in  [0,15] range.
 *
 * @return              The reference of the element corresponding to the indexes [p_rowIndex, p_columnIndex] given.
 */
FORCEINLINE float&      Matrix4x4::operator()   (int32 const        p_index)
{
    if (p_index < 0 || 15 < p_index)
        throw std::out_of_range("p_index is out of Matrix4x4's index range : [0,15]");

    return m_values[p_index];
}

/**
 * Gets the result of adding a matrix to this.
 *
 * @param p_matrix4x4   The Matrix to add.
 *
 * @return              The result of addition.
 */
FORCEINLINE Matrix4x4 	Matrix4x4::operator+	(Matrix4x4 const&   p_matrix4x4)    const
{
    return Matrix4x4
    (
        m_values[0] + p_matrix4x4.m_values[0], m_values[4] + p_matrix4x4.m_values[4], m_values[8]  + p_matrix4x4.m_values[8] , m_values[12] + p_matrix4x4.m_values[12],
        m_values[1] + p_matrix4x4.m_values[1], m_values[5] + p_matrix4x4.m_values[5], m_values[9]  + p_matrix4x4.m_values[9] , m_values[13] + p_matrix4x4.m_values[13],
        m_values[2] + p_matrix4x4.m_values[2], m_values[6] + p_matrix4x4.m_values[6], m_values[10] + p_matrix4x4.m_values[10], m_values[14] + p_matrix4x4.m_values[14],
        m_values[3] + p_matrix4x4.m_values[3], m_values[7] + p_matrix4x4.m_values[7], m_values[11] + p_matrix4x4.m_values[11], m_values[15] + p_matrix4x4.m_values[15]
    );
}

/**
 * Adds to this matrix.
 *
 * @param p_matrix4x4   The matrix to add to this.
 *
 * @return              The reference to this after addition.
 */
FORCEINLINE Matrix4x4& 	Matrix4x4::operator+=	(Matrix4x4 const&   p_matrix4x4)
{
    m_values[0]  += p_matrix4x4.m_values[0];  
    m_values[1]  += p_matrix4x4.m_values[1];  
    m_values[2]  += p_matrix4x4.m_values[2];  
    m_values[3]  += p_matrix4x4.m_values[3];  
    m_values[4]  += p_matrix4x4.m_values[4];
    m_values[5]  += p_matrix4x4.m_values[5];
    m_values[6]  += p_matrix4x4.m_values[6];
    m_values[7]  += p_matrix4x4.m_values[7];
    m_values[8]  += p_matrix4x4.m_values[8]; 
    m_values[9]  += p_matrix4x4.m_values[9]; 
    m_values[10] += p_matrix4x4.m_values[10]; 
    m_values[11] += p_matrix4x4.m_values[11];
    m_values[12] += p_matrix4x4.m_values[12];
    m_values[13] += p_matrix4x4.m_values[13];
    m_values[14] += p_matrix4x4.m_values[14];
    m_values[15] += p_matrix4x4.m_values[15];
    
    return *this;
}

/**
 * Gets the result of subtracting a matrix to this.
 *
 * @param p_matrix4x4   The Matrix to subtract.
 *
 * @return              The result of subtraction.
 */
FORCEINLINE Matrix4x4 	Matrix4x4::operator-	(Matrix4x4 const&   p_matrix4x4)    const
{
    return Matrix4x4
    (
        m_values[0] - p_matrix4x4.m_values[0], m_values[4] - p_matrix4x4.m_values[4], m_values[8]  - p_matrix4x4.m_values[8] , m_values[12] - p_matrix4x4.m_values[12],
        m_values[1] - p_matrix4x4.m_values[1], m_values[5] - p_matrix4x4.m_values[5], m_values[9]  - p_matrix4x4.m_values[9] , m_values[13] - p_matrix4x4.m_values[13],
        m_values[2] - p_matrix4x4.m_values[2], m_values[6] - p_matrix4x4.m_values[6], m_values[10] - p_matrix4x4.m_values[10], m_values[14] - p_matrix4x4.m_values[14],
        m_values[3] - p_matrix4x4.m_values[3], m_values[7] - p_matrix4x4.m_values[7], m_values[11] - p_matrix4x4.m_values[11], m_values[15] - p_matrix4x4.m_values[15]
    );
}

/**
 * Subtracts to this matrix.
 *
 * @param p_matrix4x4   The matrix to subtract to this.
 *
 * @return              The reference to this after subtraction.
 */
FORCEINLINE Matrix4x4& 	Matrix4x4::operator-=	(Matrix4x4 const&   p_matrix4x4)
{
    m_values[0]  -= p_matrix4x4.m_values[0];  
    m_values[1]  -= p_matrix4x4.m_values[1];  
    m_values[2]  -= p_matrix4x4.m_values[2];  
    m_values[3]  -= p_matrix4x4.m_values[3];  
    m_values[4]  -= p_matrix4x4.m_values[4];
    m_values[5]  -= p_matrix4x4.m_values[5];
    m_values[6]  -= p_matrix4x4.m_values[6];
    m_values[7]  -= p_matrix4x4.m_values[7];
    m_values[8]  -= p_matrix4x4.m_values[8]; 
    m_values[9]  -= p_matrix4x4.m_values[9]; 
    m_values[10] -= p_matrix4x4.m_values[10]; 
    m_values[11] -= p_matrix4x4.m_values[11];
    m_values[12] -= p_matrix4x4.m_values[12];
    m_values[13] -= p_matrix4x4.m_values[13];
    m_values[14] -= p_matrix4x4.m_values[14];
    m_values[15] -= p_matrix4x4.m_values[15];
    
    return *this;
}

/**
 * Gets the result of multiplying a Matrix to this.
 *
 * @param p_matrix4x4   The matrix to multiply this by.
 *
 * @return              The result of multiplication.
 */
FORCEINLINE Matrix4x4   Matrix4x4::operator*    (Matrix4x4 const&   p_matrix4x4)    const
{
    return Matrix4x4
    (
       // First Row //
        m_values[0] * p_matrix4x4.m_values[0]  + m_values[4] * p_matrix4x4.m_values[1]  + m_values[8]  * p_matrix4x4.m_values[2]  + m_values[12] * p_matrix4x4.m_values[3],
        m_values[0] * p_matrix4x4.m_values[4]  + m_values[4] * p_matrix4x4.m_values[5]  + m_values[8]  * p_matrix4x4.m_values[6]  + m_values[12] * p_matrix4x4.m_values[7],
        m_values[0] * p_matrix4x4.m_values[8]  + m_values[4] * p_matrix4x4.m_values[9]  + m_values[8]  * p_matrix4x4.m_values[10] + m_values[12] * p_matrix4x4.m_values[11],
        m_values[0] * p_matrix4x4.m_values[12] + m_values[4] * p_matrix4x4.m_values[13] + m_values[8]  * p_matrix4x4.m_values[14] + m_values[12] * p_matrix4x4.m_values[15],
           
        // Second Row //
        m_values[1] * p_matrix4x4.m_values[0]  + m_values[5] * p_matrix4x4.m_values[1]  + m_values[9]  * p_matrix4x4.m_values[2]  + m_values[13] * p_matrix4x4.m_values[3],
        m_values[1] * p_matrix4x4.m_values[4]  + m_values[5] * p_matrix4x4.m_values[5]  + m_values[9]  * p_matrix4x4.m_values[6]  + m_values[13] * p_matrix4x4.m_values[7],
        m_values[1] * p_matrix4x4.m_values[8]  + m_values[5] * p_matrix4x4.m_values[9]  + m_values[9]  * p_matrix4x4.m_values[10] + m_values[13] * p_matrix4x4.m_values[11],
        m_values[1] * p_matrix4x4.m_values[12] + m_values[5] * p_matrix4x4.m_values[13] + m_values[9]  * p_matrix4x4.m_values[14] + m_values[13] * p_matrix4x4.m_values[15],
                                                                                                                                  
        // Third Row //                                                                                                           
        m_values[2] * p_matrix4x4.m_values[0]  + m_values[6] * p_matrix4x4.m_values[1]  + m_values[10] * p_matrix4x4.m_values[2]  + m_values[14] * p_matrix4x4.m_values[3],
        m_values[2] * p_matrix4x4.m_values[4]  + m_values[6] * p_matrix4x4.m_values[5]  + m_values[10] * p_matrix4x4.m_values[6]  + m_values[14] * p_matrix4x4.m_values[7],
        m_values[2] * p_matrix4x4.m_values[8]  + m_values[6] * p_matrix4x4.m_values[9]  + m_values[10] * p_matrix4x4.m_values[10] + m_values[14] * p_matrix4x4.m_values[11],
        m_values[2] * p_matrix4x4.m_values[12] + m_values[6] * p_matrix4x4.m_values[13] + m_values[10] * p_matrix4x4.m_values[14] + m_values[14] * p_matrix4x4.m_values[15],
                                                                                                                                  
        // Fourth Row //                                                                                                          
        m_values[3] * p_matrix4x4.m_values[0]  + m_values[7] * p_matrix4x4.m_values[1]  + m_values[11] * p_matrix4x4.m_values[2]  + m_values[15] * p_matrix4x4.m_values[3],
        m_values[3] * p_matrix4x4.m_values[4]  + m_values[7] * p_matrix4x4.m_values[5]  + m_values[11] * p_matrix4x4.m_values[6]  + m_values[15] * p_matrix4x4.m_values[7],
        m_values[3] * p_matrix4x4.m_values[8]  + m_values[7] * p_matrix4x4.m_values[9]  + m_values[11] * p_matrix4x4.m_values[10] + m_values[15] * p_matrix4x4.m_values[11],
        m_values[3] * p_matrix4x4.m_values[12] + m_values[7] * p_matrix4x4.m_values[13] + m_values[11] * p_matrix4x4.m_values[14] + m_values[15] * p_matrix4x4.m_values[15]
    );
}

/**
 * Multiply all components of this matrix by a value
 *
 * @param p_value   The value to use.
 *
 * @return          The result of the multiplication
 */
FORCEINLINE Matrix4x4 	Matrix4x4::operator*	(float const        p_value)        const
{
    return Matrix4x4
    (
        m_values[0] * p_value, m_values[4] * p_value, m_values[8]  * p_value, m_values[12] * p_value,
        m_values[1] * p_value, m_values[5] * p_value, m_values[9]  * p_value, m_values[13] * p_value,
        m_values[2] * p_value, m_values[6] * p_value, m_values[10] * p_value, m_values[14] * p_value,
        m_values[3] * p_value, m_values[7] * p_value, m_values[11] * p_value, m_values[15] * p_value
    );
}

/**
 * Transform a position by this Matrix.
 * The translation part is taken into account. The W component is count as 1.
 *
 * @param p_position    The position to transform.
 *
 * @return              The position transformed.
 */
FORCEINLINE Vector3     Matrix4x4::operator*    (Vector3 const&     p_position)     const
{
    return MultiplyPoint(p_position);
}

/**
 * Transform a Vector4 by this Matrix.
 *
 * @param p_position    The Vector4 to transform.
 *
 * @return              The Vector4 transformed.
 */
FORCEINLINE Vector4     Matrix4x4::operator*    (Vector4 const&     p_vector4)      const
{
    return Vector4
    (
        m_values[0] * p_vector4.m_x + m_values[4] * p_vector4.m_y + m_values[8]  * p_vector4.m_z + p_vector4.m_w  * m_values[12],
        m_values[1] * p_vector4.m_x + m_values[5] * p_vector4.m_y + m_values[9]  * p_vector4.m_z + p_vector4.m_w  * m_values[13],
        m_values[2] * p_vector4.m_x + m_values[6] * p_vector4.m_y + m_values[10] * p_vector4.m_z + p_vector4.m_w  * m_values[14],
        m_values[3] * p_vector4.m_x + m_values[7] * p_vector4.m_y + m_values[11] * p_vector4.m_z + p_vector4.m_w  * m_values[15]
    );
}

/**
 * Multiply this by a matrix.
 *
 * @param p_matrix4x4   The matrix to multiply by this.
 *
 * @return              The reference to this after multiplication.
 */
FORCEINLINE Matrix4x4&  Matrix4x4::operator*=   (Matrix4x4 const&   p_matrix4x4)
{
    *this = *this * p_matrix4x4;
    return *this;
}

/**
 * Multiply all components of this matrix by a value
 *
 * @param p_value   The value to use.
 *
 * @return          The reference to this after multiplication.
 */
FORCEINLINE Matrix4x4&  Matrix4x4::operator*=	(float const        p_value)
{
    m_values[0]  *= p_value; 
    m_values[1]  *= p_value; 
    m_values[2]  *= p_value; 
    m_values[3]  *= p_value; 
    m_values[4]  *= p_value;
    m_values[5]  *= p_value;
    m_values[6]  *= p_value;
    m_values[7]  *= p_value;
    m_values[8]  *= p_value;
    m_values[9]  *= p_value;
    m_values[10] *= p_value; 
    m_values[11] *= p_value;
    m_values[12] *= p_value;
    m_values[13] *= p_value;
    m_values[14] *= p_value;
    m_values[15] *= p_value;

	return *this;
}

/*==============================[Public Globale Operators]==============================*/

/**
 * Multiply all components of this matrix by a value
 *
 * @param p_value       The value to use.
 * @param p_matrix4x4   The matrix to multiply.
 *
 * @return              The result of the multiplication
 */
FORCEINLINE Matrix4x4   operator*   (float const        p_scale,
                                     Matrix4x4 const&   p_matrix4x4)
{
    return Matrix4x4
    (
        p_matrix4x4(0) * p_scale, p_matrix4x4(4) * p_scale, p_matrix4x4(8)  * p_scale, p_matrix4x4(12) * p_scale,
        p_matrix4x4(1) * p_scale, p_matrix4x4(5) * p_scale, p_matrix4x4(9)  * p_scale, p_matrix4x4(13) * p_scale,
        p_matrix4x4(2) * p_scale, p_matrix4x4(6) * p_scale, p_matrix4x4(10) * p_scale, p_matrix4x4(14) * p_scale,
        p_matrix4x4(3) * p_scale, p_matrix4x4(7) * p_scale, p_matrix4x4(11) * p_scale, p_matrix4x4(15) * p_scale
    );  
}

#endif // !__MATRIX_4x4_INL__