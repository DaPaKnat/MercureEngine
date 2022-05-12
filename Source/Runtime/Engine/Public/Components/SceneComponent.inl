#ifndef __SCENE_COMPONENT_INL__
#define __SCENE_COMPONENT_INL__

// =================================[Virtual Public Local Methods]================================= //

INLINE      void    SceneComponent::Serialize   (Json&  p_serialize)    const
{
    p_serialize = *this;

    SerializeHierarchy(p_serialize);
}

/**
 * Is this component visible or not.
 *
 * @return True if this component is visible, otherwise false.
 */
FORCEINLINE bool    SceneComponent::IsVisible   ()                      const
{
    return m_isVisibleInGame;
}

// =================================[Public Local Methods]================================= //

/**
 * Call UpdateComponentToWorld if m_isLocalToWorldUpdated is false
 */
FORCEINLINE void                SceneComponent::ConditionUpdateLocalToWorld     ()
{
    if (!m_isLocalToWorldUpdated)
    {
        UpdateLocalToWorld();
    }
}

/**
 * Gets the SceneComponent we are attached to.
 *
 * @return The SceneComponent parent of this.
 */
FORCEINLINE SceneComponent*     SceneComponent::GetAttachParent                 ()  const
{
    return m_attachParent;
}

/**
 * Get the backward (-Z) vector (Length 1.0f) from this component, in world space.
 *
 * @return The backward (-Z) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetBackward                     ()  const
{
    return m_localToWorld.GetBackward();
}

/** 
 * Gets the attached child component at the specified index. 
 *
 * @param p_childrenIndex   The index of the children component that we want.
 *
 * @return  The child component corresponding to the index.
 * @reutrn  nullptr if index < 0 or index >= GetChildrenCount.
 */
FORCEINLINE SceneComponent*     SceneComponent::GetChildren                     (int32 const        p_childrenIndex)  const
{
    SceneComponent* children    {nullptr};

    if (0 <= p_childrenIndex && p_childrenIndex < GetChildrenCount())
        children = m_attachChildrens[p_childrenIndex];

    return children;
}

/**
 * Gets the number of attached children components.
 *
 * @return The number of attached children components.
 */
FORCEINLINE int32               SceneComponent::GetChildrenCount                ()  const
{
    return static_cast<int32>(m_attachChildrens.size());
}

/**
 * Get the down (+Y) vector (Length 1.0f) from this component, in world space.
 *
 * @return The down (+Y) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetDown                         ()  const
{
    return m_localToWorld.GetDown();
}

/**
 * Get the forward (+Z) vector (Length 1.0f) from this component, in world space.
 *
 * @return The forward (+Z) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetForward                      ()  const
{
    return m_localToWorld.GetForward();
}

/**
 * Get the left (-X) vector (Length 1.0f) from this component, in world space.
 *
 * @return The left (-X) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetLeft                         ()  const
{
    return m_localToWorld.GetLeft();
}

FORCEINLINE EComponentMobility  SceneComponent::GetMobility                     ()  const
{
    return m_mobility;
}

/**
 * Get the right (+X) vector (Length 1.0f) from this component, in world space.
 *
 * @return The right (+X) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetRight                        ()  const
{
    return m_localToWorld.GetRight();
}

/**
 * Get the location of the component relative to its parent.
 *
 * @return  The rotation of the component relative to its parents.
 */
FORCEINLINE Vector3             SceneComponent::GetRelativeLocation             ()  const
{
    return m_relativeTransform.m_translation;
}

/**
 * Get the rotation of the component relative to its parent.
 *
 * @return  The rotation of the component relative to its parents.
 */
FORCEINLINE Quaternion          SceneComponent::GetRelativeRotation             ()  const
{
    return m_relativeTransform.m_rotation;
}

/**
 * Get the scale of the component relative to its parent.
 *
 * @return  The scale of the component relative to its parents.
 */
FORCEINLINE Vector3             SceneComponent::GetRelativeScale                ()  const
{
    return m_relativeTransform.m_scale;
}

/**
 * Get the transform of the component relative to its parent.
 *
 * @return  The transform of the component relative to its parents.
 */
FORCEINLINE Transform           SceneComponent::GetRelativeTransform            ()  const
{
    return m_relativeTransform;
}

/**
 * Get the up (-Y) vector (Length 1.0f) from this component, in world space.
 *
 * @return The up (-Y) vector from this component.
 *
 */
FORCEINLINE Vector3             SceneComponent::GetUp                           ()  const
{
    return m_localToWorld.GetUp();
}

/**
 * Gets the location in world space of this component.
 *
 * @return  The location in world space of this component.
 */
FORCEINLINE Vector3             SceneComponent::GetWorldLocation                ()  const
{
    return m_localToWorld.m_translation;
}

/**
 * Gets the rotation in world space of this component.
 * 
 * @return  The rotation in world space of this component.
 */
FORCEINLINE Quaternion          SceneComponent::GetWorldRotation                ()  const
{
    return m_localToWorld.m_rotation;
}

/**
 * Gets the scale in world space of this component.
 *
 * @return  The scale in world space of this component.
 */
FORCEINLINE Vector3             SceneComponent::GetWorldScale                   ()  const
{
    return m_localToWorld.m_scale;
}

/**
 * Gets the current local to world transfrom of this component.
 *
 * @return The current local to world transform of this component.
 */
FORCEINLINE Transform const&    SceneComponent::GetWorldTransform               ()  const
{
    return m_localToWorld;
}


/**
 * Checks if any form of collision (Query or Physics) is enabled on this component.
 *
 * @return True if any form of collision is enabled, otherwise false.
 */
