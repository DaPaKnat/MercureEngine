#include "PCH.hpp"

#include "PhysicsEngine/InitBodyHelper.hpp"

#include "Components/PrimitiveComponent.hpp"

#include "PhysicsEngine/BodyInstance.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"
#include "PhysicsEngine/PhysicsActorCreationParameters.hpp"
#include "PhysicsEngine/PhysicsActorHandle.hpp"
#include "PhysicsEngine/PhysicsShapeHandle.hpp"
#include "PhysicsEngine/PhysicsInterface.hpp"

#include "Engine.hpp"
#include "EngineTypes.hpp"

#include "PxPhysicsAPI.h"

// ==============================[Public Constructor + Destructor]============================== //

InitBodyHelper::InitBodyHelper  (BodyInstance*          p_bodyInstance,
                                 Transform const&       p_transform,
                                 PrimitiveComponent*    p_primitiveComponent,
                                 PhysicsSceneHandle*    p_physicsScene)
:   m_bodyInstance          {p_bodyInstance},
    m_primitiveComponent    {p_primitiveComponent},
    m_transform             {p_transform},
    m_physicsSceneHandle    {p_physicsScene}
{}

// ==============================[Private Local Methods]============================== //

void    InitBodyHelper::CreateActor         ()
{
    if (m_bodyInstance && !m_bodyInstance->IsValidBodyInstance())
    {
        ECollisionEnabled const         collisionType       {m_bodyInstance->GetCollisionEnabled()};
        bool const                      disableSimulation   {!CollisionEnabledHasPhysics(collisionType)};
        PhysicsActorCreationParameters  actorParams;
        
        actorParams.m_physicsScene      = m_physicsSceneHandle;
        actorParams.m_transform         = m_transform;
        actorParams.m_enableGravity     = m_bodyInstance->IsGravityEnabled();
        actorParams.m_queryOnly         = disableSimulation;
        actorParams.m_isStatic          = false;

        if (IsStatic() == true)
        {
            actorParams.m_isStatic = true;
            m_bodyInstance->SetPhysicsActorHandle(PhysicsInterface::CreateActorHandle(actorParams));
        }

        else
        {
            m_bodyInstance->SetPhysicsActorHandle(PhysicsInterface::CreateActorHandle(actorParams));

            m_bodyInstance->GetPhysicsActorHandle()->SetCCDEnabled(m_bodyInstance->m_useCCD);
            m_bodyInstance->GetPhysicsActorHandle()->SetIsKinematic(!m_bodyInstance->ShouldSimulatingPhysics());

            m_bodyInstance->GetPhysicsActorHandle()->SetSendsSleepNotifies(!m_bodyInstance->m_generateWakeEvent);
        }
    }

    else
    {
        LOG(LogPhysics, Fatal, "You can't init a Physics Actor Handle if he's already init");
    }
}

bool    InitBodyHelper::CreateActorAndShape ()
{
    if (m_bodyInstance != nullptr && !m_bodyInstance->IsValidBodyInstance())
    {
        //if (!IsStatic())
        //{
        //    m_bodyInstance->m_simulatePhysics = false; 
        //}

        CreateActor();
        bool const initFail {!CreateShape()};

        if (initFail == false)
        {
            m_bodyInstance->GetPhysicsActorHandle()->SetUserData(m_bodyInstance);
            return true;
        }
    }

    LOG(LogPhysics, Warning, "Init body Instance failed !");
    return false;
}

bool    InitBodyHelper::CreateShape         ()
{
    BodyCollisionData   bodyCollisionData;
    m_bodyInstance->BuildBodyFilterData(bodyCollisionData.m_collisionFilterData);
    m_bodyInstance->BuildBodyCollisionFlags(bodyCollisionData.m_collisionFlags);

    PhysicsActorHandle actorHandle      {*m_bodyInstance->GetPhysicsActorHandle()};

    // PhysX Part //

    physx::PxGeometry*  geo {PhysicsInterface::CreateGeometry(m_primitiveComponent)};
    
    if (geo != nullptr)
    {
        physx::PxMaterial*  material    {GEngine->GetModuleManager()->Get<PhysicsEngine>().GetDefaultPhysicalMaterial()->GetPhysicsMaterialHandle().m_material};
        physx::PxShapeFlags shapeFlags;
        
        if (bodyCollisionData.m_collisionFlags.m_enableQueryCollision)
            shapeFlags |= physx::PxShapeFlag::eSCENE_QUERY_SHAPE; 

        if (bodyCollisionData.m_collisionFlags.m_enableSimulationCollisionSimple || bodyCollisionData.m_collisionFlags.m_enableSimulationCollisionComplex)
            shapeFlags |= physx::PxShapeFlag::eSIMULATION_SHAPE; 
    
        physx::PxShape*     shape       {GEngine->GetModuleManager()->Get<PhysicsEngine>().m_physXSDK->createShape(*geo, *material, true, shapeFlags)};

        if (shape)
        {
            PhysicsShapeHandle shapeHandle {shape};
            shapeHandle.SetLocalTransform(m_primitiveComponent->GetRelativeTransform());
            shapeHandle.SetQueryFilter(bodyCollisionData.m_collisionFilterData.m_querySimpleFilter);
            shapeHandle.SetSimulationFilter(bodyCollisionData.m_collisionFilterData.m_simulationFilter);
            actorHandle.AttachShape(shapeHandle);

            shape->release();
        }

        delete geo;
        return true;
    }

    return false;
}

void    InitBodyHelper::InitBody            ()
{
    if (CreateActorAndShape())
    {
        if (m_physicsSceneHandle && m_physicsSceneHandle->IsValid())
        {
            m_physicsSceneHandle->RegisterActor(m_bodyInstance->GetPhysicsActorHandle());
        }

        // Set up dynamic instance data
        if (IsStatic() == false)
        {
            //m_bodyInstance->InitDynamicProperties();
        }
    }
}

bool    InitBodyHelper::IsStatic            ()
{
    return m_primitiveComponent && m_primitiveComponent->GetMobility() == EComponentMobility::Static;
}