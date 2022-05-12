#include "PCH.hpp"

#include "PhysicsEngine/PhysicsEngine.hpp"
#include "PhysicsEngine/PhysicsSimulationFilterShader.hpp"


#include "PxPhysicsAPI.h"

// ==============================[Public Constructor + Destructor]============================== //

/**
 * Default Constructor
 */
PhysicsEngine::PhysicsEngine    ()
:   EngineModule(),
    m_physXCooking              {nullptr},
    m_physXFoundation           {nullptr},
    m_physXSDK                  {nullptr},
    m_fixeDeltaTime             {DEFAULT_FIXE_DELTA_TIME},
    m_maxSubstep                {3u},
    m_defaultPhysicalMaterial   {}
{}

// ============================== [Virtual Public Local Methods] ============================== //

/**
 * Initialize the Physics Engine.
 * Create PhysXFoundation, PhyssXSDK and PhysXCooking. If one of them is already created, it's produces an error.
 */
void    PhysicsEngine::Initialize   (EngineKey const&   p_passkey)
{
    LOG(LogPhysics, Display, "Initialization Physics Engine...");

    InitalizePhysXFoundation();
    InitializePhysXSDK();
    InitializePhysXCooking();
    InitializePhysicsScene();
    InitializeDefaultPhysicalMaterial();

    m_initialized = true;

    LOG(LogPhysics, Display, "Physics Engine initialized.");
}

void    PhysicsEngine::Update       (EngineKey const&   p_passkey)
{
}

/**
 * Shut Down Physics Engine.
 * Release PhysXCooking, PhysXSDK and PhysXFoundation.
 */
void    PhysicsEngine::Shutdown     (EngineKey const&   p_passkey)
{
    LOG(LogPhysics, Display, "Shutting down Physics Engine...");

    ShutdownPhysicsScene();
    ShutdownPhysXCooking();
    ShutdownPhysXSDK();
    ShutdownPhysXFoundation();

    m_initialized = false;

    LOG(LogPhysics, Display, "Physics Engine shut down ended.");
}


// ============================== [Public Local Methods] ============================== //

PhysicsSceneHandle* PhysicsEngine::GetPhysicsScene              ()
{
    return &m_physicScene;
}


PhysicalMaterial*   PhysicsEngine::GetDefaultPhysicalMaterial    ()
{
    return &m_defaultPhysicalMaterial;
}

// ============================== [Private Local Methods] ============================== //

void    PhysicsEngine::InitializeDefaultPhysicalMaterial    ()
{
    physx::PxMaterial*      defaultPhysXMaterial            {m_physXSDK->createMaterial(1.0f, 1.0f, 0.0f)};
    PhysicsMaterialHandle&  defaultPhysicsMaterialHandle    {m_defaultPhysicalMaterial.GetPhysicsMaterialHandle()};

    defaultPhysicsMaterialHandle.m_material = defaultPhysXMaterial;
    defaultPhysXMaterial->userData = &defaultPhysicsMaterialHandle;
}

void    PhysicsEngine::InitializePhysicsScene               ()
{
    LOG(LogPhysics, Display, "Initialization Physics Scene...");


    physx::PxSceneDesc  sceneDescriptor {m_physXSDK->getTolerancesScale()};

    sceneDescriptor.gravity                 = physx::PxVec3(0.0f, -9.81f, 0.0f);
    sceneDescriptor.filterShader            = PhysicsSimulationFilterShader;
    sceneDescriptor.simulationEventCallback = &m_physicsSimulationEventCallback;
    sceneDescriptor.flags                   = physx::PxSceneFlags   (physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS | 
                                                                     physx::PxSceneFlag::eENABLE_CCD | 
                                                                     physx::PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS);

    physx::PxDefaultCpuDispatcher* cpuDispatcher    {physx::PxDefaultCpuDispatcherCreate(0)};

    if (!cpuDispatcher)
    {
        LOG(LogPhysics, Fatal, "PxDefaultCpuDispatcherCreate failed !");
    }
    sceneDescriptor.cpuDispatcher           = cpuDispatcher;


    physx::PxScene* physxScene  {m_physXSDK->createScene(sceneDescriptor)};
    
    if (!physxScene)
    {
        LOG(LogPhysics, Fatal, "Physics Scene creation from PhysXSDK failed !");
    }
    m_physicScene.SetPhysXScene(physxScene);

    LOG(LogPhysics, Display, "Physics Scene initialized.");
}


