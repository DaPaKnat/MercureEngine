#ifndef __PHYSICS_SHAPE_HANDLE_HPP__
#define __PHYSICS_SHAPE_HANDLE_HPP__

#include "CoreMinimal.hpp"

#include "PxPhysicsAPI.h"

#include "PhysicsInterface.hpp"
#include "PhysXTypeConverter.hpp"

#include "CollisionShape.hpp"

struct CollisionFilterData;

class PhysicsShapeHandle
{
    public:

    // ==============================[Public Local Properties]============================== //
        
        /** ... */
        physx::PxShape* m_shape;

    // ==============================[Public Constructor + Destructor]============================== //

        PhysicsShapeHandle   ();

        PhysicsShapeHandle   (physx::PxShape*           p_shape);

        PhysicsShapeHandle   (PhysicsShapeHandle const& p_copy);

        PhysicsShapeHandle   (PhysicsShapeHandle&&      p_move);

        ~PhysicsShapeHandle  ()                                              = default;

    // ==============================[Public Local Methods]============================== //
    
        Transform           GetLocalTransform       ()  const;
        
        CollisionFilterData GetQueryFilterData      ()  const;

        ECollisionShapeType GetShapeType            ()  const;

        CollisionFilterData GetSimulationFilter     ()  const;

        Transform           GetTransform            ()  const;

        void*               GetUserData             ()  const;

        bool                IsQueryShape            ()  const;
        
        bool                IsShapeType             (ECollisionShapeType const  p_shapeType);

        bool                IsSimulationShape       ()  const;

        FORCEINLINE bool    IsValid                 ()  const;

        void                SetGeometry             (physx::PxGeometry const&   p_geometry);
        
        void                SetIsQueryShape         (bool const                 p_isQueryShape);

        void                SetIsSimulationShape    (bool const                 p_isSimulationShape);

        void                SetLocalTransform       (Transform const&           p_newLocalTransform);

        void                SetQueryFilter          (CollisionFilterData const& p_filterData);

        void                SetSimulationFilter     (CollisionFilterData const& p_filterData);

        void                SetUserData             (void*                      p_userData);

//      void                SetMaterials            ();

        void                Release                 ();

    // ==============================[Public Local Operator]============================== //

        bool                operator==  (PhysicsShapeHandle const&  p_other)    const;
        bool                operator!=  (PhysicsShapeHandle const&  p_other)    const;

        PhysicsShapeHandle& operator=   (PhysicsShapeHandle const&  p_copy);
        PhysicsShapeHandle& operator=   (PhysicsShapeHandle&&       p_move);

}; // !class PhysicsShapeHandle

#include "PhysicsShapeHandle.inl"

#endif // !__PHYSICS_SHAPE_HANDLE_HPP__