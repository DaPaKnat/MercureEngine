#ifndef __PHYSX_TYPE_CONVERTER_INL__
#define __PHYSX_TYPE_CONVERTER_INL__

// ==============================[Public Static Methods]============================== //

/**
 * Convert a PhysX Bounding Box into an Engine Bounding Box.
 *
 * @param p_matrix  The PhysX Bounding Box to convert.
 *
 * @return The Bounding Box converted (Engine Version).
 */
FORCEINLINE Bounds              PhysXTypeConverter::TBounds     (physx::PxBounds3 const&    p_bounds)
{
    return Bounds
    (
         PhysXTypeConverter::TVector3(p_bounds.minimum), 
         PhysXTypeConverter::TVector3(p_bounds.maximum)
    );
}

/**
 * Convert an Engine Bounding Box into a PhysX Bounding Box.
 *
 * @param p_matrix  The Engine Bounding Box to convert.
 *
 * @return The Bounding Box converted (PhysX Version).
 */  
FORCEINLINE physx::PxBounds3    PhysXTypeConverter::TBounds     (Bounds const&              p_bounds)
{
    return physx::PxBounds3
    (
         PhysXTypeConverter::TVector3(p_bounds.m_min), 
         PhysXTypeConverter::TVector3(p_bounds.m_max)
    );
}

FORCEINLINE CollisionFilterData PhysXTypeConverter::TFilterData (physx::PxFilterData const& p_filterData)
{
    return CollisionFilterData
    (
        p_filterData.word0,
        p_filterData.word1,
        p_filterData.word2,
        p_filterData.word3
    );
}

FORCEINLINE physx::PxFilterData PhysXTypeConverter::TFilterData (CollisionFilterData const& p_filterData)
{
    return physx::PxFilterData
    (
        p_filterData.m_word0,
        p_filterData.m_word1,
        p_filterData.m_word2,
        p_filterData.m_word3
    );
}

/**
 * Convert a PhysX Matrix4x4 into an Engine Matrix4x4.
 *
 * @param p_matrix  The PhysX Matrix4x4 to convert.
 *
 * @return The Matrix4x4 converted (Engine Version).
 */
FORCEINLINE Matrix4x4           PhysXTypeConverter::TMatrix4x4  (physx::PxMat44 const&      p_matrix)
{
    return Matrix4x4
    (
        PhysXTypeConverter::TVector4(p_matrix.column0),
        PhysXTypeConverter::TVector4(p_matrix.column1),
        PhysXTypeConverter::TVector4(p_matrix.column2),
        PhysXTypeConverter::TVector4(p_matrix.column3)
    );
}
                                
/**
 * Convert an Engine Matrix4x4 into a PhysX Matrix4x4.
 *
 * @param p_matrix  The Engine Matrix4x4 to convert.
 *
 * @return The Matrix4x4 converted (PhysX Version).
 */                     
FORCEINLINE physx::PxMat44      PhysXTypeConverter::TMatrix4x4  (Matrix4x4 const&           p_matrix)
{
    return physx::PxMat44
    (
        PhysXTypeConverter::TVector4(p_matrix.GetColumn(0)),
        PhysXTypeConverter::TVector4(p_matrix.GetColumn(1)),
        PhysXTypeConverter::TVector4(p_matrix.GetColumn(2)),
        PhysXTypeConverter::TVector4(p_matrix.GetColumn(3))
    );
}
                                
/**
 * Convert a PhysX Plane into an Engine Plane.
 *
 * @param p_plane   The PhysX Plane to convert.
 *
 * @return The Plane converted (Engine Version).
 */                     
FORCEINLINE Plane               PhysXTypeConverter::TPlane      (physx::PxPlane const&      p_plane)
{
    return Plane(p_plane.n.x, p_plane.n.y, p_plane.n.z, p_plane.d);
}
                                
/**
 * Convert an Engine Plane into a PhysX Plane.
 *
 * @param p_plane   The Engine Plane to convert.
 *
 * @return The Plane converted (PhysX Version).
 */                     
FORCEINLINE physx::PxPlane      PhysXTypeConverter::TPlane      (Plane const&               p_plane)
{
    return physx::PxPlane(p_plane.m_normal.m_x, p_plane.m_normal.m_y, p_plane.m_normal.m_z, p_plane.m_distance);
}
                                
/**
 * Convert a PhysX Quaternion into an Engine Quaternion.
 *
 * @param p_quaternion  The PhysX Quaternion to convert.
 *
 * @return The Quaternion converted (Engine Version).
 */                     