FORCEINLINE bool                SceneComponent::IsCollisionEnabled              ()  const
{
    return GetCollisionEnabled() != ECollisionEnabled::NoCollision;
}

/**
 * Checks if any query collision is enabled on this component.
 *
 * @return True if any query collision is enabled on this component.
 */
FORCEINLINE bool                SceneComponent::IsQueryCollisionEnabled         ()  const
{
    return CollisionEnabledHasQuery(GetCollisionEnabled());
}

/**
 * Checks if any physics collision is enabled on this component.
 *
 * @return True if any physics collision is enabled on this component.
 */
FORCEINLINE bool                SceneComponent::IsPhysicsCollisionEnabled       ()  const
{
    return CollisionEnabledHasPhysics(GetCollisionEnabled());
}

FORCEINLINE bool                SceneComponent::MoveComponent                   (Vector3 const&     p_deltaWorldOffset,
                                                                                 Quaternion const&  p_newWorldRotation,
                                                                                 bool const         p_sweep /** = false */,
                                                                                 HitResult*         p_outHitResult /** = nullptr */)
{
    return MoveComponentImpl(p_deltaWorldOffset, p_newWorldRotation, p_sweep, p_outHitResult);
}

/**
 * Set the location of the component relative to its parent.
 *
 * @param p_newRelativeLocation The new location of the component relative to its parent.
 */
FORCEINLINE void                SceneComponent::SetRelativeLocation             (Vector3 const&     p_newRelativeLocation,
                                                                                 bool const         p_sweep /** = false */, 
                                                                                 HitResult*         p_outHitResult /** = nullptr */)
{
    SetRelativeLocationAndRotation(p_newRelativeLocation, GetRelativeRotation(), p_sweep, p_outHitResult);
}

/**
 * Set the location and rotation of the component relative to its parent.
 *
 * @param   p_newRelativeLocation   The new location of the component relative to its parent.
 * @param   p_newRelativeRotation   The new rotation of the component relative to its parent.
 */
FORCEINLINE void                SceneComponent::SetRelativeLocationAndRotation  (Vector3 const&     p_newRelativeLocation,
                                                                                 Quaternion const&  p_newRelativeRotation,
                                                                                 bool const         p_sweep /** = false */,
                                                                                 HitResult*         p_outHitResult /** = nullptr */)
{
    ConditionUpdateLocalToWorld();

    Transform const DesiredRelativeTransform    {p_newRelativeLocation, p_newRelativeRotation};
    Transform const DesiredWorldTransform       {ComputeNewLocalToWorld(DesiredRelativeTransform)};
    Vector3 const   DesiredDelta                {DesiredWorldTransform.m_translation - GetWorldTransform().m_translation};

    MoveComponent(DesiredDelta, DesiredWorldTransform.m_rotation, p_sweep, p_outHitResult);
}

/**
 * Set the rotation of the component relative to its parent.
 *
 * @param p_newRelativeRotation The new rotation of the component relative to its parent.
 */
FORCEINLINE void                SceneComponent::SetRelativeRotation             (Quaternion const&  p_newRelativeRotation,
                                                                                 bool const         p_sweep /** = false */,
                                                                                 HitResult*         p_outHitResult /** = nullptr */)
{
    SetRelativeLocationAndRotation(GetRelativeLocation(), p_newRelativeRotation, p_sweep, p_outHitResult);
}

/**
 * Toggle visibility in game of the component.
 *
 *@param p_propagateToChildren  Should we toggle the visibility in game of all children. It's false by default;
 */
FORCEINLINE void                SceneComponent::ToggleVisibilityInGame          (bool const         p_propagateToChildren)
{
    SetVisibilityInGame(!m_isVisibleInGame,p_propagateToChildren);
}

// =================================[Protected Local Methods]================================= //

FORCEINLINE Transform           SceneComponent::ComputeNewLocalToWorld                (Transform const&       p_newRelativeTransform,
                                                                                       SceneComponent const*  p_parent)               const
{
    p_parent = (p_parent ? p_parent : GetAttachParent());

    if (p_parent)
    {
        return p_parent->GetWorldTransform() *  p_newRelativeTransform;
    }
    
    else
    {
        return p_newRelativeTransform;
    }
}


FORCEINLINE bool                SceneComponent::InternalSetWorldLocationAndRotation   (Vector3 const&         p_newWorldLocation,
                                                                                       Quaternion const&      p_newWorldRotation)
{
    Vector3     newLocation    {p_newWorldLocation};
    Quaternion  newRotation    {p_newWorldRotation};

    // If attached to something, trnasform into local space
    if (GetAttachParent())
    {
        Transform const parentToWorld   {GetAttachParent()->GetWorldTransform()};

        if (m_localToWorld.HasScaleWithNegativeValue() || parentToWorld.HasScaleWithNegativeValue())
        {
            Transform const worldTransform      {p_newWorldLocation, p_newWorldRotation, GetRelativeScale() * parentToWorld.m_scale};
            Transform const relativeTransform   {Transform::GetRelative(worldTransform, parentToWorld)};

            newLocation = relativeTransform.m_translation;
            newRotation = relativeTransform.m_rotation;
        }

        else
        {
            newLocation = parentToWorld.InverseTransformPoint(newLocation);
            newRotation = parentToWorld.m_rotation.GetInverse() * newRotation;
        }
    }

    if (!Vector3::IsNearlyEqual(newLocation, GetRelativeLocation()) || !Quaternion::IsNearlySame(newRotation, GetRelativeRotation()))
    {
        m_relativeTransform.m_translation   = newLocation;
        m_relativeTransform.m_rotation      = newRotation;
        
        UpdateLocalToWorld();

        return true;
    }

    return false;
}

#endif // !__SCENE_COMPONENT_INL__