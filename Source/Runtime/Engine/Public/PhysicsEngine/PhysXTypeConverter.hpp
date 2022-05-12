#ifndef __PHYSX_TYPE_CONVERTER_HPP__
#define __PHYSX_TYPE_CONVERTER_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"
#include "CollisionShape.hpp"
#include "CollisionFilterData.hpp"

/** A class with static functionalitie that convert PhysX classes into Engine classes and vice versa */
struct ENGINE_API PhysXTypeConverter
{
    // ==============================[Public Static Methods]============================== //

        static FORCEINLINE  Bounds                  TBounds     (physx::PxBounds3 const&            p_bound);

        static FORCEINLINE  physx::PxBounds3        TBounds     (Bounds const&                      p_bound);

        static FORCEINLINE  CollisionFilterData     TFilterData (physx::PxFilterData const&         p_filterData);

        static FORCEINLINE  physx::PxFilterData     TFilterData (CollisionFilterData const&         p_filterData);

        static FORCEINLINE  Matrix4x4               TMatrix4x4  (physx::PxMat44 const&              p_matrix);

        static FORCEINLINE  physx::PxMat44          TMatrix4x4  (Matrix4x4 const&                   p_matrix);
        
        static FORCEINLINE  Plane                   TPlane      (physx::PxPlane const&              p_plane);
        
        static FORCEINLINE  physx::PxPlane          TPlane      (Plane const&                       p_plane);
        
        static FORCEINLINE  Quaternion              TQuaternion (physx::PxQuat const&               p_quaternion);
        
        static FORCEINLINE  physx::PxQuat           TQuaternion (Quaternion const&                  p_quaternion);
        
        static ECollisionShapeType                  TShapeType  (physx::PxGeometryType::Enum const p_shapeTyppe);

        static physx::PxGeometryType::Enum          TShapeType  (ECollisionShapeType const          p_shapeType);

        static FORCEINLINE  Transform               TTransform  (physx::PxTransform const&          p_transform);
        
        static FORCEINLINE  physx::PxTransform      TTransform  (Transform const&                   p_transform);
        
        static FORCEINLINE  Vector2                 TVector2    (physx::PxVec2  const&              p_vector2);
        
        static FORCEINLINE  physx::PxVec2           TVector2    (Vector2 const&                     p_vector2);
        
        static FORCEINLINE  Vector3                 TVector3    (physx::PxVec3  const&              p_vector3);
        
        static FORCEINLINE  physx::PxVec3           TVector3    (Vector3 const&                     p_vector3);
        
        static FORCEINLINE  Vector4                 TVector4    (physx::PxVec4  const&              p_vector4);
        
        static FORCEINLINE  physx::PxVec4           TVector4    (Vector4 const&                     p_vector4);

}; // !class PhysXTypeConverter

#include "PhysXTypeConverter.inl"

#endif // !__PHYS_X_TYPE_CONVERTER_HPP__