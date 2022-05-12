#include "PCH.hpp"

#include "Object/Object.hpp"
#include "GameFramework/Entity.hpp"
#include "GameWorldSettings.hpp"
#include "PhysicsEngine/PhysicsEngine.hpp"
#include "Level.hpp"
#include "World.hpp"
#include "Engine.hpp"

ECollisionChannel DefaultCollisionChannel = (ECollisionChannel)0;

// ============================== [Public Constructors and Destructor] ============================== //

World::World    () 
:   m_currentLevel	{nullptr},
	m_hasBegunPlay	{false},
    m_mainCamera    {nullptr},
    m_physicsScene  {GEngine->GetModuleManager()->Get<PhysicsEngine>().GetPhysicsScene()}
{
    m_physicsScene->SetOwningWorld(this);
}

// ============================== [Public Local Methods] ============================== //

void	            World::BeginPlay	                    ()
{
	if (m_currentLevel != nullptr && !m_hasBegunPlay)
	{
		m_hasBegunPlay = true;

		m_currentLevel->BeginPlay();

        LOG(LogWorld, Display, "World::BeginPlay\n");
	}
}

PhysicsSceneHandle* World::GetPhysicsScene                  ()  const
{
    return m_physicsScene;
}

void                World::SetPhysicsScene                  (PhysicsSceneHandle*    p_scene)
{
    m_physicsScene = p_scene;
    p_scene->SetOwningWorld(this);
}

void	            World::Tick		                        (float const	        p_deltaTime)
{
	if (m_currentLevel != nullptr && m_hasBegunPlay)
	{
		m_currentLevel->Tick(p_deltaTime);
        m_physicsScene->Simulate(p_deltaTime);
	}
}

void	            World::EndPlay	                        ()
{
	if (m_currentLevel != nullptr && m_hasBegunPlay)
	{
		m_currentLevel->EndPlay();

		m_hasBegunPlay = false;

        LOG(LogWorld, Display, "World::EndPlay\n");
	}
}

void                World::SetMainCamera                    (CameraComponent*       p_newMainCamera)
{
    m_mainCamera = p_newMainCamera;
}

CameraComponent*    World::GetMainCamera                    ()  const
{
    return m_mainCamera;
}

bool                World::LineTraceMultiByChannel          (std::vector<HitResult>&            p_outHitResults,
                                                             Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             ECollisionChannel const            p_traceChannel,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastMulti(p_outHitResults, 
                                            p_traceStart, 
                                            p_traceEnd, 
                                            p_traceChannel, 
                                            p_queryParams, 
                                            p_responseParams, 
                                            CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}

bool                World::LineTraceMultiByObjectType       (std::vector<HitResult>&            p_outHitResults,
                                                             Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastMulti(p_outHitResults, 
                                            p_traceStart, 
                                            p_traceEnd, 
                                            DefaultCollisionChannel, 
                                            p_queryParams, 
                                            CollisionResponseParams::DefaultResponseParams, 
                                            p_objectQueryParams);
    }  

    return false;
}


bool                World::LineTraceSingleByChannel         (HitResult&                         p_outHitResult,
                                                             Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             ECollisionChannel const            p_traceChannel,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastSingle(p_outHitResult, 
                                             p_traceStart, 
                                             p_traceEnd,
                                             p_traceChannel, 
                                             p_queryParams, 
                                             p_responseParams, 
                                             CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}

bool                World::LineTraceSingleByObjectType      (HitResult&                         p_outHitResult,
                                                             Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastSingle(p_outHitResult, 
                                             p_traceStart, 
                                             p_traceEnd, 
                                             DefaultCollisionChannel, 
                                             p_queryParams, 
                                             CollisionResponseParams::DefaultResponseParams, 
                                             p_objectQueryParams);
    }                 

    return false;
}


