#include "PCH.hpp"

//#include "Components/SceneComponent.hpp"

#include "GameFramework/Entity.hpp"

// =================================[Public Constructor + Destructor]================================= //

/**
 * Default Constructor.
 */
SceneComponent::SceneComponent  ()
:   Super()
{
    m_mobility                  = EComponentMobility::Movable;
    m_relativeTransform.m_scale = Vector3::One;
    m_isVisibleInGame           = true;
    m_attachParent              = nullptr;
    m_wantsOnUpdateTransform    = false;
}


// =================================[Virtual Public Local Methods]================================= //

/**
 * Check if it's possible to attach another scene component as a child
 *
 * @param p_child   The child we want to test if we can attach it.
 *
 * @return True if it's possible to attach p_child as child, otherwise false
 */
bool                                SceneComponent::CanAttachAsChild                (SceneComponent const*          p_child)    const
{
    return true;
}

/**
 * Gets the form of collision of this component.
 *
 * @return The form of collision of this component.
 */
ECollisionEnabled                   SceneComponent::GetCollisionEnabled             ()  const
{
    return ECollisionEnabled::NoCollision;
}

/**
 * Get the collision channel used by this object when it moves.
 *
 * @return  The collision channel of hthis object.
 */
ECollisionChannel                   SceneComponent::GetCollisionObjectType          ()  const
{
    return ECollisionChannel::WorldDynamic;
}

/**
 * Gets the response type given a specific channel.
 *
 * @param p_channel The channel whose response type we want to know.
 *
 * @return The collision response associated to the channel given.
 */
ECollisionResponse                  SceneComponent::GetCollisionResponseToChannel   (ECollisionChannel const        p_channel)  const
{
    return ECollisionResponse::Ignore;
}

/** 
 * Gets a reference constant to CollisionResponseContainer of this component.
 *
 * @return A reference constant to CollisionResponseContainer of this component.
 */
CollisionResponseContainer const&   SceneComponent::GetCollisionResponseToChannels  ()  const
{
    return CollisionResponseContainer::GetDefaultResponseContainer();
}

/**
 * Returns whether this component is currently using physics simulation.
 *
 * @return  True if this component is using the physics simulation, otherwise false.
 */
bool                                SceneComponent::IsSimulatingPhysics             ()  const
{
    return false;
}

/**
 * Called when AttachParent changes.
 */
void                                SceneComponent::OnAttachementChanged            ()
{}

/**
 * Called after a child scene component is attached from this
 *
 * @param p_child   The child scene component that we attach to this.
 */
void                                SceneComponent::OnChildAttached                 (SceneComponent*                p_child)
{}

/**
* Called after a child scene component is detached from this
*
* @param p_child   The child scene component that we attach to this.
*/
void                                SceneComponent::OnChildDetached                 (SceneComponent*                p_child)
{}

void                                SceneComponent::OnUpdateTransform               (EUpdateTransformFlags const    p_updateTransformFlags,
                                                                                     ETeleportType const            p_teleportType /** = ETeleportType::None */)
{
}

/**
 * Set the movement type of this component.
 *
 * @paran p_newMobility The new mobility type.
 */
void                                SceneComponent::SetMobility                     (EComponentMobility const       p_newMobility)
{
    if (p_newMobility != m_mobility)
    {
        m_mobility = p_newMobility;
        if (m_mobility == EComponentMobility::Movable)
        {
            for (SceneComponent* child : GetAttachChildrens())
            {
                if (child != nullptr)
                {
                    child->SetMobility(m_mobility);
                }
            }
        }
    }
}

/**
 * Update the localToWorld Transform.
 */
void                                SceneComponent::UpdateLocalToWorld              (EUpdateTransformFlags const    p_updateTransformFlags /** = EUpdateTransformFlags::None */, 
                                                                                     ETeleportType const            p_teleportType /**= ETeleportType::None */)
{
    UpdateLocalToWorldWithParent(GetAttachParent(), p_updateTransformFlags, p_teleportType);
}

// =================================[Public Local Methods]================================= //

/**
 * Adds a delta to the location of the component in its local reference.
 *
 * @param p_deltaLocalLocation  Change in location of the component in its local reference.
 */