/**
 * Initialize PhysX Foundation.
 */
void    PhysicsEngine::InitalizePhysXFoundation             ()
{
    LOG(LogPhysics, Display, "Creation of PhysX Foundation...");
    if (m_physXFoundation == nullptr)
    {
        m_physXFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, m_physXAllocatorCallback, m_physXErrorCallback);
       
       if (m_physXFoundation == nullptr)
        {
            LOG(LogPhysics, Fatal, "PhysX Foundation creation failed !");
        }
        else // (m_physXFoundation != nullptr)
        {
            LOG(LogPhysics, Display, "PhysX Foundation creation succed !");
        }
    }
            
    else // (m_physXFoundation == nullptr)
    {
        LOG(LogPhysics, Warning, "PhysX Foundation is already initalized !");
    }
}

/**
 * Initialize PhysX Cooking.
 */
void    PhysicsEngine::InitializePhysXCooking               ()
{
    LOG(LogPhysics, Display, "Creation of PhysX Cooking...");
    
    if (m_physXCooking == nullptr)
    {
        // Create Cooking //
        m_physXCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_physXFoundation, m_physXSDK->getTolerancesScale());

        if (m_physXCooking == nullptr)
        {
            LOG(LogPhysics, Fatal, "PhysX Cooking creation failed !");
        }
        
        else // (m_physXCooking != nullptr)
        {
            LOG(LogPhysics, Display, "PhysX Cooking creation succed !");
        }
    }

    else // (m_physXCooking != nullptr)
    {
        LOG(LogPhysics, Warning, "PhysX Cooking is already initalized !");
    }
}

/**
 * Initialize PhysX SDK.
 */
void    PhysicsEngine::InitializePhysXSDK                   ()
{
    LOG(LogPhysics, Display, "Creation of PhysX SDK...");

    if (m_physXSDK == nullptr)
    {
        // Create Physics //
        physx::PxTolerancesScale toleranceScale;

        m_physXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_physXFoundation, toleranceScale);
        if (m_physXSDK == nullptr)
        {
            LOG(LogPhysics, Fatal, "PhysX SDK creation failed !");
        }
        
        else // (m_physXSDK != nullptr)
        {
            LOG(LogPhysics, Display, "PhysX SDK creation succed !");
        }
    }

    else // (m_physXSDK != nullptr)
    {
        LOG(LogPhysics, Warning, "PhysX SDK is already initalized !");
    }
}


/**
 * ShutDown PhysX Foundation.
 */
void    PhysicsEngine::ShutdownPhysXFoundation              ()
{
    LOG(LogPhysics, Display, "Shutting down PhysX Foundation...");

    if (m_physXFoundation != nullptr)
    {
        m_physXFoundation->release();
        m_physXFoundation = nullptr;
        LOG(LogPhysics, Display, "Shutting down PhysX Foundation succed !");
    }
    else
    {
        LOG(LogPhysics, Warning, "Can't shut down PhysX Foundation because it's not initalized");
    }

    LOG(LogPhysics, Display, "Shutting down PhysX Foundation ended.");
}

/**
 * ShutDown PhysX Cooking.
 */
void    PhysicsEngine::ShutdownPhysXCooking                 ()
{
    LOG(LogPhysics, Display, "Shutting down PhysX Cooking...");

    if (m_physXCooking != nullptr)
    {
        m_physXCooking->release();
        m_physXCooking = nullptr;
        LOG(LogPhysics, Display, "Shutting down PhysX Cooking succed !");
    }
    else
    {
        LOG(LogPhysics, Warning, "Can't shut down PhysX Cooking because it's not initalized");
    }

    LOG(LogPhysics, Display, "Shutting down PhysX Cooking ended.");
}

/**
 * ShutDown PhysX SDK.
 */
void    PhysicsEngine::ShutdownPhysXSDK                     ()
{
    LOG(LogPhysics, Display, "Shutting down PhysX SDK...");

    if (m_physXSDK != nullptr)
    {
        m_physXSDK->release();
        m_physXSDK = nullptr;
        LOG(LogPhysics, Display, "Shutting down PhysX SDK succed !");

    }
    else
    {
        LOG(LogPhysics, Warning, "Can't shut down PhysX SDK because it's not initalized");
    }

    LOG(LogPhysics, Display, "Shutting down PhysX SDK ended.");
}

void    PhysicsEngine::ShutdownPhysicsScene                 ()
{
    m_physicScene.Release();
}