bool                World::LineTraceTestByChannel           (Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             ECollisionChannel const            p_traceChannel,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastTest(p_traceStart, 
                                           p_traceEnd,
                                           p_traceChannel, 
                                           p_queryParams, 
                                           p_responseParams, 
                                           CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}

bool                World::OverlapBlockingTestByChannel     (Vector3 const&                     p_overlapLocation,
                                                             Quaternion const&                  p_overlapRotation, 
                                                             ECollisionChannel const            p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /**= CollisionQueryParams::DefaultQueryParams*/, 
                                                             CollisionResponseParams const&     p_responseParams /**= CollisionResponseParams::DefaultResponseParams*/) const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapBlockingTest(p_collisionShape, 
                                                           p_overlapLocation, 
                                                           p_overlapRotation, 
                                                           p_traceChannel, 
                                                           p_queryParams, 
                                                           p_responseParams, 
                                                           CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}


bool                World::OverlapAnyTestByChannel          (Vector3 const&                     p_overlapLocation,
                                                             Quaternion const&                  p_overlapRotation, 
                                                             ECollisionChannel const            p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /**= CollisionQueryParams::DefaultQueryParams*/, 
                                                             CollisionResponseParams const&     p_responseParams /**= CollisionResponseParams::DefaultResponseParams*/) const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapAnyTest(p_collisionShape, 
                                                      p_overlapLocation, 
                                                      p_overlapRotation, 
                                                      p_traceChannel, 
                                                      p_queryParams, 
                                                      p_responseParams, 
                                                      CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}


bool                World::OverlapBlockingTestByObjectType  (Vector3 const&                     p_overlapLocation,
                                                             Quaternion const&                  p_overlapRotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /**= CollisionQueryParams::DefaultQueryParams*/)   const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapBlockingTest(p_collisionShape, 
                                                           p_overlapLocation, 
                                                           p_overlapRotation, 
                                                           DefaultCollisionChannel, 
                                                           p_queryParams, 
                                                           CollisionResponseParams::DefaultResponseParams, p_objectQueryParams);
    }

    return false;  
}


bool                World::OverlapAnyTestByObjectType       (Vector3 const&                     p_overlapLocation, 
                                                             Quaternion const&                  p_overlapRotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /**= CollisionQueryParams::DefaultQueryParams*/)    const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapAnyTest(p_collisionShape, 
                                                      p_overlapLocation, 
                                                      p_overlapRotation, 
                                                      DefaultCollisionChannel, 
                                                      p_queryParams, 
                                                      CollisionResponseParams::DefaultResponseParams, 
                                                      p_objectQueryParams);
    }

    return false;
}

bool                World::OverlapMutliByChannel            (std::vector<OverlapResult>&        p_outOverlapResults,
                                                             Vector3 const&                     p_overlapLocation, 
                                                             Quaternion const&                  p_overlapRotation, 
                                                             ECollisionChannel const            p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */, 
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */)  const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapMulti(p_outOverlapResults,
                                                    p_collisionShape,
                                                    p_overlapLocation, 
                                                    p_overlapRotation, 
                                                    p_traceChannel, 
                                                    p_queryParams, 
                                                    p_responseParams,
                                                    CollisionObjectQueryParams::DefaultObjectQueryParams);
    }

    return false;
}

bool                World::OverlapMutliByObjectType         (std::vector<OverlapResult>&        p_outOverlapResults,
                                                             Vector3 const&                     p_overlapLocation, 
                                                             Quaternion const&                  p_overlapRotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */)   const
{
    if (m_physicsScene)
    {
        return m_physicsScene->GeometryOverlapMulti(p_outOverlapResults,
                                                    p_collisionShape,
                                                    p_overlapLocation, 
                                                    p_overlapRotation, 
                                                    DefaultCollisionChannel, 
                                                    p_queryParams, 
                                                    CollisionResponseParams::DefaultResponseParams, 
                                                    p_objectQueryParams);
    }

    return false;
}


bool                World::LineTraceTestByObjectType        (Vector3 const&                     p_traceStart,
                                                             Vector3 const&                     p_traceEnd,
                                                             CollisionObjectQueryParams const&  p_objectQueryParams,
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */)  const
{
    if (m_physicsScene->IsValid())
    {
        return m_physicsScene->RaycastTest  (p_traceStart, 
                                             p_traceEnd, 
                                             DefaultCollisionChannel, 
                                             p_queryParams, 
                                             CollisionResponseParams::DefaultResponseParams, 
                                             p_objectQueryParams);
    }  

    return false;
}

bool                World::SweepMultiByChannel              (std::vector<HitResult>&            p_outHitResults,
                                                             Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             ECollisionChannel                  p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParam  /** = CollisionResponseParams::DefaultResponseParams */) const
{
    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceMultiByChannel(p_outHitResults, p_sweepStart, p_sweepEnd, p_traceChannel, p_queryParams, p_responseParam);
        }

        else
        {
            return m_physicsScene->GeometrySweepMulti(p_outHitResults, 
                                                      p_collisionShape,
                                                      p_rotation,
                                                      p_sweepStart,
                                                      p_sweepEnd,
                                                      p_traceChannel,
                                                      p_queryParams,
                                                      p_responseParam,
                                                      CollisionObjectQueryParams::DefaultObjectQueryParams);
        }
    }

    return false;
}