void                                SceneComponent::AddLocalOffset                  (Vector3 const&                     p_deltaLocalLocation,
                                                                                     bool const                         p_sweep, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr*/)
{
    Vector3 const localOffset   {GetRelativeRotation().RotateVector(p_deltaLocalLocation)};
   
   SetRelativeLocation(GetRelativeLocation() + localOffset, p_sweep, p_outHitResult);
}

/**
 * Adds a delta to the rotation of the component in its local reference.
 *
 * @param p_deltaLocalRotation  Change in rotation of the component in its local reference.
 */
void                                SceneComponent::AddLocalRotation                (Quaternion const&                  p_deltaLocalRotation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetRelativeRotation(GetRelativeRotation() * p_deltaLocalRotation, p_sweep, p_outHitResult);
}

/**
 * Adds a delta to the transform of the component in its local reference.
 *
 * @param p_deltaLocalTransform  Change in transform of the component in its local reference.
 */
void                                SceneComponent::AddLocalTransform               (Transform const&                   p_deltaLocalTransform,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetRelativeTransform(GetRelativeTransform() * p_deltaLocalTransform, p_sweep, p_outHitResult);
}

/**
 * Adds a dela to the location of the component relative to its parent.
 *
 * @param p_deltaRelativeLocation  Change in location of the component relative to its parent.
 */
void                                SceneComponent::AddRelativeOffset               (Vector3 const&                     p_deltaRelativeLocation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetRelativeLocation(GetRelativeLocation() + p_deltaRelativeLocation, p_sweep, p_outHitResult);
}

/**
 * Adds a dela to the rotation of the component relative to its parent.
 *
 * @param p_deltaRelativeRotation   Change in rotation of the component relative to its parent.
 */
void                                SceneComponent::AddRelativeRotation             (Quaternion const&                  p_deltaRelativeRotation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetRelativeRotation(p_deltaRelativeRotation * GetRelativeRotation(), p_sweep, p_outHitResult);
}

/**
 * Adds a delta to the location of the component in world space.
 *
 * @param p_deltaWorldLocation  Change in location in world space for the component.
 */
void                                SceneComponent::AddWorldOffset                  (Vector3 const&                     p_deltaWorldLocation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetWorldLocation(GetWorldLocation() + p_deltaWorldLocation, p_sweep, p_outHitResult);
}

/**
* Adds a delta to the rotation of the component in world space.
*
* @param p_deltaWorldLocation  Change in location in world space for the component.
*/
void                                SceneComponent::AddWorldRotation                (Quaternion const&                  p_deltaWorldRotation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetWorldRotation(p_deltaWorldRotation * GetWorldRotation(), p_sweep, p_outHitResult);
}

/**
 * Adds a delta to the transform of the component in world space. The scale is unchange.
 *
 * @param p_deltaWorldTransform Change in location in world space for the component.
 */
void                                SceneComponent::AddWorldTransform               (Transform const&                   p_deltaTransform,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    Quaternion const    newWorldRotation    {p_deltaTransform.m_rotation * GetWorldTransform().m_rotation};
    Vector3 const       newWorldPosition    {p_deltaTransform.m_translation + GetWorldTransform().m_translation};

    SetWorldTransform(Transform(newWorldPosition, newWorldRotation), p_sweep, p_outHitResult);
}

/**
 * Attach this component to another scene component.
 *
 * @param p_parent  The parent where we want to attaches this.
 * @param p_rule    How to handle transform modification when attaching
 *
 * @return True if attachement is successful (or already attached), otherwise false.
 */