FORCEINLINE Quaternion          PhysXTypeConverter::TQuaternion (physx::PxQuat const&       p_quaternion)
{
    return Quaternion(p_quaternion.x, p_quaternion.y, p_quaternion.z, p_quaternion.w);
}
                                
/**
 * Convert an Engine Quaternion into a PhysX Quaternion.
 *
 * @param p_quaternion   The Engine Quaternion to convert.
 *
 * @return The Quaternion converted (PhysX Version).
 */                     
FORCEINLINE physx::PxQuat       PhysXTypeConverter::TQuaternion (Quaternion const&          p_quaternion)
{
    return physx::PxQuat(p_quaternion.m_x, p_quaternion.m_y, p_quaternion.m_z, p_quaternion.m_w);
}
               
/**
 * Convert a PhysX Transform into an Engine Transform.
 *
 * @param p_transform  The PhysX Transform to convert.
 *
 * @return The Transform converted (Engine Version).
 */                     
FORCEINLINE Transform           PhysXTypeConverter::TTransform  (physx::PxTransform const&  p_transform)
{
    return Transform
    (
        PhysXTypeConverter::TVector3(p_transform.p),
        PhysXTypeConverter::TQuaternion(p_transform.q)
    );
}

/**
 * Convert an Engine Transform into a PhysX Transform.
 *
 * @param p_transform   The Engine Transform to convert.
 *
 * @return The Transform converted (PhysX Version).
 */
FORCEINLINE physx::PxTransform  PhysXTypeConverter::TTransform  (Transform const&           p_transform)
{
    return physx::PxTransform
    (
        PhysXTypeConverter::TVector3(p_transform.m_translation),
        PhysXTypeConverter::TQuaternion(p_transform.m_rotation)
    );
}

/**
 * Convert a PhysX Vector2 into an Engine Vector2.
 *
 * @param p_vector2 The PhysX Vector2 to convert.
 *
 * @return The Vector2 converted (Engine Version).
 */
FORCEINLINE Vector2             PhysXTypeConverter::TVector2    (physx::PxVec2  const&      p_vector2)
{
    return Vector2(p_vector2.x, p_vector2.y);
}
                                
/**
 * Convert an Vector2 Transform into a PhysX Vector2.
 *
 * @param p_vector2 The Engine Vector2 to convert.
 *
 * @return The Vector2 converted (PhysX Version).
 */                     
FORCEINLINE physx::PxVec2       PhysXTypeConverter::TVector2    (Vector2 const&             p_vector2)
{
    return physx::PxVec2(p_vector2.m_x, p_vector2.m_y);
}
                                
/**
 * Convert a PhysX Vector3 into an Engine Vector3.
 *
 * @param p_vector3 The PhysX Vector3 to convert.
 *
 * @return The Vector3 converted (Engine Version).
 */                     
FORCEINLINE Vector3             PhysXTypeConverter::TVector3    (physx::PxVec3  const&      p_vector3)
{
    return Vector3(p_vector3.x, p_vector3.y, p_vector3.z);
}
                                
/**
 * Convert an Vector3 Transform into a PhysX Vector3.
 *
 * @param p_vector3 The Engine Vector3 to convert.
 *
 * @return The Vector3 converted (PhysX Version).
 */                     
FORCEINLINE physx::PxVec3       PhysXTypeConverter::TVector3    (Vector3 const&             p_vector3)
{
    return physx::PxVec3(p_vector3.m_x, p_vector3.m_y, p_vector3.m_z);
}

/**
 * Convert a PhysX Vector4 into an Engine Vector4.
 *
 * @param p_vector4 The PhysX Vector4 to convert.
 *
 * @return The Vector4 converted (Engine Version).
 */
FORCEINLINE Vector4             PhysXTypeConverter::TVector4    (physx::PxVec4  const&      p_vector4)
{
    return Vector4(p_vector4.x, p_vector4.y, p_vector4.z, p_vector4.w);
}

/**
 * Convert an Vector4 Transform into a PhysX Vector4.
 *
 * @param p_vector4 The Engine Vector4 to convert.
 *
 * @return The Vector4 converted (PhysX Version).
 */
FORCEINLINE physx::PxVec4       PhysXTypeConverter::TVector4    (Vector4 const&             p_vector4)
{
    return physx::PxVec4(p_vector4.m_x, p_vector4.m_y, p_vector4.m_z, p_vector4.m_w);
}

#endif // !class __PHYS_X_TYPE_CONVERTER_INL__