bool                World::SweepMultiByObjectType           (std::vector<HitResult>&            p_outHitResults,
                                                             Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */)   const
{
    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceMultiByObjectType(p_outHitResults, p_sweepStart, p_sweepEnd, p_objectQueryParams, p_queryParams);
        }

        else
        {
            return m_physicsScene->GeometrySweepMulti(p_outHitResults, 
                                                      p_collisionShape,
                                                      p_rotation,
                                                      p_sweepStart,
                                                      p_sweepEnd,
                                                      DefaultCollisionChannel,
                                                      p_queryParams,
                                                      CollisionResponseParams::DefaultResponseParams,
                                                      p_objectQueryParams);
        }
    }

    return false;
}



 bool               World::SweepSingleByChannel             (HitResult&                         p_outHitResult,
                                                             Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             ECollisionChannel                  p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */) const
 {
    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceSingleByChannel(p_outHitResult, p_sweepStart, p_sweepEnd, p_traceChannel, p_queryParams, p_responseParams);
        }

        else
        {
            return m_physicsScene->GeometrySweepSingle(p_outHitResult,
                                                       p_collisionShape, 
                                                       p_rotation, 
                                                       p_sweepStart, 
                                                       p_sweepEnd, 
                                                       p_traceChannel, 
                                                       p_queryParams, 
                                                       p_responseParams, 
                                                       CollisionObjectQueryParams::DefaultObjectQueryParams);
        }
    }  
 
    return false;
 }

bool                World::SweepSingleByObjectType          (HitResult&                         p_outHitResult,
                                                             Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */) const
{

    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceSingleByObjectType(p_outHitResult,p_sweepStart, p_sweepEnd, p_objectQueryParams, p_queryParams);
        }
        else
        {
            return m_physicsScene->GeometrySweepSingle(p_outHitResult,
                                                       p_collisionShape, 
                                                       p_rotation, 
                                                       p_sweepStart, 
                                                       p_sweepEnd, 
                                                       DefaultCollisionChannel, 
                                                       p_queryParams, 
                                                       CollisionResponseParams::DefaultResponseParams, 
                                                       CollisionObjectQueryParams::DefaultObjectQueryParams);
        }
    }

    return false;
}

 bool               World::SweepTestByChannel               (Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             ECollisionChannel                  p_traceChannel, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */,
                                                             CollisionResponseParams const&     p_responseParams /** = CollisionResponseParams::DefaultResponseParams */) const
 {
    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceTestByChannel(p_sweepStart,
                                          p_sweepEnd,
                                          p_traceChannel, 
                                          p_queryParams, 
                                          p_responseParams);
        }

        else
        {
            return m_physicsScene->GeometrySweepTest(p_collisionShape, 
                                                     p_rotation, 
                                                     p_sweepStart, 
                                                     p_sweepEnd, 
                                                     p_traceChannel, 
                                                     p_queryParams, 
                                                     p_responseParams, 
                                                     CollisionObjectQueryParams::DefaultObjectQueryParams);
        }
    }  
 
    return false;
 }

bool                World::SweepTestByObjectType            (Vector3 const&                     p_sweepStart, 
                                                             Vector3 const&                     p_sweepEnd, 
                                                             Quaternion const&                  p_rotation, 
                                                             CollisionObjectQueryParams&        p_objectQueryParams, 
                                                             CollisionShape const&              p_collisionShape, 
                                                             CollisionQueryParams const&        p_queryParams /** = CollisionQueryParams::DefaultQueryParams */) const
{

    if (m_physicsScene->IsValid())
    {
        if (p_collisionShape.IsNearlyZero())
        {
            return LineTraceTestByObjectType(p_sweepStart, p_sweepEnd, p_objectQueryParams, p_queryParams);
        }
        else
        {
            return m_physicsScene->GeometrySweepTest(p_collisionShape, 
                                                     p_rotation, 
                                                     p_sweepStart, 
                                                     p_sweepEnd, 
                                                     DefaultCollisionChannel, 
                                                     p_queryParams, 
                                                     CollisionResponseParams::DefaultResponseParams, 
                                                     CollisionObjectQueryParams::DefaultObjectQueryParams);
        }
    }

    return false;
}