bool                                SceneComponent::AttachTo                        (SceneComponent*                    p_parent,
                                                                                     AttachmentTransformRules const&    p_attachmentRules)
{
    if (p_parent)
    {
        // Check if this is already attached to this p_parent
        if (p_parent == GetAttachParent()  && std::find(p_parent->m_attachChildrens.begin(), p_parent->m_attachChildrens.end(), this) != p_parent->m_attachChildrens.end())
        {
            return true;
        }

        // Check if p_parent is not this.
        if (p_parent == this)
        {
            /// PUT DEBUG ///
            return false;
        }

        Entity* myEntity      {GetOwner()};
        Entity* parentEntity  {p_parent->GetOwner()};

        // Check if this is not the root component
        if (myEntity == parentEntity && myEntity && myEntity->GetRootComponent() == this)
        {
            return false;
        }

        // Check if p_parent is not alreay attached to this.
        if (p_parent->IsAttachedTo(this))
        {
            /// PUT DEBUG (AttachTo Itself) ///
            return false;
        }

        // Check if p_parent allow at this to attach on it.
        if (!p_parent->CanAttachAsChild(this))
        {
            return false;
        }

        /// Check if this and p_parent is non-static

        // Find if we're already attached to this parent, it that case, we save its position in the array
        std::vector<SceneComponent*>::iterator  attachIt        {std::find(p_parent->m_attachChildrens.begin(), p_parent->m_attachChildrens.end(), this)};
        bool                                    hasBeenDetach   {false};

        // Check if the iterator is valid
        if (attachIt != p_parent->m_attachChildrens.end())
        {
            // Check if p_attachmentRule is no set a Keep Relative, if that case, we can detach this. otherwise it's not needed.
            if (p_attachmentRules != AttachmentTransformRules::KeepRelative)
            {
                DetachmentTransformRules const detachmentRules
                {
                    p_attachmentRules.m_locationRule == EAttachmentRule::KeepRelative ? EDetachmentRule::KeepRelative : EDetachmentRule::KeepRelative,
                    p_attachmentRules.m_rotationRule == EAttachmentRule::KeepRelative ? EDetachmentRule::KeepRelative : EDetachmentRule::KeepRelative,
                    p_attachmentRules.m_scaleRule == EAttachmentRule::KeepRelative ? EDetachmentRule::KeepRelative : EDetachmentRule::KeepRelative
                };

                Detach(detachmentRules);
                hasBeenDetach = true;
            }
        }



        m_attachParent = p_parent;

        OnAttachementChanged();

        // Add tis component into the parent's child array
        if (attachIt != p_parent->m_attachChildrens.end() && hasBeenDetach)
        {
            p_parent->m_attachChildrens.insert(attachIt,this);
        }
        else
        {
            p_parent->m_attachChildrens.push_back(this);
        }

        // Update the transform of the child with the attachment Rules

        Transform const relativeTransform   {/*Transform::GetRelativeTransform(GetAttachParent()->GetWorldTransform(),GetWorldTransform())*/};

        switch (p_attachmentRules.m_locationRule)
        {
            case EAttachmentRule::KeepRelative: 
                /// Do nothing
                break;

            case EAttachmentRule::KeepWorld: 
                m_relativeTransform.m_translation = relativeTransform.m_translation; 
                break;
                
            case EAttachmentRule::SnapToTarget: 
                m_relativeTransform.m_translation = Vector3::Zero; 
                break;
        }

        switch (p_attachmentRules.m_rotationRule)
        {
            case EAttachmentRule::KeepRelative: 
                /// Do nothing
                break;

            case EAttachmentRule::KeepWorld: 
                m_relativeTransform.m_rotation = relativeTransform.m_rotation; 
                break;
                
            case EAttachmentRule::SnapToTarget: 
                m_relativeTransform.m_rotation = Quaternion::Identity; 
                break;
        }

        switch (p_attachmentRules.m_scaleRule)
        {
            case EAttachmentRule::KeepRelative: 
                /// Do nothing
                break;

            case EAttachmentRule::KeepWorld: 
                m_relativeTransform.m_scale = relativeTransform.m_scale; 
                break;
                
            case EAttachmentRule::SnapToTarget: 
                m_relativeTransform.m_scale = Vector3::One; 
                break;      
        }

        GetAttachParent()->OnChildAttached(this);

        UpdateLocalToWorld();

        return true;
    }

    return false;
}

/**
 * Detach this component fronwhatever it is attached to.
 *
 * @param p_detachmentRules How to handle transform modification when detaching.
 */
