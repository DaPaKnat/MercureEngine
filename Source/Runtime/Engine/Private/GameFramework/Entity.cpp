#include "PCH.hpp"

#include "GameFramework/Entity.hpp"
#include "Components/PrimitiveComponent.hpp"

// =================================[Public Constructor + Destructor]================================= //

/**
 * Default Constructor.
 */
Entity::Entity  ()
:	m_canEverTick			{ true },
	m_rootComponent			{ nullptr },
	m_ownedComponents		{ std::vector<EntityComponent*>() },
	m_hasBegunPlay			{ false },
    m_enableCollision       { false },
    m_world                 { GEngine->GetWorld() }
{}

// =================================[Public Local Methods]================================= //

bool    Entity::AddComponent                (EntityComponent*               p_component)
{
    if (p_component == nullptr || p_component->GetOwner() == this)
        return false;

    p_component->SetOwner(this);

    if (std::find(m_ownedComponents.begin(), m_ownedComponents.end(), p_component) == m_ownedComponents.end())
    {
        m_ownedComponents.push_back(p_component);

        return true;
    }
    else
        return false;
}

/**
 * Adds a delta to the location of this Entity in its local reference.
 *
 * @param p_deltaLocation   The change in location in local space.
 *
 */
void    Entity::AddLocalOffset              (Vector3 const&                 p_deltaLocation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddLocalOffset(p_deltaLocation, p_sweep, p_outHitResult);
    }
}

/**
 * Adds a delta to the rotation of this Entity in its local reference.
 *
 * @param p_deltaLocation   The change in rotation in local space.
 *
 */
void    Entity::AddLocalRotation            (Quaternion const&              p_deltaRotation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddLocalRotation(p_deltaRotation, p_sweep, p_outHitResult);
    }
}

/**
 * Adds a delta to the transform of this Entity in its local reference.
 *
 * @param p_deltaLocation   The change in transform in local space.
 *
 */
void    Entity::AddLocalTransform           (Transform const&               p_deltaTransform,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddLocalTransform(p_deltaTransform, p_sweep, p_outHitResult);
    }
}

/**
 * Adds a delta to the location of this Entity in in world space.
 *
 * @param p_deltaLocation   The change in location.
 *
 */
void    Entity::AddWorldOffset              (Vector3 const&                 p_deltaLocation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddWorldOffset(p_deltaLocation, p_sweep, p_outHitResult);
    }
}

/**
 * Adds a delta to the rotation of this Entity in in world space.
 *
 * @param p_deltaLocation   The change in rotation.
 *
 */
void    Entity::AddWorldRotation            (Quaternion const&              p_deltaRotation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddWorldRotation(p_deltaRotation, p_sweep, p_outHitResult);
    }
}

/**
 * Adds a delta to the transform of this Entity in in world space.
 * The scale is unchanged.
 *
 * @param p_deltaLocation   The change in transform.
 *
 */
void    Entity::AddWorldTransform           (Transform const&               p_deltaTransform,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->AddWorldTransform(p_deltaTransform, p_sweep, p_outHitResult);
    }
}

World*  Entity::GetWorld                    ()  const
{
    return m_world;
}

void    Entity::Deserialize                 (Json const&                    p_node,
                                             Json const&                    p_components)
{
    if (!p_node.contains("OwnedComponents"))
        return;

    for (auto const& node : p_node["OwnedComponents"])
    {
        EntityComponent* component = static_cast<EntityComponent*>(ObjectManager::Get().GetObjectOfType(node["TypeID"].get<size_t>(), node["ID"].get<size_t>()));

        if (component != nullptr)
        {
            component->Deserialize(p_components[std::to_string(component->GetInstanceType())][std::to_string(component->GetInstanceID())], p_components);
            component->SetOwner(this);
        }
    }

    if (p_node.contains("RootComponent"))
    {
        auto node = p_node["RootComponent"];

        m_rootComponent = static_cast<SceneComponent*>(ObjectManager::Get().GetObjectOfType(node["TypeID"].get<size_t>(), node["ID"].get<size_t>()));
    }

    PreInitialize();
    Initialize();
    PostInitialize();
}

