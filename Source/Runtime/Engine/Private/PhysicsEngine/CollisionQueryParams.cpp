#include "PCH.hpp"

#include "PhysicsEngine/CollisionQueryParams.hpp"

// ==============================[Public Local Constructor + Destructor]============================== //

/**
 * Default Constructor.
 */
CollisionQueryParams::CollisionQueryParams  ()
:   m_findInitialOverlaps       {false},
    m_ignoreBlocks              {false},
    m_ignoreTouchs              {false},
    m_queryMobilityType         {EQueryMobilityType::Any},
    m_returnFaceIndex           {false},
    m_returnPhysicalMaterial    {false},
    m_traceComplex              {false}
 {}

// ==============================[Public Local Methods]============================== //

void                                    CollisionQueryParams::AddIgnoredComponent           (PrimitiveComponent*                        p_componentToIgnore)
{
    // Check if the given component is not already in the ignore component list.
    if (std::find(m_ignoredComponents.begin(), m_ignoredComponents.end(), p_componentToIgnore) == m_ignoredComponents.end())
    {
        m_ignoredComponents.push_back(p_componentToIgnore);
    }
}

void                                    CollisionQueryParams::AddIgnoredComponents          (std::vector<PrimitiveComponent*> const&    p_componentsToIgnore)
{
    for (PrimitiveComponent* comp : m_ignoredComponents)
    {
        AddIgnoredComponent(comp);
    }
}

void                                    CollisionQueryParams::AddIgnoredEntity              (Entity*                                    p_entityToIgnore)
{
    // Check if the given component is not already in the ignore component list.
    if (std::find(m_ignoredEntities.begin(), m_ignoredEntities.end(), p_entityToIgnore) == m_ignoredEntities.end())
    {
        m_ignoredEntities.push_back(p_entityToIgnore);
    }
}

void                                    CollisionQueryParams::AddIgnoredEntities            (std::vector<Entity*> const&                p_entitieToIgnore)
{
    for (Entity* entity : p_entitieToIgnore)
    {
        if (std::find(m_ignoredEntities.begin(), m_ignoredEntities.end(), entity) != m_ignoredEntities.end())
        {
            m_ignoredEntities.push_back(entity);
        }
    }
}



void                                    CollisionQueryParams::ClearIgnoredComponents        ()
{
    m_ignoredComponents.clear();
}

void                                    CollisionQueryParams::ClearIgnoredEntities          ()
{
    m_ignoredEntities.clear();
}

std::vector<PrimitiveComponent*> const& CollisionQueryParams::GetIngoredComponents          ()  const
{
    return m_ignoredComponents;
}

std::vector<Entity*> const&             CollisionQueryParams::GetIgnoredEntities            ()  const
{
    return m_ignoredEntities;
}

physx::PxQueryFlags                     CollisionQueryParams::GetPhysXQueryMobilityFlags    ()  const
{
    physx::PxQueryFlags queryMobilityFlag;

    switch (m_queryMobilityType)
    {
        case EQueryMobilityType::Any        : queryMobilityFlag = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC; break;
        case EQueryMobilityType::Dynamic    : queryMobilityFlag = physx::PxQueryFlag::eDYNAMIC; break;
        case EQueryMobilityType::Static     : queryMobilityFlag = physx::PxQueryFlag::eSTATIC; break;
        default                             : queryMobilityFlag = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC; break;
    }

    return queryMobilityFlag;
}