void                                SceneComponent::Detach                          (DetachmentTransformRules const&    p_detachmentRules)
{
    // Check if this has a parent
    if (GetAttachParent())
    {
        std::vector<SceneComponent*>::iterator childIt {std::find(GetAttachParent()->m_attachChildrens.begin(), GetAttachParent()->m_attachChildrens.end(), this)};

        if (childIt != GetAttachParent()->m_attachChildrens.end())
        {
            GetAttachParent()->m_attachChildrens.erase(childIt);
            GetAttachParent()->OnChildDetached(this);

            m_attachParent = nullptr;
            OnAttachementChanged();

            // Apply DetachmemtRules
            switch (p_detachmentRules.m_locationRule)
            {
                case EDetachmentRule::KeepRelative: 
                    // Do nothing
                    break;

                case EDetachmentRule::KeepWorld: 
                    m_relativeTransform.m_translation = GetWorldLocation(); 
                    break;
            }

            switch (p_detachmentRules.m_rotationRule)
            {
                case EDetachmentRule::KeepRelative: 
                    // Do nothing
                    break;

                case EDetachmentRule::KeepWorld: 
                    m_relativeTransform.m_rotation = GetWorldRotation(); 
                    break;
            }

            switch (p_detachmentRules.m_scaleRule)
            {
                case EDetachmentRule::KeepRelative: 
                    // Do nothing
                    break;

                case EDetachmentRule::KeepWorld: 
                    m_relativeTransform.m_scale = GetWorldScale(); 
                    break;
            }

            UpdateLocalToWorld();
        }
    }
}

/**
 * Get the array of scene component attached at this component.
 *
 * @return The array of scene component attached at this component.
 */
std::vector<SceneComponent*> const& SceneComponent::GetAttachChildrens              ()  const
{
    return m_attachChildrens;
}

/**
 * Walks up the attachment chain from this SceneComponent and gets the SceneComponent at the top.
 *  
 * @return The SceneCompoent at the top of the attachment chain of this. If AttachParent is nullptr, return this.
 */
SceneComponent*                     SceneComponent::GetAttachRoot                   ()  const
{
    SceneComponent const* top {this};
    
    while (top && top->GetAttachParent())
        top = top->GetAttachParent();
    
    return const_cast<SceneComponent*>(top);
}

/** 
 * Walks up the attachment chain from this SceneComponent and returns the top-level actor it's attached to.
 */
Entity*                             SceneComponent::GetAttachRootEntity             ()
{
    SceneComponent const* const    attachRootReworkSceneComponent    {GetAttachRoot()};
    return (attachRootReworkSceneComponent ? attachRootReworkSceneComponent->GetOwner() : nullptr);
}

/**
 * Get alls attached child components.
 *
 * @param p_includeAllDesceandants  If we include all descendant in the list.
 * @parem p_childrens   The array of children components
 */
void                                SceneComponent::GetChildrens                    (bool const                         p_includeAllDescendants,
                                                                                     std::vector<SceneComponent*>&      p_childrens)
{
    p_childrens.clear();

    if (p_includeAllDescendants)
    {
        GetChildrenWithDescendant(p_childrens);
    }

    else
    {
        p_childrens.insert(p_childrens.end(), m_attachChildrens.begin(), m_attachChildrens.end());
    }
}

ECollisionResponse                  SceneComponent::GetCollisionResponseToComponent (SceneComponent const*              p_otherComponent)   const
{
    if (p_otherComponent == nullptr || GetCollisionEnabled() == ECollisionEnabled::NoCollision || p_otherComponent->GetCollisionEnabled() == ECollisionEnabled::NoCollision)
    {
        return ECollisionResponse::Ignore;
    }

    
    return Math::Min
    (
        GetCollisionResponseToChannel(p_otherComponent->GetCollisionObjectType()),
        p_otherComponent->GetCollisionResponseToChannel(GetCollisionObjectType())
    );

}

/** 
 * Gets all parent components up and to including the root component.
 * (Order: parent to root component)
 *
 * @param p_parents The array of parents CceneComponent.
 */
void                                SceneComponent::GetParents                      (std::vector<SceneComponent*>&      p_parents)  const
{
    p_parents.clear();

    SceneComponent* parentIterator   {GetAttachParent()};

    while (parentIterator != nullptr)
    {
        p_parents.push_back(parentIterator);
        parentIterator = parentIterator->GetAttachParent();
    }
}

/**
 * Walks up the attachment chain to see if this component is attached to the supplied component.
 *
 * @param p_testComponent   The component to check with.
 *
 * @return True if this is attach to p_testComponent in the attachement chain, otherwise false.
 */
