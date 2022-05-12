#include "PCH.hpp"

#include "PhysicsEngine/PhysXSimulationEventCallback.hpp"
#include "PhysicsEngine/BodyInstance.hpp"
#include "PhysicsEngine/HitResult.hpp"

#include "Components/PrimitiveComponent.hpp"
#include "GameFramework/Entity.hpp"


// ==============================[Virtual Public Local Methods]============================== //

/**
 * Provides early access to the new pose of moving rigid bodies.
 * When this call occurs, rigid bodies having the PxRigidBodyFlag::eENABLE_POSE_INTEGRATION_PREVIEW flag set, were moved by the simulation and their new poses can be accessed through the provided buffers.
 *
 * The provided buffers are valid and can be read until the next call to PxScene::simulate() or PxScene::collide().
 * Buffered user changes to the rigid body pose will not yet be reflected in the provided data. More important, the provided data might contain bodies that have been deleted while the simulation was running. It is the user's responsibility to detect and avoid dereferencing such bodies.
 * This callback gets triggered while the simulation is running. If the provided rigid body references are used to read properties of the object, then the callback has to guarantee no other thread is writing to the same body at the same time.
 * The code in this callback should be lightweight as it can block the simulation, that is, the PxScene::fetchResults() call.
 *
 * @param p_bodyBuffer  The rigid bodies that moved and requested early pose reporting.
 * @param p_poseBuffer  The integrated rigid body poses of the bodies listed in bodyBuffer.
 * @param p_count       The number of entries in the provided buffers.
 */
void PhysXSimulationEventCallback::onAdvance            (physx::PxRigidBody const* const*   p_bodyBuffer,
                                                         physx::PxTransform const*          p_poseBuffer, 
                                                         physx::PxU32 const                 p_count)
{
    // We don't use this methods
}


/**
 * This method is called when a breakable constraint breaks.
 *
 * The user should not release the constraint shader inside this call!
 * No event will get reported if the constraint breaks but gets deleted while the time step is still being simulated.
 *
 * @param p_constraints The constraints which have been broken..
 * @param p_count       The number of constraints.
 */
void PhysXSimulationEventCallback::onConstraintBreak    (physx::PxConstraintInfo*           p_constraints,
                                                         physx::PxU32                       p_count)
{
    // We don't use this methods
}

/**
 * This is called when certain contact events occur.
 * The method will be called for a pair of actors if one of the colliding shape pairs requested contact notification. You request which events are reported using the filter shader/callback mechanism (see PxSimulationFilterShader, PxSimulationFilterCallback, PxPairFlag).
 * Do not keep references to the passed objects, as they will be invalid after this function returns.
 *
 * @param p_pairHeader  Information on the two actors whose shapes triggered a contact report.
 * @param p_pairs       The contact pairs of two actors for which contact reports have been requested. See PxContactPair.
 * @param p_nbPairs     The number of provided contact pairs.
 */