void    Entity::DispatchBlockingHit         (PrimitiveComponent* const      p_myComponent,
                                             PrimitiveComponent* const      p_otherComponent,
                                             bool const                     p_selfMoved,
                                             HitResult const&               p_hitResult)
{
    HitResult result    {p_hitResult};

    if (p_selfMoved)
    {
        result.m_normal         = -result.m_normal;
        result.m_impactNormal   = -result.m_impactNormal;
    }

    InternalDispatchBlockingHit(p_myComponent, p_otherComponent, p_selfMoved, result);
}

void    Entity::NotifyHit                   (PrimitiveComponent const*      p_myComponent,
                                             Entity const*                  p_otherEntity,
                                             PrimitiveComponent const*      p_otherComponent,
                                             bool const                     p_selfMoved,
                                             Vector3 const&                 p_hitLocation,
                                             Vector3 const&                 p_hitNormal,
                                             Vector3 const&                 p_normalImpulse,
                                             HitResult const&               p_hitResult)
{
}

/**
 * Checks if the root component of this Entity is Valid.
 *
 * @return True, if the the root component is valid ( != nullptr), otherwise false.
 */
bool    Entity::HasValidRootComponent       ()  const
{
    return (m_rootComponent != nullptr);
}

/**
 * Checks if the Entity component given is owner by this Entity.
 *
 * @param p_entityComponent The entity component to checks.
 *
 * @return  True if this p_entityComponent is owned by this, otherwise false.
 */
bool    Entity::IsOwning                    (EntityComponent*               p_entityComponent)   const
{
    return std::find(m_ownedComponents.begin(), m_ownedComponents.end(), p_entityComponent) != m_ownedComponents.end();
}

bool    Entity::IsOverlapping               (Entity const*                  p_other)    const
{
    for (EntityComponent* ownedComponent : m_ownedComponents)
    {
        if (PrimitiveComponent* primitivecomp   {(PrimitiveComponent*)ownedComponent})
        {
            if (primitivecomp->GetOverlapInfos().size() > 0 && primitivecomp->IsOverlapping(p_other))
            {
                return true;
            }
        }
    }

    return false;
}

void    Entity::NotifyBeginOverlap          (Entity const*                  p_otherEntity)
{
}

void    Entity::NotifyEndOverlap            (Entity const*                  p_otherEntity)
{
}


void    Entity::Serialize                   (Json&                          p_save) const
{
    Json output;

    output["m_instanceID"] = GetInstanceID();
    output["m_instanceType"] = GetInstanceType();
    output["m_instanceName"] = GetName();

    for (auto component : m_ownedComponents)
    {
        if (component != nullptr)
        {
            Json comp;

            comp["TypeID"] = component->GetInstanceType();
            comp["ID"] = component->GetInstanceID();

            if (output["OwnedComponents"].find(comp) == output["OwnedComponents"].end())
                output["OwnedComponents"] += comp;
        }
    }

    if (m_rootComponent != nullptr)
    {
        Json root;

        root["TypeID"] = m_rootComponent->GetInstanceType();
        root["ID"] = m_rootComponent->GetInstanceID();

        output["RootComponent"] = root;
    }

    p_save += output;
}

void    Entity::SetEnableCollision          (bool const                     p_newEnableCollision)
{
    if (m_enableCollision != p_newEnableCollision)
    {
        m_enableCollision = p_newEnableCollision;

        std::vector<EntityComponent*> const& components {GetComponents()};

        for (EntityComponent* comp : components)
        {
            if (comp)
            {
                comp->OnEntityEnableCollisionChanged();
            }
        }
    }
}

/**
 * Set the Entity's root component to the specified relative location.
 *
 * @param p_newRelativeLocation New relative Location of the Entity's root component.
 */
void    Entity::SetRelativeLocation         (Vector3 const&                 p_newRelativeLocation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetRelativeLocation(p_newRelativeLocation, p_sweep, p_outHitResult);
    }
}