bool                                SceneComponent::IsAttachedTo                    (SceneComponent const*              p_testComponent)    const
{
    if (p_testComponent)
    {
        
        SceneComponent* comp    {GetAttachParent()};
        while (comp != nullptr)
        {
            if (p_testComponent == comp)
                return true;
            
            comp = comp->GetAttachParent();
        }
    }

    return false;
}

/** 
 * Reset the transform of the component relative to its parent. 
 * Sets relative location to zero, relative rotation to no rotation, and Scale to 1. 
 */
void                                SceneComponent::ResetRelativeTransform          ()
{
    SetRelativeLocationAndRotation(Vector3::Zero, Quaternion::Identity);
    SetRelativeScale(Vector3::One);
}

/**
 * Set the scale of the component relative to its parent.
 *
 * @param p_newRelativeScale    The new scale of the component relative to its parent.
 */
void                                SceneComponent::SetRelativeScale                (Vector3 const&                     p_newRelativeScale)
{
    if (p_newRelativeScale != GetRelativeScale())
    {
        m_relativeTransform.m_scale = p_newRelativeScale;
        UpdateLocalToWorld();
    }
}

/**
 * Set the transform of the component relative to its parent.
 *
 * @param p_newRelativeTransform    The new transform of the component relative to its parent.
 */
void                                SceneComponent::SetRelativeTransform            (Transform const&                   p_newRelativeTransform,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    SetRelativeLocationAndRotation(p_newRelativeTransform.m_translation, p_newRelativeTransform.m_rotation, p_sweep, p_outHitResult);
    SetRelativeScale(p_newRelativeTransform.m_scale);
}

void                                SceneComponent::SetupAttachment                 (SceneComponent*                    p_parent)
{
    if (p_parent != this)
    {
        if (p_parent == nullptr || !p_parent->IsAttachedTo(this))
        {
            if (m_attachParent == nullptr || std::find(m_attachParent->m_attachChildrens.begin(), m_attachParent->m_attachChildrens.end(), this) == m_attachParent->m_attachChildrens.end())
            {
                m_attachParent = p_parent;
                m_attachParent->m_attachChildrens.push_back(this);
            }
        }
    }
}

/**
 * Set the visibility of the component.
 *
 * @param p_newVisibility       The new visibility state.
 * @param p_propagateToChildren Should we propagate the new visibility state to all childrens.
 */
void                                SceneComponent::SetVisibilityInGame             (bool const                         p_newVisibility,
                                                                                     bool const                         p_propagateToChildren)
{
    if (p_newVisibility != m_isVisibleInGame)
    {
        m_isVisibleInGame = p_newVisibility;
        OnVisibilityInGameChanged();
    }

    std::vector<SceneComponent*> const& childrens   {GetAttachChildrens()};

    if (p_propagateToChildren && m_attachChildrens.size() > 0)
    {
        for (SceneComponent* child : childrens)
        {
            child->SetVisibilityInGame(p_newVisibility, true);
        }
    }
}

/**
 * Put this component at the specified location in world space. Updates relative location to achieve the final world location.
 *
 * @param p_newWorldLocation    The new location in world space for the component.
 */
void                                SceneComponent::SetWorldLocation                (Vector3 const&                     p_newWorldLocation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    Vector3 newRelativeLocation {p_newWorldLocation};

    if (GetAttachParent())
    {
        newRelativeLocation = GetWorldTransform().InverseTransformPoint(p_newWorldLocation);
    }
    
    SetRelativeLocation(newRelativeLocation, p_sweep, p_outHitResult);
}

/**
 * Set the relative location and rotation of the component to put it at the supplied pose in world space.
 *
 * @param p_newWorldLocation    The new location in world space for the component.
 * @param p_newWorldRotation    The new rotation in world space for the component.
 */
void                                SceneComponent::SetWorldLocationAndRotation     (Vector3 const&                     p_newWorldLocation,
                                                                                     Quaternion const&                  p_newWorldRotation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    Vector3     newRelativeLocation {p_newWorldLocation};
    Quaternion  newRelativeRotation {p_newWorldRotation};

    if (GetAttachParent())
    {
        Transform const parentToWorld   {GetAttachParent()->GetWorldTransform()};

        newRelativeLocation = parentToWorld.InverseTransformPoint(p_newWorldLocation);
        newRelativeRotation = parentToWorld.m_rotation.GetInverse() * p_newWorldRotation;
    }

    SetRelativeLocationAndRotation(newRelativeLocation, newRelativeRotation, p_sweep, p_outHitResult);
}

