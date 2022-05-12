#include "PCH.hpp"

#include "PhysicsEngine/PhysXTypeConverter.hpp"


// ==============================[Public Static Methods]============================== //

ECollisionShapeType         PhysXTypeConverter::TShapeType  (physx::PxGeometryType::Enum const  p_shapeTyppe)
{
    switch (p_shapeTyppe)
    {
        case physx::PxGeometryType::eBOX:       return ECollisionShapeType::Box;
        case physx::PxGeometryType::eCAPSULE:   return ECollisionShapeType::Capsule;
        case physx::PxGeometryType::eSPHERE:    return ECollisionShapeType::Sphere;
    }

    return ECollisionShapeType::None;
}

physx::PxGeometryType::Enum PhysXTypeConverter::TShapeType  (ECollisionShapeType const          p_shapeTyppe)
{
    switch (p_shapeTyppe)
    {
        case ECollisionShapeType::Box:      physx::PxGeometryType::eBOX;
        case ECollisionShapeType::Capsule:  physx::PxGeometryType::eCAPSULE;
        case ECollisionShapeType::Sphere:   physx::PxGeometryType::eSPHERE;
    }

    return physx::PxGeometryType::eINVALID;
}