/**
 * Set the Entity's root component to the specified relative rotation.
 *
 * @param p_newRelativeRotation New relative Rotation of the Entity's root component.
 */
void    Entity::SetRelativeRotation         (Quaternion const&              p_newRelativeRotation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetRelativeRotation(p_newRelativeRotation, p_sweep, p_outHitResult);
    }
}

/**
 * Set the Entity's root component to the specified relative scale.
 *
 * @param p_newRelativeScale    New relative scale of the Entity's root component.
 */
void    Entity::SetRelativeScale            (Vector3 const&                 p_newRelativeScale)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetRelativeScale(p_newRelativeScale);
    }
}

/**
 * Set the Entity's root component to the specified relative transform.
 *
 * @param p_newRelativeScale    New relative scale of the Entity's root component.
 */
void    Entity::SetRelativeTransform        (Transform const&               p_newRelativeTransform,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetRelativeTransform(p_newRelativeTransform, p_sweep, p_outHitResult);
    }
}

/**
 * Set root component to be the specifield component.
 *
 * @param p_newRootComponent    The new root component. its owner should be this Entity.
 *
 * @return True if successful, false otherwise.
 */
bool    Entity::SetRootComponent            (SceneComponent*                p_newRootComponent)
{
    if (p_newRootComponent == nullptr || p_newRootComponent->GetOwner() == this)
    {
        m_rootComponent = p_newRootComponent;
        
        return true;
    }
    
    return false;
}

/**
 * Moves the Entity to the specifie location (In World Space).
 *
 * @param p_newLocation The new location to move the Entity to.
 */
void    Entity::SetWorldLocation            (Vector3 const&                 p_newLocation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetWorldLocation(p_newLocation, p_sweep, p_outHitResult);
    }
}

/**
 * Moves the Entity to the specifie location and Rotation (In World Space).
 *
 * @param p_newLocation The new location to move the Entity to.
 * @param p_newRotation The new rotation for the Entity.
 */
void    Entity::SetWorldLocationAndRotation (Vector3 const&                 p_newLocation,
                                             Quaternion const&              p_newRotation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetWorldLocationAndRotation(p_newLocation, p_newRotation, p_sweep, p_outHitResult);
    }
}

/**
 * Set Entity's rotation to the specified rotation (In World Space).
 *
 * @param p_newRotation The new rotation for the Entity.
 */
void    Entity::SetWorldRotation            (Quaternion const&              p_newRotation,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetWorldRotation(p_newRotation, p_sweep, p_outHitResult);
    }
}

/**
* Set Entity's scale to the specified scale (In World Space).
*
* @param p_newScale The new scale for the Entity.
*/
void    Entity::SetWorldScale               (Vector3 const&                 p_newScale)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetWorldScale(p_newScale);
    }
}

void    Entity::SetWorldTransform           (Transform const&               p_newTransform,
                                             bool const                     p_sweep /** = false */, 
                                             HitResult*                     p_outHitResult /** = nullptr */)
{
    if (m_rootComponent)
    {
        m_rootComponent->SetWorldTransform(p_newTransform, p_sweep, p_outHitResult);
    }
}

void    Entity::SetWorld                    (World*                         p_world)
{
    m_world = p_world;
}

void    Entity::UpdateComponentTransforms   ()
{
    for (EntityComponent* entityComponent : GetComponents())
    {
        if (entityComponent)
        {
            entityComponent->UpdateLocalToWorld();
        }
    }
}

// =================================[Private Local Methods]================================= //

void    Entity::InternalDispatchBlockingHit (PrimitiveComponent* const      p_myComponent,
                                             PrimitiveComponent* const      p_otherComponent,
                                             bool const                     p_selfMoved,
                                             HitResult const&               p_hitResult)
{
    if (p_otherComponent)
    {
        if (!IsPendingKill())
        {
            Entity* otherEntity {p_otherComponent->GetOwner()};
            
            NotifyHit(p_myComponent, otherEntity, p_otherComponent, p_selfMoved, p_hitResult.m_impactLocation, p_hitResult.m_impactNormal, Vector3::Zero, p_hitResult);
        }
    }
}