/**
 * Put this component at the specified rotation in world space. Updates relative rotation to achieve the final world rotation.
 *
 * @param p_newWorldRotation    New rotation in world space for the component.
 */
void                                SceneComponent::SetWorldRotation                (Quaternion const&                  p_newWorldRotation,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    Quaternion newRelativeRotation  {p_newWorldRotation};


    if (GetAttachParent())
    {
        newRelativeRotation = GetWorldTransform().m_rotation.GetInverse() * p_newWorldRotation;
    }

    SetRelativeRotation(newRelativeRotation, p_sweep, p_outHitResult);
}

/**
 * Set the relative scale of the component to put it at the supplied scale in world space.
 *
 * @param p_newWorldScale   New scale in world space for this component.
 */
void                                SceneComponent::SetWorldScale                   (Vector3 const&                     p_newWorldScale)
{
    Vector3 newRelativeScale    {p_newWorldScale};

    if (GetAttachParent())
    {
        newRelativeScale = p_newWorldScale * Vector3::SafeScaleReciprocal(GetWorldTransform().m_scale);
    }

    SetRelativeScale(newRelativeScale);
}

/**
 * Set the transform of the component in world space.
 *
 * @param p_newWorldScale   New transform in world space for this component.
 */
void                                SceneComponent::SetWorldTransform               (Transform const&                   p_newWorldTransform,
                                                                                     bool const                         p_sweep /** = false */, 
                                                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    if (GetAttachParent() != nullptr)
    {
        Transform const parentToWorld       {GetAttachParent()->GetWorldTransform()};
        Transform       relativeTransform   {Transform::GetRelative(parentToWorld, GetWorldTransform())};

        SetRelativeTransform(relativeTransform, p_sweep, p_outHitResult);
    }

    else
    {
        SetRelativeTransform(p_newWorldTransform, p_sweep, p_outHitResult);
    }
}

/**
 * Update all childs scene components of this component.
 * Called after updated local to world transform of this.
 */
void                                SceneComponent::UpdateChildTransforms           (EUpdateTransformFlags const        p_updateTransformFlags /** = EUpdateTransformFlags::None */, 
                                                                                     ETeleportType                      p_teleportType /** = ETeleportType::None */)
{
    if (GetChildrenCount() > 0)
    {
        for (SceneComponent* const child : m_attachChildrens)
        {
            if (child)
            {
                child->UpdateLocalToWorld();
            }
        }
    }

    if (GetChildrenCount()  > 0)
    {
        for (SceneComponent* const child : m_attachChildrens)
        {
            if (child != nullptr)
            {
                child->UpdateLocalToWorld(p_updateTransformFlags, p_teleportType);
            }
        }
    }
}

void                                SceneComponent::UpdateOverlaps                  (std::vector<OverlapInfo> const*    p_pendingOverlaps /** = nullptr */,
                                                                                     bool const                         p_doNotifies /** = true */,
                                                                                     std::vector<OverlapInfo> const*    p_overlapAtEndLocation /** = nullptr */)
{
    UpdateOverlapsImpl(p_pendingOverlaps, p_doNotifies, p_overlapAtEndLocation);
}

// =================================[Virtual Protected Local Methods]================================= //

bool    SceneComponent::MoveComponentImpl           (Vector3 const&                     p_deltaWorldOffset,
                                                     Quaternion const&                  p_newWorldRotation,
                                                     bool const                         p_sweep, 
                                                     HitResult*                         p_outHitResult /** = nullptr */)
{
    if (IsPendingKill())
    {

        if (p_outHitResult != nullptr)
        {
            *p_outHitResult = HitResult();
        }

        return false;
    }

    if (p_outHitResult != nullptr)
    {
        *p_outHitResult = HitResult();
        p_outHitResult->m_hitTime = 1.0f; 
    }

    // Do nothing, is the offset is zero or the rotation is null.
    if (p_deltaWorldOffset.IsZero() && Quaternion::IsNearlySame(p_newWorldRotation, GetWorldTransform().m_rotation))
    {
        return true;
    }

    bool const hasMoved {InternalSetWorldLocationAndRotation(GetWorldLocation() + p_deltaWorldOffset, p_newWorldRotation)};

    if (hasMoved)
    {
        UpdateOverlaps();
    }

    return true;
}

