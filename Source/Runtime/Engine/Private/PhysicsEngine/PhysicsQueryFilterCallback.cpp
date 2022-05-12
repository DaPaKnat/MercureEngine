#include "PCH.hpp"

#include "PhysicsEngine/PhysicsQueryFilterCallback.hpp"
#include "PhysicsEngine/PhysicsEnums.hpp"

#include "GameFramework/Entity.hpp"
#include "Components/PrimitiveComponent.hpp"

// ==============================[Public Static Methods]============================== //

physx::PxQueryHitType::Enum PhysicsQueryFilterCallback::ComputeQueryHitType (physx::PxFilterData const& p_queryFilter,
                                                                             physx::PxFilterData const& p_shapeFilter,
                                                                             bool const                 p_preFilter /** = false */)
{
    ECollisionQueryType const   queryType           {static_cast<ECollisionQueryType>(p_queryFilter.word0)};
    ECollisionChannel const     queryChannel        {static_cast<ECollisionChannel>(p_queryFilter.word3)};

    ECollisionChannel const     shapeChannel        {static_cast<ECollisionChannel>(p_shapeFilter.word3)};
    int32 const                 shapeChannelBit     {1 << static_cast<int32>(shapeChannel)};

    physx::PxQueryHitType::Enum queryHitTypeResult  {physx::PxQueryHitType::eNONE};

    if (queryType == ECollisionQueryType::ObjectQuery)
    {
        int32 const multiTrace  {static_cast<int32>(queryChannel)};
        
        // Is this one of the object type that we eant
        if (shapeChannelBit & p_queryFilter.word1)
        {
            if (p_preFilter)
            {
                queryHitTypeResult = multiTrace ? physx::PxQueryHitType::eTOUCH : physx::PxQueryHitType::eBLOCK;
            }

            else
            {
                queryHitTypeResult = physx::PxQueryHitType::eBLOCK;
            }
        }
    }

    else // (queryType == ECollisionQueryType::TraceQuery)
    {
        physx::PxU32 const          queryBit        {1u << static_cast<physx::PxU32>(queryChannel)};
        physx::PxQueryHitType::Enum queryHitType    {physx::PxQueryHitType::eNONE};
        physx::PxQueryHitType::Enum shapeHitType    {physx::PxQueryHitType::eNONE};

        if ((queryBit & p_shapeFilter.word1) != 0)
        {
            queryHitType = physx::PxQueryHitType::eBLOCK;
        }

        else if ((queryBit & p_shapeFilter.word2) != 0)
        {
            queryHitType = physx::PxQueryHitType::eTOUCH;
        }

        if ((shapeChannelBit & p_queryFilter.word1) != 0)
        {
            shapeHitType = physx::PxQueryHitType::eBLOCK;
        }

        else if ((shapeChannelBit & p_queryFilter.word2) != 0)
        {
            shapeHitType = physx::PxQueryHitType::eTOUCH;
        }

        return Math::Min(queryHitType, shapeHitType);
    }

    return queryHitTypeResult;
}



// ==============================[Public Constructor]============================== //

PhysicsQueryFilterCallback::PhysicsQueryFilterCallback  (CollisionQueryParams const&    p_queryParams)
:   m_ignoreBlocks          {p_queryParams.m_ignoreBlocks},
    m_ignoreTouches         {p_queryParams.m_ignoreTouchs},
    m_isOverlapQuery        {false},
    m_ignoredComponents     {p_queryParams.GetIngoredComponents()},
    m_ignoredEntities       {p_queryParams.GetIgnoredEntities()},
    m_preFilterReturnValue  {physx::PxQueryHitType::eNONE}
{}

// ==============================[Public Local Methods]============================== //

physx::PxQueryHitType::Enum PhysicsQueryFilterCallback::postFilter  (physx::PxFilterData const& p_filterData,
                                                                     physx::PxQueryHit const&   p_queryHit)
{
    // Currently this function is not used
    return physx::PxQueryHitType::eBLOCK;
}


physx::PxQueryHitType::Enum PhysicsQueryFilterCallback::preFilter  (physx::PxFilterData const&  p_filterData,
                                                                    physx::PxShape const*       p_shape,
                                                                    physx::PxRigidActor const*  p_actor,
                                                                    physx::PxHitFlags&          p_outQueryFlags)
{
    m_preFilterReturnValue = physx::PxQueryHitType::eNONE;

    if (p_shape != nullptr)
    {
        physx::PxFilterData         shapeQueryFilter    {p_shape->getQueryFilterData()};

        physx::PxU32                shapeFlags          {shapeQueryFilter.word3};
        physx::PxU32                queryFlags          {p_filterData.word3};
        physx::PxU32                commonFlags         {shapeFlags & queryFlags};

        physx::PxQueryHitType::Enum resultQueryHitType  {ComputeQueryHitType(p_filterData, shapeQueryFilter, true)};


        if (resultQueryHitType == physx::PxQueryHitType::eTOUCH && m_ignoreTouches == true)
        {
            m_preFilterReturnValue = physx::PxQueryHitType::eNONE;
            return m_preFilterReturnValue;
        }

        if (resultQueryHitType == physx::PxQueryHitType::eBLOCK && m_ignoreBlocks == true)
        {
            m_preFilterReturnValue = physx::PxQueryHitType::eNONE;
            return m_preFilterReturnValue;
        }

        for (Entity* entity: m_ignoredEntities)
        {
            if (entity != nullptr && entity->GetInstanceID() == shapeQueryFilter.word0)
            {
                m_preFilterReturnValue = physx::PxQueryHitType::eNONE;
                return m_preFilterReturnValue;
            }
        }

        for (PrimitiveComponent* component : m_ignoredComponents)
        {
            if (component != nullptr && component->GetInstanceID() == p_shape->getSimulationFilterData().word2)
            {
                m_preFilterReturnValue = physx::PxQueryHitType::eNONE;
                return m_preFilterReturnValue;
            }
        }

        if (m_isOverlapQuery == true && resultQueryHitType == physx::PxQueryHitType::eBLOCK)
        {
            resultQueryHitType = physx::PxQueryHitType::eTOUCH;
        }

        m_preFilterReturnValue = resultQueryHitType;
        return resultQueryHitType;
    }

    return m_preFilterReturnValue;
}
        
