#include "PCH.hpp"

#include "PhysicsEngine/PhysicsSimulationFilterShader.hpp"

#include "EngineTypes.hpp"

// ==============================[Global Methods]============================== //

physx::PxFilterFlags    PhysicsSimulationFilterShader   (physx::PxFilterObjectAttributes    p_attributes0, 
                                                         physx::PxFilterData                p_filterData0, 
                                                         physx::PxFilterObjectAttributes    p_attributes1,
                                                         physx::PxFilterData                p_filterData1,
                                                         physx::PxPairFlags&                p_pairFlags, 
                                                         const void*                        p_constantBlock, 
                                                         physx::PxU32                       p_constantBlockSize)
{
    physx::PxFilterFlags    resultFilterFlags   {physx::PxFilterFlags()};

    bool const              isKinematic0        {physx::PxFilterObjectIsKinematic(p_attributes0)};
    bool const              isKinematic1        {physx::PxFilterObjectIsKinematic(p_attributes1)};


    // Checks if both object are Kinematic
    if (isKinematic0 && isKinematic1)
    {
        // manage kinematic kinematic //
        resultFilterFlags = physx::PxFilterFlag::eSUPPRESS;
    }

    else
    {
        bool const isRigidStatic0   {physx::PxGetFilterObjectType(p_attributes0) == physx::PxFilterObjectType::eRIGID_STATIC};
        bool const isRigidStatic1   {physx::PxGetFilterObjectType(p_attributes1) == physx::PxFilterObjectType::eRIGID_STATIC};

        // Ignore Kinematic-Static (Assume that that statics can't be flagged kinematics)
        if ((isKinematic0 || isKinematic1) && (isRigidStatic0 || isRigidStatic1))
        {
            resultFilterFlags = physx::PxFilterFlag::eSUPPRESS;
        }

        else
        {
            ECollisionChannel   channel0        {static_cast<ECollisionChannel>(p_filterData0.word3)};
            ECollisionChannel   channel1        {static_cast<ECollisionChannel>(p_filterData1.word3)};

            physx::PxU32        is0CollideWith1 {p_filterData0.word1 & (1 << static_cast<uint32>(channel1))};
            physx::PxU32        is1CollideWith0 {p_filterData1.word1 & (1 << static_cast<uint32>(channel0))};

            bool const          areColliding    {is0CollideWith1 && is1CollideWith0};

            if (areColliding == false)
            {
                resultFilterFlags = physx::PxFilterFlag::eSUPPRESS;
            }

            else
            {
                p_pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS | physx::PxPairFlag::eNOTIFY_CONTACT_POINTS;
            }
        }
    }

    return resultFilterFlags;
}