/**
 * Overridable internal method executed when p_isVisible has changed.
 */
void    SceneComponent::OnVisibilityInGameChanged   ()
{
}

void    SceneComponent::UpdateOverlapsImpl          (std::vector<OverlapInfo> const*    p_pendingOverlaps /** = nullptr */,
                                                     bool const                         p_doNotifies /** = true */,
                                                     std::vector<OverlapInfo> const*    p_overlapAtEndLocation /** = nullptr */)
{
   std::vector<SceneComponent*> const& attachedChildren {GetAttachChildrens()};

   for (SceneComponent* child : attachedChildren)
   {
       if (child)
       {
           child->UpdateOverlaps(nullptr, p_doNotifies);
       }
   }
}

void    SceneComponent::SerializeHierarchy          (Json&                              p_json) const
{
    Json data;

    for (auto comp : m_attachChildrens)
    {
        if (comp == nullptr)
            continue;

        data["TypeID"] = comp->GetInstanceType();
        data["ID"] = comp->GetInstanceID();

        p_json["Childrens"] += data;
    }

    if (m_attachParent)
    {
        data["TypeID"] = m_attachParent->GetInstanceType();
        data["ID"] = m_attachParent->GetInstanceID();

        p_json["Parent"] = data;
    }
}

// =================================[Private Local Methods]================================= //

/**
 * Appends all descendant (Recursivelu of this scene component to the list of children.
 * (the list is not clear)
 *
 * @param p_childrenWithDescendant  The array of children with descendants.
 */
void    SceneComponent::GetChildrenWithDescendant       (std::vector<SceneComponent*>&  p_childrenWithDescendant)
{
    p_childrenWithDescendant.insert(p_childrenWithDescendant.end(), m_attachChildrens.begin(), m_attachChildrens.end());

    for (SceneComponent* child : m_attachChildrens)
    {
        if (child)
        {
            child->GetChildrenWithDescendant(p_childrenWithDescendant);
        }
    }

}

void    SceneComponent::PropagateTransformUpdate        (bool const                     p_transformChanged,
                                                         EUpdateTransformFlags const    p_updateTransformFlags /** = EUpdateTransformFlags::None */,
                                                         ETeleportType const            p_teleportType /** = ETeleportType::None */)
{
    std::vector<SceneComponent*> const& attachChildren  {GetAttachChildrens()};
    
    if (p_transformChanged)
    {

        if (m_physicsStateCreated)
        {
            if (m_wantsOnUpdateTransform)
            {
                OnUpdateTransform(p_updateTransformFlags, p_teleportType);
            }
        }
 
        if (attachChildren.size() > 0)
        {
            EUpdateTransformFlags childrenFlagNoPhysics = ~EUpdateTransformFlags::SkipPhysicsUpdate & p_updateTransformFlags;
            UpdateChildTransforms(childrenFlagNoPhysics, p_teleportType);
        }
    }

    else
    {
        if (attachChildren.size() > 0)
        {
            UpdateChildTransforms();
        }
    }

}

void    SceneComponent::UpdateLocalToWorldWithParent    (SceneComponent*                m_attachParent,
                                                         EUpdateTransformFlags const    p_updateTransformFlags,
                                                         ETeleportType const            p_teleport /** = ETeleportType::None */)
{
    // If the parent hasn't been updated, we update this one.
    if (m_attachParent && !m_attachParent->m_isLocalToWorldUpdated)
    {
        m_attachParent->UpdateLocalToWorld();

        if (m_isLocalToWorldUpdated)
        {
            return;
        }
    }

    m_isLocalToWorldUpdated = true;
    Transform const newTransform {ComputeNewLocalToWorld(m_relativeTransform, m_attachParent)};

    bool hasChanged {!Transform::IsSame(m_localToWorld,newTransform)};


    if (hasChanged == true || p_teleport != ETeleportType::None)
    {
        m_localToWorld = newTransform;
        PropagateTransformUpdate(true, p_updateTransformFlags, p_teleport);
    }

    else
    {
        PropagateTransformUpdate(false);
    }
}