void PhysXSimulationEventCallback::onContact            (physx::PxContactPairHeader const&  p_pairHeader,
                                                         physx::PxContactPair const*        p_pairs, 
                                                         physx::PxU32                       p_nbPairs)
{
    if (p_pairHeader.flags & (physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0 | physx::PxContactPairHeaderFlag::eREMOVED_ACTOR_0))
    {
        return;
    }

    physx::PxActor* const actor0    {p_pairHeader.actors[0]};
    physx::PxActor* const actor1    {p_pairHeader.actors[1]};

    if (actor0 != nullptr && actor1 != nullptr)
    {
        physx::PxRigidActor const* rigidActor0  {actor0->is<physx::PxRigidBody>()};
        physx::PxRigidActor const* rigidActor1  {actor1->is<physx::PxRigidBody>()};

        if (rigidActor0 != nullptr && rigidActor1 != nullptr)
        {
            BodyInstance* const bodyInstance0   {(BodyInstance*)rigidActor0->userData};
            BodyInstance* const bodyInstance1   {(BodyInstance*)rigidActor1->userData};

            if (bodyInstance0 != nullptr && bodyInstance1 != nullptr && bodyInstance0 != bodyInstance1)
            {
                PrimitiveComponent* const component0    {bodyInstance0->m_ownerComponent};
                PrimitiveComponent* const component1    {bodyInstance0->m_ownerComponent};

                if (rigidActor0 != nullptr && rigidActor1 != nullptr && (bodyInstance0->IsNotifyRigidBodyCollision() || bodyInstance1->IsNotifyRigidBodyCollision()))
                {
                    physx::PxContactPairPoint   contactPointBuffer[16];
                    uint32                      numContactPoint {p_pairs->extractContacts(contactPointBuffer,16)};

                    physx::PxVec3 totalNormalImpulse    {0.0f};
                    physx::PxVec3 totalNormal           {0.0f};
                    physx::PxVec3 locations             {0.0f};

                    for (uint32 index {0u} ; index < numContactPoint ; ++index)
                    {
                        physx::PxContactPairPoint const& point {contactPointBuffer[0]};

                        totalNormal += point.normal;
                        totalNormalImpulse += point.impulse.dot(point.normal) * point.normal;
                        locations += point.position;
                    }

                    Vector3 normalImpulse   {PhysXTypeConverter::TVector3(totalNormalImpulse).GetSafeNormalized()};
                    Vector3 normal          {PhysXTypeConverter::TVector3(totalNormal).GetSafeNormalized()};
                    Vector3 location        {PhysXTypeConverter::TVector3(locations / static_cast<float>(numContactPoint))};

                    if (bodyInstance0->IsNotifyRigidBodyCollision())
                    {
                        if (Entity* entity0 {component0->GetOwner()})
                        {
                            HitResult hitResult0;
                            hitResult0.m_entity = component1->GetOwner();
                            hitResult0.m_primitiveComponent = component1;
                            hitResult0.m_impactLocation = location;
                            hitResult0.m_impactNormal = normal;
                            hitResult0.m_isBlockingHit = true;

                            entity0->NotifyHit(component1, hitResult0.m_entity, hitResult0.m_primitiveComponent, true, location, normal, normalImpulse, hitResult0);
                        }
                    }

                    if (bodyInstance1->IsNotifyRigidBodyCollision())
                    {
                        if (Entity* entity1 {component1->GetOwner()})
                        {
                            HitResult hitResult1;
                            hitResult1.m_entity = component0->GetOwner();
                            hitResult1.m_primitiveComponent = component0;
                            hitResult1.m_impactLocation = -location;
                            hitResult1.m_impactNormal = -normal;
                            hitResult1.m_isBlockingHit = true;

                            entity1->NotifyHit(component1, hitResult1.m_entity, hitResult1.m_primitiveComponent, true, location, normal, normalImpulse, hitResult1);
                        }
                    }
                }
            }
        }
    }
}

/**
 * This is called with the actors which have just been put to sleep.
 *
 * Only supported by rigid bodies yet.
 * Only called on actors for which the PxActorFlag eSEND_SLEEP_NOTIFIES has been set.
 * Only the latest sleep state transition happening between fetchResults() of the previous frame and fetchResults() of the current frame will get reported. 
 * For example, let us assume actor A is asleep, then A->wakeUp() gets called, then later A->putToSleep() gets called. At the next simulate/fetchResults() step only an onSleep() event will get triggered because that was the last transition (assuming the simulation does not wake the actor up).
 * If an actor gets newly added to a scene with properties such that it is asleep and the sleep state does not get changed by the user or simulation, then an onSleep() event will get sent at the next simulate/fetchResults() step.
 *
 * @param p_actors  The actors which have just been put to sleep.
 * @param p_count   The number of actors.
 */
void PhysXSimulationEventCallback::onSleep              (physx::PxActor**                   p_actors,
                                                         physx::PxU32                       p_count)
{}

/**
 * This is called with the current trigger pair events.
 * Shapes which have been marked as triggers using PxShapeFlag::eTRIGGER_SHAPE will send events according to the pair flag specification in the filter shader (see PxPairFlag, PxSimulationFilterShader).
 *
 * Trigger shapes will no longer send notification events for interactions with other trigger shapes. 
 * For PhysX 3.4 there is the option to re-enable the reports by raising PxSceneFlag::eDEPRECATED_TRIGGER_TRIGGER_REPORTS. 
 * In PhysX 3.5 there will be no support for these reports any longer. See the 3.4 migration guide for more information.
 *
 * @param p_pairs  The trigger pair events..
 * @param p_count  The number of trigger pair events.
 */
void PhysXSimulationEventCallback::onTrigger            (physx::PxTriggerPair*              p_pairs,
                                                         physx::PxU32                       p_count)
{}

/**
 * This is called with the actors which have just been woken up.
 *
 * Only supported by rigid bodies yet.
 * Only called on actors for which the PxActorFlag eSEND_SLEEP_NOTIFIES has been set.
 * Only the latest sleep state transition happening between fetchResults() of the previous frame and fetchResults() of the current frame will get reported. 
 * For example, let us assume actor A is awake, then A->putToSleep() gets called, then later A->wakeUp() gets called. At the next simulate/fetchResults() step only an onWake() event will get triggered because that was the last transition.
 * If an actor gets newly added to a scene with properties such that it is awake and the sleep state does not get changed by the user or simulation, then an onWake() event will get sent at the next simulate/fetchResults() step.
 *
 * @param p_actors  The actors which just woke up..
 * @param p_count   The number of actors.
 */
void PhysXSimulationEventCallback::onWake               (physx::PxActor**                   p_actors,
                                                         physx::PxU32                       p_count)
{}